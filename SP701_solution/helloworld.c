/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xtmrctr.h"
#include "xparameters.h"

#define PWM_PERIOD              5000000    /* PWM period in (500 ms) */
#define TMRCTR_0                0            /* Timer 0 ID */
#define TMRCTR_1                1            /* Timer 1 ID */
#define CYCLE_PER_DUTYCYCLE     10           /* Clock cycles per duty cycle */
#define MAX_DUTYCYCLE           100          /* Max duty cycle */
#define DUTYCYCLE_DIVISOR       4            /* Duty cycle Divisor */
#define WAIT_COUNT              PWM_PERIOD   /* Interrupt wait counter */
#define TMRCTR_DEVICE_ID        XPAR_TMRCTR_0_DEVICE_ID

XTmrCtr TimerCounterInst;

int main()
{
	u8  DutyCycle;
	u8  NoOfCycles;
	u8  Div;
	u8  Value;
	u32 Period;
	u32 HighTime;
	u64 WaitCount;
	int Status;
	float DivF;

    init_platform();

    print("Mastering MicroBlaze\n\r");


    XTmrCtr_Initialize(&TimerCounterInst, TMRCTR_DEVICE_ID);

    XTmrCtr_PwmDisable(&TimerCounterInst);

    Div = DUTYCYCLE_DIVISOR;
    Period = PWM_PERIOD;
    HighTime = PWM_PERIOD * (Div/10) ;

    DutyCycle = XTmrCtr_PwmConfigure(&TimerCounterInst, Period, HighTime);

	/* Enable PWM */
	XTmrCtr_PwmEnable(&TimerCounterInst);

	while(1){
		 print("Select a Brightness between 0 and 9\n\r");

		 /* Read an input value from the console. */
		 Value = inbyte();
		 Div = Value - 0x30;
		 DivF = (float) Div /10;
		 HighTime = PWM_PERIOD * DivF;

		 XTmrCtr_PwmDisable(&TimerCounterInst);
		 DutyCycle = XTmrCtr_PwmConfigure(&TimerCounterInst, Period, HighTime);
		 XTmrCtr_PwmEnable(&TimerCounterInst);
	}

    cleanup_platform();
    return 0;
}
