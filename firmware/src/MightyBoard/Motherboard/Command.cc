/*
 * Copyright 2010 by Adam Mayer	 <adam@makerbot.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "Command.hh"
#include "Steppers.hh"
#include "Commands.hh"
#include "Configuration.hh"
#include "Timeout.hh"
#include "CircularBuffer.hh"
//#include <util/atomic.h>
//#include <avr/eeprom.h>
#include "EepromMap.hh"
#include "SDCard.hh"
#include "Pin.hh"
//#include <util/delay.h>
#include "Piezo.hh"
#include "RGB_LED.hh"
//#include "Interface.hh"
#include "UtilityScripts.hh"
#include "Planner.hh"
#include "stdio.h"
#include "Menu_locales.hh"

#include "Host.hh"
#include "Delay.hh"
#include "Eeprom.hh"

extern "C" {
	#include "lpc_types.h"
#include "comm.h"
}

namespace command {

#define COMMAND_BUFFER_SIZE 512
uint8_t buffer_data[COMMAND_BUFFER_SIZE];
CircularBuffer command_buffer(COMMAND_BUFFER_SIZE, buffer_data);
uint8_t currentToolIndex = 0;

uint32_t line_number;

bool outstanding_tool_command = false;
bool check_temp_state = false;
bool paused = false;
bool heat_shutdown = false;
uint32_t sd_count = 0;

uint16_t getRemainingCapacity() {
	uint16_t sz;
//	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		sz = command_buffer.getRemainingCapacity();
//	}
	return sz;
}

void pause(bool pause) {
	paused = pause;
}
void heatShutdown(){
	heat_shutdown = true;
}

bool isPaused() {
	return paused;
}

bool isEmpty() {
	return command_buffer.isEmpty();
}

void push(uint8_t byte) {
	command_buffer.push(byte);
}

uint8_t pop8() {
//	sd_count ++;
	return command_buffer.pop();
}

int16_t pop16() {
	union {
		// AVR is little-endian
		int16_t a;
		struct {
			uint8_t data[2];
		} b;
	} shared;
	shared.b.data[0] = command_buffer.pop();
	shared.b.data[1] = command_buffer.pop();
//	sd_count+=2;
	return shared.a;
}

int32_t pop32() {
	union {
		// AVR is little-endian
		int32_t a;
		struct {
			uint8_t data[4];
		} b;
	} shared;
	shared.b.data[0] = command_buffer.pop();
	shared.b.data[1] = command_buffer.pop();
	shared.b.data[2] = command_buffer.pop();
	shared.b.data[3] = command_buffer.pop();
//	sd_count+=4;
	return shared.a;
}

enum {
	READY,
	MOVING,
	DELAY,
	HOMING,
	WAIT_ON_TOOL,
	WAIT_ON_PLATFORM,
	WAIT_ON_BUTTON
} mode = READY;

Timeout delay_timeout;
Timeout homing_timeout;
Timeout tool_wait_timeout;
Timeout button_wait_timeout;
/// Bitmap of button pushes to wait for
uint8_t button_mask;
enum {
	BUTTON_TIMEOUT_CONTINUE = 0,
	BUTTON_TIMEOUT_ABORT = 1,
	BUTTON_CLEAR_SCREEN = 2
};
/// Action to take when button times out
uint8_t button_timeout_behavior;


bool sdcard_reset = false;

void reset() {
	command_buffer.reset();
	line_number = 0;
	check_temp_state = false;
	paused = false;
	sd_count = 0;
	sdcard_reset = false;
	mode = READY;
}

bool isWaiting(){
	return (mode == WAIT_ON_BUTTON);
}
    
bool isReady() {
    return (mode == READY);
}

uint32_t getLineNumber() {
	return line_number;	
}

void clearLineNumber() {
	line_number = 0;
}


// Handle movement comands -- called from a few places
static void handleMovementCommand(const uint8_t &command) {
//	xprintf("handleMovementCommand" " (%s:%d)\n",_F_,_L_);
	// if we're already moving, check to make sure the buffer isn't full
	if (/*mode == MOVING && */planner::isBufferFull()) {
		return; // we'll be back!
	}
	if (command == HOST_CMD_QUEUE_POINT_EXT) {
//		xprintf("HOST_CMD_QUEUE_POINT_EXT" " (%s:%d)\n",_F_,_L_);
		// check for completion
		if (command_buffer.getLength() >= 25) {
			Motherboard::getBoard().resetUserInputTimeout();
			pop8(); // remove the command code
			mode = MOVING;

			int32_t x = pop32();
			int32_t y = pop32();
			int32_t z = pop32();
			int32_t a = pop32();
			int32_t b = pop32();
			int32_t dda = pop32();

			line_number++;
		
			planner::addMoveToBuffer(Point(x,y,z,a,b), dda);
		}
	}
	 else if (command == HOST_CMD_QUEUE_POINT_NEW) {
//		xprintf("HOST_CMD_QUEUE_POINT_NEW" " (%s:%d)\n",_F_,_L_);
		// check for completion
		if (command_buffer.getLength() >= 26) {
			Motherboard::getBoard().resetUserInputTimeout();
			pop8(); // remove the command code
			mode = MOVING;
			
			int32_t x = pop32();
			int32_t y = pop32();
			int32_t z = pop32();
			int32_t a = pop32();
			int32_t b = pop32();
			int32_t us = pop32();
			int32_t relative = pop8();

			line_number++;
			
			planner::addMoveToBufferRelative(Point(x,y,z,a,b), us, relative);
		}
	}
	
}

