/* -*- mode: c++; tab-width: 8 -*- */
/*
 * hasak (ham and swiss army knife) keyer for Teensy 4.X, 3.X
 * Copyright (c) 2021 by Roger Critchlow, Charlestown, MA, USA
 * ad5dz, rec@elf.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
** Used Stoffregen's non-blocking WS2812Serial library.
** But saw some artefacts on the HL2 spectrum display
** that correlated with powering Hasak.
** Switched to Adafruit_NeoPixel library.
** Still saw the birdies,
** Now think it might be the USB cable radiating.
*/

#define WS2812Serial 1
#define NeoPixel 0

#if WS2812Serial
#include <WS2812Serial.h>
#endif

#if NeoPixel
#include <Adafruit_NeoPixel.h>
#endif

static const int led_pin = KYR_LED_OUT_PIN;
static const int led_numled = 3;

#if WS2812Serial
static byte led_drawingMemory[led_numled*3];         //  3 bytes per LED
static DMAMEM byte led_displayMemory[led_numled*12]; // 12 bytes per LED

static WS2812Serial leds(led_numled, led_displayMemory, led_drawingMemory, led_pin, WS2812_RGB);
#endif

#if NeoPixel
static Adafruit_NeoPixel leds(led_numled, led_pin, NEO_RGB + NEO_KHZ800);
#endif

/*
// More intense...
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
*/
// Less intense...
#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

static void led_setcolor(int color) {
  for (int i=0; i < leds.numPixels(); i++) {
#if WS2812Serial
    leds.setPixel(i, color);
#endif
#if NeoPixel
    leds.setPixelColor(i, color);
#endif
  }
  leds.show();
}

static void led_setup(void) {
  leds.begin();
  led_setcolor(ORANGE);
}
