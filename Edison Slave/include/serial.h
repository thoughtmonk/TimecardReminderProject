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

#ifndef SRC_SERIAL_H_
#define SRC_SERIAL_H_

#include <inttypes.h>

#include "ring_buffer.h"
#include "board.h"
#include "common_types.h"

static ring_buffer_t *rx_uart_buffer;

return_type_t uart_initialize(ring_buffer_t *ring_buffer);
return_type_t uart_send_byte(const uint8_t byte);
return_type_t uart_send_array(const uint8_t *send_array, uint8_t size);

__interrupt void USCI0RX_ISR(void);


#endif /* SRC_SERIAL_H_ */
