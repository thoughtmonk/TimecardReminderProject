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

#include "ring_buffer.h"


return_type_t rb_initialize(ring_buffer_t *ring_buffer, uint8_t *buffer, uint16_t length)
{
	ring_buffer->buffer = buffer;
	ring_buffer->size = length;
	ring_buffer->read = 0;
	ring_buffer->write = 0;

	return RC_SUCCESS;
}


uint16_t rb_length(ring_buffer_t *ring_buffer)
{
	return (ring_buffer->write - ring_buffer->read) & (ring_buffer->size - 1);
}


return_type_t rb_write(ring_buffer_t *ring_buffer, uint8_t data)
{
	/* Check if there is enough space to add data to the buffer. */
	if(rb_length(ring_buffer) == (ring_buffer->size - 1))
		return RC_OUT_OF_BOUNDS;

	/* Write new data to the ring buffer. */
	ring_buffer->buffer[ring_buffer->write] = data;

	/* Update the ring buffer's write location. */
	ring_buffer->write = (ring_buffer->write + 1) & (ring_buffer->size - 1);

	return RC_SUCCESS;
}


return_type_t rb_read(ring_buffer_t *ring_buffer, uint8_t *data)
{
	if(rb_length(ring_buffer) == 0)
		return RC_NO_AVAILABLE_ACTION;

	/* Grab data from the buffer. */
	*data = ring_buffer->buffer[ring_buffer->read];

	/* Update the ring buffer's read location. */
	ring_buffer->read = (ring_buffer->read + 1) & (ring_buffer->size - 1);

	return RC_SUCCESS;
}

