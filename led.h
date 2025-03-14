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

#if KYR_LED_OUT_PIN >= 0 && KYR_LED_NUM_LED > 0

#define LED_WS2812Serial 1
#define LED_NeoPixel 0

#if LED_WS2812Serial

#include <WS2812Serial.h>

static byte led_drawingMemory[KYR_LED_NUM_LED*3];         //  3 bytes per LED
static DMAMEM byte led_displayMemory[KYR_LED_NUM_LED*12]; // 12 bytes per LED

static WS2812Serial leds(KYR_LED_NUM_LED, led_displayMemory, led_drawingMemory, KYR_LED_OUT_PIN, WS2812_RGB);

#endif

#if LED_NeoPixel
#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel leds(KYR_LED_NUM_LED, KYR_LED_OUT_PIN, NEO_RGB + NEO_KHZ800);

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
#if LED_WS2812Serial
    leds.setPixel(i, color);
#endif
#if LED_NeoPixel
    leds.setPixelColor(i, color);
#endif
  }
  leds.show();
}

/*
 * This listens to the midi note bound to sidetone keying
 * and sets the leds value to correspond to the midi note value.
 */
static void led_note_listener(int note, int _) { 
  const int val = note_get(note);
  const int on_color = xnrpn_get(NRPN_XLED_ON_COLOR);
  const int off_color = xnrpn_get(NRPN_XLED_OFF_COLOR);
  led_setcolor( val ? on_color : off_color );
  // Serial.printf("led_note_listener(note=%d, _) led_setcolor( %d ? %d : %d)\n", note, val, on_color, off_color);
}

/*
 * This listens to the nrpn which assigns the LED pin
 * and sets the pin value to correspond to the midi note value.
 */
static void led_nrpn_listener(int nrpn, int _) {
  const int pin = nrpn_get(nrpn);
  // Serial.printf("led_nrpn_listener(nrpn=%d, _) -> pin=%d and note=%d\n", nrpn, pin, NOTE_KEY_ST);
  if (pin_valid(pin)) {
    note_listen(NOTE_KEY_ST, led_note_listener);
    led_setcolor(xnrpn_get(NRPN_XLED_OFF_COLOR));
  } else {
    note_unlisten(NOTE_KEY_ST, led_note_listener);
    led_setcolor(0);
  }
}

static void led_setup(void) {
  leds.begin();
  led_setcolor(0);
  nrpn_listen(NRPN_LED_ENABLE, led_nrpn_listener);
}

#else /* KYR_LED_OUT_PIN >= 0 && KYR_LED_NUM_LED > 0 */

static void led_setup(void) {}

#endif /* KYR_LED_OUT_PIN >= 0 && KYR_LED_NUM_LED > 0 */