bool processExtruderCommandPacket() {
	Motherboard& board = Motherboard::getBoard();
        uint8_t	id = pop8();
		uint8_t command = pop8();
		uint8_t length = pop8();

		switch (command) {
		case SLAVE_CMD_SET_TEMP:
//			xprintf("SLAVE_CMD_SET_TEMP" " (%s:%d)\n",_F_,_L_);
			board.getPlatformHeater().set_target_temperature(pop16());
			/// if platform is actively heating and extruder is not cooling down, pause extruder
			if(board.getPlatformHeater().isHeating() && !board.getPlatformHeater().isCooling() && !board.getExtruderBoard(id).getExtruderHeater().isCooling()){
				check_temp_state = true;
				board.getExtruderBoard(id).getExtruderHeater().Pause(true);
			}  /// else ensure extruder is not paused  
			else {
				board.getExtruderBoard(id).getExtruderHeater().Pause(false);
			}
			Motherboard::getBoard().setBoardStatus(Motherboard::STATUS_PREHEATING, false);
			return true;
		// can be removed in process via host query works OK
 		case SLAVE_CMD_PAUSE_UNPAUSE:
			host::pauseBuild(!command::isPaused());
			return true;
		case SLAVE_CMD_TOGGLE_FAN:
			board.getExtruderBoard(id).setFan((pop8() & 0x01) != 0);
			return true;
		case SLAVE_CMD_TOGGLE_VALVE:
			board.setValve((pop8() & 0x01) != 0);
			return true;
		case SLAVE_CMD_SET_PLATFORM_TEMP:
//			xprintf("SLAVE_CMD_SET_PLATFORM_TEMP" " (%s:%d)\n",_F_,_L_);
			board.setUsingPlatform(true);
			board.getPlatformHeater().set_target_temperature(pop16());
			// pause extruder heaters platform is heating up
			bool pause_state; /// avr-gcc doesn't allow cross-initializtion of variables within a switch statement
			pause_state = false;
			if(!board.getPlatformHeater().isCooling()){
				pause_state = true;
			}
			check_temp_state = pause_state;
			board.getExtruderBoard(0).getExtruderHeater().Pause(pause_state);
			board.getExtruderBoard(1).getExtruderHeater().Pause(pause_state);
			Motherboard::getBoard().setBoardStatus(Motherboard::STATUS_PREHEATING, false);
			
			return true;
        // not being used with 5D
		case SLAVE_CMD_TOGGLE_MOTOR_1:
			pop8();
			return true;
        // not being used with 5D
		case SLAVE_CMD_TOGGLE_MOTOR_2: 
			pop8();
			return true;
		case SLAVE_CMD_SET_MOTOR_1_PWM:
//			xprintf("SLAVE_CMD_SET_MOTOR_1_PWM" " (%s:%d)\n",_F_,_L_);
			pop8();
			return true;
		case SLAVE_CMD_SET_MOTOR_2_PWM:
			pop8();
			return true;
		case SLAVE_CMD_SET_MOTOR_1_DIR:
			pop8();
			return true;
		case SLAVE_CMD_SET_MOTOR_2_DIR:
			pop8();
			return true;
		case SLAVE_CMD_SET_MOTOR_1_RPM:
			pop32();
			return true;
		case SLAVE_CMD_SET_MOTOR_2_RPM:
			pop32();
			return true;
		case SLAVE_CMD_SET_SERVO_1_POS:
			pop8();
			return true;
		case SLAVE_CMD_SET_SERVO_2_POS:
			pop8();
			return true;
		}
	return false;
}


