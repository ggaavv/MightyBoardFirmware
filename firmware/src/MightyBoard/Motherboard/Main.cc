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

#include "Main.hh"
#include "DebugPacketProcessor.hh"
#include "Host.hh"
#include "Command.hh"
//#include <avr/interrupt.h>
//#include <util/atomic.h>
//#include <avr/wdt.h>
#include "Timeout.hh"
#include "Steppers.hh"
#include "Motherboard.hh"
#include "SDCard.hh"
#include "Eeprom.hh"
#include "EepromMap.hh"
#include "ThermistorTable.hh"
//#include <util/delay.h>
#include "UtilityScripts.hh"

#include "Delay.hh"
extern "C" {
	#include "LPC17xx.h"
	#include "vcomdemo.c"
	#include "lpc17xx_nvic.h"
	#include "comm.h"
}

#define USER_FLASH_START 0x3000 // For USB bootloader
//#define USER_FLASH_START 0x0 // No USB bootloader
#define BOOTLOADER_START 0x0 // To enter bootloader


void reset(bool hard_reset) {
//	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		
//		bool brown_out = false;
//		uint8_t resetFlags = MCUSR & 0x0f;
		// check for brown out reset flag and report if true
//		if(resetFlags & (1 << 2)){
//			brown_out = true;
//		}
		
        // clear watch dog timer and re-enable
		if(hard_reset)
		{ 
            // ATODO: remove disable
//			wdt_disable();
//			MCUSR = 0x0;
//			wdt_enable(WDTO_8S); // 8 seconds is max timeout
		}
		
		// initialize major classes
		Motherboard& board = Motherboard::getBoard();
		sdcard::reset();
		xprintf("sdcard::reset" " (%s:%d)\n",_F_,_L_);
		utility::reset();
		xprintf("utility::reset" " (%s:%d)\n",_F_,_L_);
		planner::init();
		xprintf("planner::init" " (%s:%d)\n",_F_,_L_);
		planner::abort();
		command::reset();
		eeprom::init();
		xprintf("eeprom::init" " (%s:%d)\n",_F_,_L_);
		steppers::reset();
		xprintf("steppers::reset" " (%s:%d)\n",_F_,_L_);
		initThermistorTables();
		board.reset(hard_reset);
		xprintf("board.reset(hard_reset)" " (%s:%d)\n",_F_,_L_);
		
	// brown out occurs on normal power shutdown, so this is not a good message		
	//	if(brown_out)
	//	{
	//		board.getInterfaceBoard().errorMessage("Brown-Out Reset     Occured", 27);
	//		board.startButtonWait();
	//	}	
//	}
}

int main() {
	
	//----Initialization of LPC----//
	/* NOTE: you will need to call SystemCoreClockUpdate() as the very
	first line in your main function. This will update the various
	registers and constants to allow accurate timing. */
//	SystemCoreClockUpdate();
//	SystemInit();									// Initialize clocks
	// DeInit NVIC and SCBNVIC
	NVIC_DeInit();
	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits */
	// b100 bxxx.yy000    Group priority bits:[7:5]    Subpriority bits:[4:3]    Group priorities:8   Subpriorities:4 = 4
	// assign Stepper to group 0 sub-priority 0 = 0
	// assign USB in to group 1 sub-priority 0 = 4
	// assign USB out to group 2 sub-priority 0 = 8
	// assign Uart1 to group 2 sub-priority 1 = 9
	// assign timer to group 3 sub-priority 0 = 12
	// assign pizo to group 4 sub-priority 0 = 16
	NVIC_SetPriorityGrouping(4);

	SCB->VTOR = (USER_FLASH_START & 0x1FFFFF80);

	comm_init();
//	xprintf("\033[2J");
	xprintf("\r\n\r\n\r\n\r\n\r\n**BOOTED**" " (%s:%d)\n",_F_,_L_);

	xprintf("before set direction" " (%s:%d)\n",_F_,_L_);
	A_DIR_PIN.setDirection(true);
	xprintf("after set direction" " (%s:%d)\n",_F_,_L_);

//	while(1){
		A_DIR_PIN.setValue(false);
		xprintf("after A_DIR_PIN.setValue(false);" " (%s:%d)\n",_F_,_L_);
		_delay_us(100000);
		A_DIR_PIN.setValue(true);
		xprintf("after A_DIR_PIN.setValue(true);" " (%s:%d)\n",_F_,_L_);
		_delay_us(100000);
//	}

	Motherboard& board = Motherboard::getBoard();
	reset(true);
	xprintf("reset done" " (%s:%d)\n",_F_,_L_);
	steppers::init();
	xprintf("stepper init done" " (%s:%d)\n",_F_,_L_);
//	sei();
		xprintf("Loop" " (%s:%d)\n",_F_,_L_);
	while (1) {
		// Host interaction thread.
		host::runHostSlice();
		// Command handling thread.
		command::runCommandSlice();
		// Motherboard slice
		board.runMotherboardSlice();
        // reset the watch dog timer
//		wdt_reset();
		uint32_t loop;
		loop++;
		if (loop > 50000){
			loop=0;
			xprintf(".");
		}
//		_delay_us(100000);
	}
	return 0;
}
