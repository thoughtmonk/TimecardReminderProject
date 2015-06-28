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

#include "neopixel.h"

// This struct is more or less unused in this implementation. 
// If I were to write another library for another platform though, 
// which allowed control of several strips, I would use this
// format. It allows control of different strips using a struct. 
// Basically, using OOP in C.

typedef struct RGB_LED_struct{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
}RGB_LED;

RGB_LED ledStrip[NUM_PIXELS];

void neopixel_initialize()
{
  P1DIR |= 0x80;

  P1SEL |= 0x80;
  P1SEL2 |= 0x80;

  UCB0CTL0 |= UCCKPH + UCMST + UCSYNC + UCMSB;
  UCB0CTL1 |= UCSSEL_2;

  UCB0BR0 = 3;
  UCB0BR1 = 0;

  UCB0CTL1 &= ~UCSWRST;
}

void neopixel_setRGB(int pixelAddr, uint8_t r, uint8_t g, uint8_t b)
{
  if(pixelAddr >= NUM_PIXELS)
    return;

  ledStrip[pixelAddr].red = r;
  ledStrip[pixelAddr].blue = b;
  ledStrip[pixelAddr].green = g;

}

void neopixel_setRed(int pixelAddr, uint8_t r)
{
  ledStrip[pixelAddr].red = r;
}

void neopixel_setGreen(int pixelAddr, uint8_t g)
{
  ledStrip[pixelAddr].green = g;
}

void neopixel_setBlue(int pixelAddr, uint8_t b)
{
  ledStrip[pixelAddr].blue = b;
}

void neopixel_setStrip( uint8_t r, uint8_t g, uint8_t b)
{
  int i;

  for(i = 0; i < NUM_PIXELS; i++)
    neopixel_setRGB( i, r, g, b);
}

void neopixel_draw()
{
  int i, j;

  //uint8_t packet[3];

  __bic_SR_register(GIE);

  for(i = 0; i < NUM_PIXELS; i++)
  {
    uint8_t packet[3] = {ledStrip[i].green,ledStrip[i].red, ledStrip[i].blue};
    for(j = 0; j < 3; j++)
    {
      uint8_t mask = 0x80;

      while(mask != 0)
      {
        while(!(IFG2 & UCB0TXIFG));

        if (packet[j] & mask)
          UCB0TXBUF = 0xF0;
        else
          UCB0TXBUF = 0xC0;

        mask >>= 1;
      }
    }
  }
  __delay_cycles(800);

  __bis_SR_register(GIE);
}


uint8_t neopixel_getRGB(int pixelAddr, uint8_t color)
{
  if(color == 0)
    return ledStrip[pixelAddr].red;
  else if(color == 1)
    return ledStrip[pixelAddr].green;
  else if(color == 2)
    return ledStrip[pixelAddr].blue;
  return 0;
}
