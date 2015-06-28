/**********************************************************************************************************************\
 * The MIT License (MIT)																							  *
 *																													  *
 * Copyright (c) <year> <copyright holders>																			  *
 *																													  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy										  *
 * of this software and associated documentation files (the "Software"), to deal									  *
 * in the Software without restriction, including without limitation the rights										  *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell										  *
 * copies of the Software, and to permit persons to whom the Software is											  *
 * furnished to do so, subject to the following conditions:															  *
 * 																													  *
 * The above copyright notice and this permission notice shall be included in										  *
 * all copies or substantial portions of the Software.																  *
 * 																													  *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR										  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,											  *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE										  *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER											  *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,									  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN										  *
 * THE SOFTWARE.																									  *
 * 																													  *
\**********************************************************************************************************************/

/**********************************************************************************************************************\
 *	Project: Remindr
 *	Author: Sean Wagoner
 *	Date: July 28th, 2015
 *
 *	Purpose:
 *
\**********************************************************************************************************************/


#include "serial.h"

/******************************************************************************
 *
 *****************************************************************************/
return_type_t uart_initialize(ring_buffer_t *ring_buffer)
{
	rx_uart_buffer = ring_buffer;

	/* Set UCSWRST UCAxCTL1. This turns off the UART. */
	UCA0CTL1 |= UCSWRST;

	/* Configure ports. */
	P1SEL |= BIT1|BIT2;
	P1SEL2 |= BIT1|BIT2;

	/* Initialize all USCI registers. */
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = 0x00FF & UCBR;
	UCA0BR1 = (0xFF00 & UCBR) >> 8;
	UCA0MCTL |= UCBRS1 | UCBRS2;
	UCA0CTL1 &= ~UCSWRST;
	IE2 |= UCA0RXIE;

	/* Clear UCSWRST, UCAxCTL1. This turns on the UART. */


	return RC_SUCCESS;
}

/******************************************************************************
 *
 *****************************************************************************/
return_type_t uart_send_byte(const uint8_t byte)
{
	while(!(IFG2 & UCA0TXIFG));
	UCA0TXBUF = byte;
	return RC_SUCCESS;
}

/******************************************************************************
 *
 *****************************************************************************/
return_type_t uart_send_array(const uint8_t *send_array, uint8_t size)
{
	uint8_t i;
	for(i = 0; i < size; i++)
		uart_send_byte(send_array[i]);
	return RC_SUCCESS;
}

/******************************************************************************
 *
 *****************************************************************************/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	uint8_t data;
	data = UCA0RXBUF;
	rb_write(rx_uart_buffer, data);
}

