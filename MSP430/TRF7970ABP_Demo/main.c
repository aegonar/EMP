/*
 * {main.c}
 *
 * {main application}
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/****************************************************************
* Firmware for the TRF7970A Booster Pack on the MSP430G2553 Launch Pad.
* Rev. # 1.1 -- 27.Sep.2013
*
* DESCRIPTION: This Example detects 15693, Type A, and Type B NFC tags.
* It then indicates the Tag type through LED's on the TRF7970A Booster pack.
* The UID and the RSSI (field strength) are sent out via a UART at 9600 Baud
* and can be read on a Computer.  This Firmware assumes the IRQ SEL jumper on
* the Booster pack to be in position #2.
*
* The Trf7970 is an integrated analog front end and
* data framing system for a 13.56 MHz RFID reader system.
* Built-in programming options make it suitable for a wide range
* of applications both in proximity and vicinity RFID systems.
* The reader is configured by selecting the desired protocol in
* the control registers. Direct access to all control registers
* allows fine tuning of various reader parameters as needed.
*
* The Trf7970A is interfaced to a MSP430F2553 through a SPI (serial)
* interface using a hardware USCI. The MCU is the master device and
* initiates all communication with the reader.
*
* The anti-collision procedures (as described in the ISO
* standards 14443A/B and 15693) are implemented in the MCU
* firmware to help the reader detect and communicate with one
* PICC/VICC among several PICCs/VICCs.
*
*
* AUTHOR:   Mai Long    		DATE: 06 MAY 2013
* 			J.D. Crutchfield	DATE: 27 SEP 2013
*
* CHANGES:
* REV.    DATE        WHO    DETAIL
* 1.1	  Sep.2013    J.D.	 Fixed delay between EN high until TRF is ready
*
* BUILT WITH:
* Code Composer Studio Core Edition Version: 5.4.0.00091
* (C) Copyright Texas Instruments, 2009. All rights reserved.
*****************************************************************/

//===============================================================
// Program with hardware USART and SPI communication	        ;
// interface with TRF7970A reader chip.                         ;
//                                                              ;
// PORT1.0 - HEART BEAT LED                                     ;
// PORT1.1 - UART RX                                            ;
// PORT1.2 - UART TX                                            ;
// PORT1.5 - SPI DATACLK                                        ;
// PORT1.6 - SPI MISO (REMOVE LED2 JUMPER)                      ;
// PORT1.7 - SPI MOSI                                           ;
//                                                              ;
// PORT2.0 - IRQ (INTERUPT REQUEST from TRF7970A)               ;
// PORT2.1 - SLAVE SELECT                                       ;
// PORT2.2 - TRF7970A ENABLE                                    ;
// PORT2.3 - ISO14443B LED                                      ;
// PORT2.4 - ISO14443A LED                                      ;
// PORT2.5 - ISO15693  LED                                      ;
//===============================================================

/********** HEADER FILES **********/
//===============================================================
#include <msp430.h>                         // processor specific header
#include <stdlib.h>                         // general purpose standard library
#include <stdio.h>                          // standard input/output header
#include "iso14443a.h"
#include "iso14443b.h"
#include "iso15693.h"
#include "mcu.h"
#include "Trf7970.h"
#include "types.h"
#include "uart.h"
//===============================================================

/********** GLOBAL VARIABLES **********/
//===============================================================
u08_t buf[100];					// TX/RX BUFFER FOR TRF7970A
u08_t enable = 0;
u08_t Tag_Count;
u08_t i_reg = 0x01;             // INTERRUPT REGISTER
u08_t irq_flag = 0x00;
u08_t rx_error_flag = 0x00;
s08_t rxtx_state = 1;           // USED FOR TRANSMIT RECEIVE BYTE COUNT
u08_t host_control_flag = 0;
u08_t stand_alone_flag = 1;
//===============================================================

void main(void)
{
	// WDT ~350ms, ACLK=1.5kHz, interval timer
	WDTCTL = WDT_ADLY_16;

	// Enable WDT interrupt
	IE1 |= WDTIE;

	SLAVE_SELECT_PORT_SET;
	SLAVE_SELECT_HIGH;

	ENABLE_PORT_SET;
	ENABLE_TRF;

	// wait until TRF7970A system clock started
	McuDelayMillisecond(2);

	// settings for communication with TRF7970A
	Trf7970CommunicationSetup();

	// Set Clock Frequency and Modulation
	Trf7970InitialSettings();

	// set the DCO to 8 MHz
	McuOscSel(1);

	// Re-configure the USART with this external clock
	Trf7970ReConfig();

	// Configure UART
	UartSetup();

	McuDelayMillisecond(5);
//	UartSendCString("Reader enabled.");
//	UartPutCrlf();
//	McuDelayMillisecond(2);

	// General enable interrupts
	__bis_SR_register(GIE);

	// indicates that setting are done
	enable = 1;

	// stand alone mode
	stand_alone_flag = 1;

	// launchpad LED1
	P1DIR |= BIT0;

	while(1)
	{
		Tag_Count = 0;
		IRQ_OFF;
		DISABLE_TRF;

		// Enter LPM3
		__bis_SR_register(LPM3_bits);

		// launchpad LED1 - Toggle (heartbeat)
		P1OUT ^= BIT0;

		// Clear IRQ Flags before enabling TRF7970A
		IRQ_CLR;
		IRQ_ON;

		ENABLE_TRF;

		// Must wait at least 4.8 ms to allow TRF7970A to initialize.
		__delay_cycles(40000);

		#ifdef ENABLE15693
			 Iso15693FindTag();		// Scan for 15693 tags
		#endif

		#ifdef ENABLE14443A
			  Iso14443aFindTag();	// Scan for 14443A tags
		#endif

//		#ifdef ENABLE14443B
//			  Iso14443bFindTag();	// Scan for 14443B tags
//		#endif

		// Write total number of tags read to UART
		if(Tag_Count > 0){
//			Tag_Count = UartNibble2Ascii(Tag_Count & 0x0F);	// convert to ASCII
//			UartSendCString("Tags Found: ");
//			UartPutChar(Tag_Count);
//			UartPutCrlf();
//			UartPutCrlf();
			McuDelayMillisecond(1000);
		}

	}
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
   //exit LPM3
   __bic_SR_register_on_exit(LPM3_bits);
}

/********** IT'S A TRAP!!!! (ISR'S) **********/
//===============================================================
#pragma vector= PORT1_VECTOR
__interrupt void PORT1_ISR (void)
{
	while(1);
}

#pragma vector= ADC10_VECTOR
__interrupt void ADC12_ISR (void)
{
	while(1);
}

#pragma vector= USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR (void)
{
	while(1);
}

#pragma vector= USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR (void)
{
	while(1);
}

#pragma vector= TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
{
	while(1);
}

#pragma vector= COMPARATORA_VECTOR
__interrupt void COMPARATORA_ISR (void)
{
	while(1);
}

#pragma vector= TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR (void)
{
	while(1);
}

#pragma vector= TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{
	while(1);
}

#pragma vector= NMI_VECTOR
__interrupt void NMI_ISR (void)
{
	while(1);
}
//===============================================================


