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
 *		This program uses the MSP430's serial port to recieve led position and color packets and translate them into
 *	an SPI transmission that can control Adafruit's neopixel strips. Each packet recieved should have a delimiter at
 *	the start and end of a packet (0xFF) and contain 4 bytes worth on information (pixel, red brightness, green
 *	brightness, and blue brightness).
 *
 *		The hardware used by this program is both of the Universal Serial ports on the MSP430G2553.
\**********************************************************************************************************************/

#include <msp430.h> 
#include <inttypes.h>
#include <stdio.h>

#include "common_types.h"
#include "board.h"
#include "neopixel.h"
#include "serial.h"
#include "ring_buffer.h"

#define RING_BUFFER_SIZE 		128
#define PACKET_START_LENGTH 	-1
#define PACKET_FULL_LENGTH 		4
#define PACKET_DELIMITER		0xFF

void initialize_cpu();

/*
 * main.c
 */
int main(void)
{
	ring_buffer_t uart_buffer;
	uint8_t rx_buffer[RING_BUFFER_SIZE];

	int16_t packet_bytes_recv;
	uint8_t packet[PACKET_FULL_LENGTH];

	packet_bytes_recv = PACKET_START_LENGTH;

	neopixel_initialize();
    initialize_cpu();
    rb_initialize(&uart_buffer, rx_buffer, RING_BUFFER_SIZE);
    uart_initialize(&uart_buffer);

    neopixel_setStrip(0,0,0);
    neopixel_draw();

    /*
     * No need for power savings, so we will just continually poll the ring buffer for new data. Later iterations
     * may need power savings, and if so,
     */
    while(1)
    {
    	uint8_t new_data;

		if(rb_read(&uart_buffer, &new_data) == RC_SUCCESS)
		{
			if(packet_bytes_recv == PACKET_START_LENGTH)
			{
				if(new_data != PACKET_DELIMITER)
					break;
				packet_bytes_recv++;
			}
			else if(packet_bytes_recv == PACKET_FULL_LENGTH)
			{
				packet_bytes_recv = PACKET_START_LENGTH;
				if(new_data != PACKET_DELIMITER)
				{
					break;
				}

				if(packet[0] == PACKET_DELIMITER)
				{
					neopixel_draw();
				}
				else
				{
					neopixel_setRGB(packet[0],packet[1],packet[2],packet[3]);
				}
			}
			else
			{
				packet[packet_bytes_recv] = new_data;
				packet_bytes_recv++;
			}
		}
    }
}

void initialize_cpu()
{
  // Turn off watchdog timer.
  WDTCTL = WDTPW + WDTHOLD;
  // Set CPU frequency to the calibrated 16 Mhz.
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;

}
