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

//#include <avr/interrupt.h>
//#include <util/atomic.h>
//#include <avr/wdt.h>
//#include <util/delay.h>

#include "Main.hh"
#include "DebugPacketProcessor.hh"
#include "Host.hh"
#include "Command.hh"
#include "Timeout.hh"
#include "Steppers.hh"
#include "Motherboard.hh"
#include "SDCard.hh"
#include "Eeprom.hh"
#include "EepromMap.hh"
#include "ThermistorTable.hh"
#include "UtilityScripts.hh"
#include "Configuration.hh"
#include "Pin.hh"

#include "Delay.hh"
extern "C" {
	#include "LPC17xx.h"
//	#include "vcomdemo.c"
	#include "lpc17xx_nvic.h"
	#include "comm.h"
	#include "lpc17xx_wdt.h"
	#include "lpc17xx_rtc.h"
//#include "lpc17xx_clkpwr.h"
}

#define USER_FLASH_START 0x3000 // For USB bootloader
//#define USER_FLASH_START 0x0 // No USB bootloader
#define BOOTLOADER_START 0x0 // To enter bootloader

extern "C" void WDT_IRQHandler (void){
	NVIC_DisableIRQ(WDT_IRQn);
	xprintf("WDT_IRQHandler" " (%s:%d)\n",_F_,_L_);
	SCB->VTOR = (BOOTLOADER_START & 0x1FFFFF80);
	RTC_WriteGPREG(LPC_RTC, 2, 0xbbbbbbbb);
	WDT_Init (WDT_CLKSRC_PCLK, WDT_MODE_RESET);
	WDT_Start(1);
	NVIC_EnableIRQ(WDT_IRQn);
	while(1);
}

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
			NVIC_SetPriority(WDT_IRQn, 0);
			WDT_Init (WDT_CLKSRC_PCLK, WDT_MODE_INT_ONLY);
			WDT_Start(1000000);
			NVIC_EnableIRQ(WDT_IRQn);
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

extern volatile int LINE_READY;

volatile uint32_t TOG[4] = {0,0,0,0};
volatile uint32_t USER_MILLIS;

volatile char UART_LINE[50];
volatile uint32_t UART_LINE_LEN;

int main() {
	
	//----Initialization of LPC----//
	/* NOTE: you will need to call SystemCoreClockUpdate() as the very
	first line in your main function. This will update the various
	registers and constants to allow accurate timing. */
//	SystemCoreClockUpdate();
//	SystemInit();									// Initialize clocks
	// DeInit NVIC and SCBNVIC
//	NVIC_DeInit();
//	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits */
	// b100 bxxx.yy000    Group priority bits:[7:5]    Subpriority bits:[4:3]    Group priorities:8   Subpriorities:4 = 4
	// assign Stepper to group 0 sub-priority 1 = 1
	// assign USB in to group 1 sub-priority 0 = 4
	// assign USB out to group 2 sub-priority 0 = 8
	// assign Uart1 to group 2 sub-priority 1 = 9
	// assign Uart0 - debug to group 2 sub-priority 1 = 10
	// assign timer to group 3 sub-priority 0 = 12
	// assign pizo to group 4 sub-priority 0 = 16
	NVIC_SetPriorityGrouping(4);

//	SCB->VTOR = (USER_FLASH_START & 0x1FFFFF80);

	comm_init();
	comm_flush();
//	xprintf("\033[2J");
	xprintf("\r\n\r\n\r\n\r\n\r\n**BOOTED**" " (%s:%d)\n",_F_,_L_);
//	xprintf("before set direction" " (%s:%d)\n",_F_,_L_);
	DEBUG_LED1.setDirection(true);
	DEBUG_LED2.setDirection(true);
	DEBUG_LED3.setDirection(true);
	DEBUG_LED4.setDirection(true);
	xprintf("%x" " (%s:%d)\n",LPC_SC->PCLKSEL1,_F_,_L_);
	xprintf("%x" " (%s:%d)\n",&GPIO_SetValue,_F_,_L_);
	xprintf("%x" " (%s:%d)\n",&GPIO_ClearValue,_F_,_L_);
//	xprintf("after set direction" " (%s:%d)\n",_F_,_L_);
	xprintf("after A_DIR_PIN.setValue(false);" " (%s:%d)\n",_F_,_L_);
//	while(1){
		DEBUG_LED1.setValue(false);
		DEBUG_LED2.setValue(false);
		DEBUG_LED3.setValue(false);
		DEBUG_LED4.setValue(false);
		xprintf("after A_DIR_PIN.setValue(false);" " (%s:%d)\n",_F_,_L_);
		_delay_us(100000);
		DEBUG_LED1.setValue(true);
		DEBUG_LED2.setValue(true);
		DEBUG_LED3.setValue(true);
		DEBUG_LED4.setValue(true);
		xprintf("after A_DIR_PIN.setValue(true);" " (%s:%d)\n",_F_,_L_);
		_delay_us(100000);
//	}
//	for (uint32_t i = 0x10000000; i < 0x10007fff0; i++,i++,i++,i++) {
//		xprintf("%x %x %c\n",i,eeprom_address(i, 0),eeprom_address(i, 0));
//		_delay_us(1000);
//	}

	Motherboard& board = Motherboard::getBoard();
	reset(true);
	xprintf("reset done" " (%s:%d)\n",_F_,_L_);
	steppers::init();
	xprintf("stepper init done" " (%s:%d)\n",_F_,_L_);
//	sei();
		xprintf("Loop" " (%s:%d)\n",_F_,_L_);
//		eeprom::read_all_from_flash();
	while (1) {
		// Host interaction thread.
		host::runHostSlice();
		// Command handling thread.
		command::runCommandSlice();
		// Motherboard slice
		board.runMotherboardSlice();
        // reset the watch dog timer
		WDT_Feed ();
		if(LINE_READY){
			exec_cmd(UART_LINE);
			LINE_READY=0;
		}
//		wdt_reset();
		uint32_t loop;
		loop++;
		if (loop > 5000){
			loop=0;
			xprintf(".");
		}
//		_delay_us(100000);
	}
	return 0;
}