// A fast slice for processing commands and refilling the stepper queue, etc.
void runCommandSlice() {
    // get command from SD card if building from SD
	if (sdcard::isPlaying()) {
		DEBUG_PIN2.setValue(true);
		while (command_buffer.getRemainingCapacity() > 0 && sdcard::playbackHasNext()) {
			sd_count++;
			command_buffer.push(sdcard::playbackNext());
		}
		if(!sdcard::playbackHasNext() && (sd_count < sdcard::getFileSize()) && !sdcard_reset){
			
//			Motherboard::getBoard().getInterfaceBoard().resetLCD();
			Motherboard::getBoard().errorResponse(STATICFAIL_MSG);
			sdcard_reset = true;
			/// do the sd card initialization files
			//command_buffer.reset();
			//sdcard::startPlayback(host::getBuildName());
			//uint32_t count;
			//while(count < sd_count){
			//	sdcard::playbackNext();
			//}
		}else if(!sdcard::playbackHasNext() && command_buffer.isEmpty()){
			sdcard::finishPlayback();
		}
	}
    // get command from onboard script if building from onboard
	if(utility::isPlaying()){		
		while (command_buffer.getRemainingCapacity() > 0 && utility::playbackHasNext()){
			command_buffer.push(utility::playbackNext());
		}
		if(!utility::playbackHasNext() && command_buffer.isEmpty()){
			utility::finishPlayback();
		}
	}
	
	// if printer is not waiting for tool or platform to heat, we need to make
	// sure the extruders are not in a paused state.  this is relevant when 
	// heating using the control panel in desktop software
	if(check_temp_state){
		if (Motherboard::getBoard().getPlatformHeater().has_reached_target_temperature()){
			// unpause extruder heaters in case they are paused
			Motherboard::getBoard().getExtruderBoard(0).getExtruderHeater().Pause(false);
			Motherboard::getBoard().getExtruderBoard(1).getExtruderHeater().Pause(false);
			check_temp_state = false;
		}
	}
	// don't execute commands if paused or shutdown because of heater failure
	if (paused || heat_shutdown) {	return; }
	if (mode == HOMING) {
//		xprintf("HOMING" " (%s:%d)\n",_F_,_L_);
		if (!steppers::isRunning()) {
			mode = READY;
		} else if (homing_timeout.hasElapsed()) {
			planner::abort();
			mode = READY;
		}
	}
	if (mode == MOVING) {
		if (!steppers::isRunning()) {
			mode = READY;
		} else {
			if (command_buffer.getLength() > 0) {
				Motherboard::getBoard().resetUserInputTimeout();
				uint8_t command = command_buffer[0];
				if (command == HOST_CMD_QUEUE_POINT_EXT || command == HOST_CMD_QUEUE_POINT_NEW) {
//					xprintf("command == HOST_CMD_QUEUE_POINT_EXT || command == HOST_CMD_QUEUE_POINT_NEW" " (%s:%d)\n",_F_,_L_);
					handleMovementCommand(command);
				}
				else if (command == HOST_CMD_ENABLE_AXES) {
					if (command_buffer.getLength() >= 2) {
						pop8(); // remove the command code
						uint8_t axes = pop8();
						}
				}
			}
		}
	}
	if (mode == DELAY) {
		// check timers
		if (delay_timeout.hasElapsed()) {
			mode = READY;
		}
	}
	if (mode == WAIT_ON_TOOL) {
		if(tool_wait_timeout.hasElapsed()){
			Motherboard::getBoard().errorResponse("I timed out while   attempting to heat  my extruder."); 
			mode = READY;		
		}
		else if(!Motherboard::getBoard().getExtruderBoard(currentToolIndex).getExtruderHeater().isHeating()){
			mode = READY;
		}else if( Motherboard::getBoard().getExtruderBoard(currentToolIndex).getExtruderHeater().has_reached_target_temperature()){
            mode = READY;
		}
	}
	if (mode == WAIT_ON_PLATFORM) {
		if(tool_wait_timeout.hasElapsed()){
			Motherboard::getBoard().errorResponse("I timed out while   attempting to heat  my platform."); 
			mode = READY;		
		} else if (!Motherboard::getBoard().getPlatformHeater().isHeating()){
			mode = READY;
		}
		else if(Motherboard::getBoard().getPlatformHeater().has_reached_target_temperature()){
            mode = READY;
		}
	}
	if (mode == WAIT_ON_BUTTON) {
		if (button_wait_timeout.hasElapsed()) {
			if (button_timeout_behavior & (1 << BUTTON_TIMEOUT_ABORT)) {
				// Abort build!
				// We'll interpret this as a catastrophic situation
				// and do a full reset of the machine.
				Motherboard::getBoard().reset(false);

			} else {
				mode = READY;
			//	Motherboard::getBoard().interfaceBlink(0,0);
			}
		} else {
			// Check buttons
//			InterfaceBoard& ib = Motherboard::getBoard().getInterfaceBoard();
//			if (ib.buttonPushed()) {			
//				if(button_timeout_behavior & (1 << BUTTON_CLEAR_SCREEN))
//					ib.popScreen();
//				Motherboard::getBoard().interfaceBlink(0,0);
//				RGB_LED::setDefaultColor();
//				mode = READY;
//			}
		}
	}

	if (mode == READY) {
//		xprintf("mode == READY" " (%s:%d)\n",_F_,_L_);
		
		// process next command on the queue.
		if ((command_buffer.getLength() > 0)){
//			xprintf("command_buffer.getLength() > 0" " (%s:%d)\n",_F_,_L_);
			Motherboard::getBoard().resetUserInputTimeout();
			
			uint8_t command = command_buffer[0];

		if (command == HOST_CMD_QUEUE_POINT_EXT || command == HOST_CMD_QUEUE_POINT_NEW) {
					handleMovementCommand(command);
			}  else if (command == HOST_CMD_CHANGE_TOOL) {
				if (command_buffer.getLength() >= 2) {
					pop8(); // remove the command code
                    currentToolIndex = pop8();
                    line_number++;
                    
                    planner::changeToolIndex(currentToolIndex);
				}
			} else if (command == HOST_CMD_ENABLE_AXES) {
				if (command_buffer.getLength() >= 2) {
					pop8(); // remove the command code
					uint8_t axes = pop8();
					line_number++;
					
					bool enable = (axes & 0x80) != 0;
					for (int i = 0; i < STEPPER_COUNT; i++) {
						if ((axes & _BV(i)) != 0) {
							steppers::enableAxis(i, enable);
						}
					}
				}
			} else if (command == HOST_CMD_SET_POSITION_EXT) {
//				xprintf("HOST_CMD_SET_POSITION_EXT) {" " (%s:%d)\n",_F_,_L_);
				// check for completion
				if (command_buffer.getLength() >= 21) {
					pop8(); // remove the command code
					int32_t x = pop32();
					int32_t y = pop32();
					int32_t z = pop32();
					int32_t a = pop32();
					int32_t b = pop32();
					line_number++;
//					xprintf("if (command_buffer.getLength() >= 21) {" " (%s:%d)\n",_F_,_L_);
					planner::definePosition(Point(x,y,z,a,b));
				}
			} else if (command == HOST_CMD_DELAY) {
				if (command_buffer.getLength() >= 5) {
					mode = DELAY;
					pop8(); // remove the command code
					// parameter is in milliseconds; timeouts need microseconds
					uint32_t microseconds = pop32() * 1000L;
					line_number++;
					
					delay_timeout.start(microseconds);
				}
			} else if (command == HOST_CMD_PAUSE_FOR_BUTTON) {
				if (command_buffer.getLength() >= 5) {
					pop8(); // remove the command code
					button_mask = pop8();
					uint16_t timeout_seconds = pop16();
					button_timeout_behavior = pop8();
					line_number++;
					
					if (timeout_seconds != 0) {
						button_wait_timeout.start(timeout_seconds * 1000L * 1000L);
					} else {
						button_wait_timeout = Timeout();
					}
                    // set button wait via interface board
					Motherboard::getBoard().interfaceBlink(25,15);
//					InterfaceBoard& ib = Motherboard::getBoard().getInterfaceBoard();
//					ib.waitForButton(button_mask);
					mode = WAIT_ON_BUTTON;
				}
			} else if (command == HOST_CMD_DISPLAY_MESSAGE) {
//				MessageScreen* scr = Motherboard::getBoard().getMessageScreen();
				if (command_buffer.getLength() >= 6) {
					pop8(); // remove the command code
					uint8_t options = pop8();
					uint8_t xpos = pop8();
					uint8_t ypos = pop8();
					uint8_t timeout_seconds = pop8();
					line_number++;
					
                    // check message clear bit
//					if ( (options & (1 << 0)) == 0 ) { scr->clearMessage(); }
					// set position and add message
//					scr->setXY(xpos,ypos);
//					scr->addMessage(command_buffer);

					// push message screen if the full message has been recieved
//					if((options & (1 << 1))){
//						InterfaceBoard& ib = Motherboard::getBoard().getInterfaceBoard();
//						if (ib.getCurrentScreen() != scr) {
//							ib.pushScreen(scr);
//						} else {
//							scr->refreshScreen();
//						}
						// set message timeout if not a buttonWait call
//						if ((timeout_seconds != 0) && (!(options & (1 <<2)))) {
//								scr->setTimeout(timeout_seconds);//, true);
//						}

//						if (options & (1 << 2)) { // button wait bit --> start button wait
//							if (timeout_seconds != 0) {
//								button_wait_timeout.start(timeout_seconds * 1000L * 1000L);
//							} else {
//								button_wait_timeout = Timeout();
//							}
//							button_mask = (1 << ButtonArray::CENTER);  // center button
//							button_timeout_behavior &= (1 << BUTTON_CLEAR_SCREEN);
//							Motherboard::getBoard().interfaceBlink(25,15);
//							InterfaceBoard& ib = Motherboard::getBoard().getInterfaceBoard();
//							ib.waitForButton(button_mask);
//							mode = WAIT_ON_BUTTON;
//						}
//					}
				}
					
			} else if (command == HOST_CMD_FIND_AXES_MINIMUM ||
					command == HOST_CMD_FIND_AXES_MAXIMUM) {
//				xprintf("HOST_CMD_FIND_AXES_MINIMUM||HOST_CMD_FIND_AXES_MAXIMUM) {" " (%s:%d)\n",_F_,_L_);
				if (command_buffer.getLength() >= 8) {
					pop8(); // remove the command
					uint8_t flags = pop8();
					uint32_t feedrate = pop32(); // feedrate in us per step
					uint16_t timeout_s = pop16();
					line_number++;
					
					bool direction = command == HOST_CMD_FIND_AXES_MAXIMUM;
					mode = HOMING;
					homing_timeout.start(timeout_s * 1000L * 1000L);
					steppers::startHoming(command==HOST_CMD_FIND_AXES_MAXIMUM,
							flags,
							feedrate);
				}
			} else if (command == HOST_CMD_WAIT_FOR_TOOL) {
				if (command_buffer.getLength() >= 6) {
					mode = WAIT_ON_TOOL;
					pop8();
					currentToolIndex = pop8();
					uint16_t toolPingDelay = (uint16_t)pop16();
					uint16_t toolTimeout = (uint16_t)pop16();
					line_number++;
					
					// if we re-add handling of toolTimeout, we need to make sure
					// that values that overflow our counter will not be passed)
					tool_wait_timeout.start(toolTimeout*1000000L);
				}
			} else if (command == HOST_CMD_WAIT_FOR_PLATFORM) {
        // FIXME: Almost equivalent to WAIT_FOR_TOOL
				if (command_buffer.getLength() >= 6) {
					mode = WAIT_ON_PLATFORM;
					pop8();
					uint8_t currentToolIndex = pop8();
					uint16_t toolPingDelay = (uint16_t)pop16();
					uint16_t toolTimeout = (uint16_t)pop16();
					line_number++;
					
					// if we re-add handling of toolTimeout, we need to make sure
					// that values that overflow our counter will not be passed)
					tool_wait_timeout.start(toolTimeout*1000000L);
				}
			} else if (command == HOST_CMD_STORE_HOME_POSITION) {

				// check for completion
				if (command_buffer.getLength() >= 2) {
					pop8();
					uint8_t axes = pop8();
					line_number++;
					
					// Go through each axis, and if that axis is specified, read it's value,
					// then record it to the eeprom.
					for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
						if ( axes & (1 << i) ) {
							eeprom::setEeprom32(eeprom_offsets::AXIS_HOME_POSITIONS_STEPS + (i*4), steppers::getPosition()[i]);
//							xprintf("%x" " (%s:%d)\n",eeprom_offsets::AXIS_HOME_POSITIONS_STEPS + (i*4),_F_,_L_);
//							xprintf("%x" " (%s:%d)\n",steppers::getPosition()[i],_F_,_L_);
//							uint16_t offset = eeprom_offsets::AXIS_HOME_POSITIONS_STEPS + 4*i;
//							uint32_t position = steppers::getPosition()[i];
//							cli();
//							eeprom_write_block(&position, (void*) offset, 4);
//							sei();
						}
						//eeprom::save_to_flash();
					}
				}
			} else if (command == HOST_CMD_RECALL_HOME_POSITION) {
				// check for completion
				if (command_buffer.getLength() >= 2) {
					pop8();
					uint8_t axes = pop8();
					line_number++;

					Point newPoint = steppers::getPosition();

					for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
						if ( axes & (1 << i) ) {
							newPoint[i] = eeprom::getEeprom32(eeprom_offsets::AXIS_HOME_POSITIONS_STEPS + (i*4),0);
//							xprintf("%x %d" " (%s:%d)\n",EEPROM_START_ADDRESS + (eeprom_offsets::AXIS_HOME_POSITIONS_STEPS+(i*4))*4,eeprom_address(EEPROM_START_ADDRESS, eeprom_offsets::AXIS_HOME_POSITIONS_STEPS+(i*4)),_F_,_L_);
//							uint16_t offset = eeprom_offsets::AXIS_HOME_POSITIONS_STEPS + 4*i;
//							cli();
//							eeprom_read_block(&(newPoint[i]), (void*) offset, 4);
//							sei();
						}
					}

					planner::definePosition(newPoint);
				}

			}else if (command == HOST_CMD_SET_POT_VALUE){
				if (command_buffer.getLength() >= 3) {
					pop8(); // remove the command code
					uint8_t axis = pop8();
					uint8_t value = pop8();
					line_number++;
                    steppers::setAxisPotValue(axis, value);
				}
			}else if (command == HOST_CMD_SET_RGB_LED){
				if (command_buffer.getLength() >= 6) {
					pop8(); // remove the command code

					uint8_t red = pop8();
					uint8_t green = pop8();
					uint8_t blue = pop8();
					uint8_t blink_rate = pop8();

                    uint8_t effect = pop8();
                    line_number++;
                    RGB_LED::setLEDBlink(blink_rate);
                    RGB_LED::setCustomColor(red, green, blue);

				}
			}else if (command == HOST_CMD_SET_BEEP){
				if (command_buffer.getLength() >= 6) {
					pop8(); // remove the command code
					uint16_t frequency= pop16();
					uint16_t beep_length = pop16();
					uint8_t effect = pop8();
					line_number++;
                    Piezo::setTone(frequency, beep_length);

				}			
			}else if (command == HOST_CMD_TOOL_COMMAND) {
//				xprintf("HOST_CMD_TOOL_COMMAND" " (%s:%d)\n",_F_,_L_);
				if (command_buffer.getLength() >= 4) { // needs a payload
					uint8_t payload_length = command_buffer[3];
					if (command_buffer.getLength() >= 4+payload_length) {
							pop8(); // remove the command code
							line_number++;
							processExtruderCommandPacket();
				}
			}
			} else if (command == HOST_CMD_SET_BUILD_PERCENT){
//				xprintf("HOST_CMD_SET_BUILD_PERCENT) {" " (%s:%d)\n",_F_,_L_);
				if (command_buffer.getLength() >= 3){
					pop8(); // remove the command code
					uint8_t percent = pop8();
					uint8_t ignore = pop8(); // remove the reserved byte
					line_number++;
//					interface::setBuildPercentage(percent);
				}
			} else if (command == HOST_CMD_QUEUE_SONG ) //queue a song for playing
 			{
				/// Error tone is 0,
				/// End tone is 1,
				/// all other tones user-defined (defaults to end-tone)
				if (command_buffer.getLength() >= 2){
					pop8(); // remove the command code
					uint8_t songId = pop8();
					line_number++;
					if(songId == 0)
						Piezo::errorTone(4);
					else if (songId == 1 )
						Piezo::doneTone();
					else
						Piezo::errorTone(2);
				}

			} else if ( command == HOST_CMD_RESET_TO_FACTORY) {
				/// reset EEPROM settings to the factory value. Reboot bot.
				if (command_buffer.getLength() >= 2){
				pop8(); // remove the command code
				uint8_t options = pop8();
				line_number++;
				eeprom::factoryResetEEPROM();
				Motherboard::getBoard().reset(false);
				}
			} else if ( command == HOST_CMD_BUILD_START_NOTIFICATION) {
//				xprintf("HOST_CMD_BUILD_START_NOTIFICATION" " (%s:%d)\n",_F_,_L_);
				if (command_buffer.getLength() >= 5){
					pop8(); // remove the command code
					int buildSteps = pop32();
					line_number++;
					host::handleBuildStartNotification(command_buffer);		
				}
			 } else if ( command == HOST_CMD_BUILD_END_NOTIFICATION) {
				if (command_buffer.getLength() >= 2){
					pop8(); // remove the command code
					uint8_t flags = pop8();
					line_number++;
					host::handleBuildStopNotification(flags);
				}
			
			} else {
			}
		}
	}
	
	/// we're not handling overflows in the line counter.  Possibly implement this later.
	if(line_number > MAX_LINE_COUNT){
		line_number = MAX_LINE_COUNT + 1;
	}
}
}


