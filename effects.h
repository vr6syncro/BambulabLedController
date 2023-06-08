#ifndef effects_h
#define effects_h
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

/* Traffic Light Effect (first led green, secon yellow, thrid red)

  ampel_effect();

*/

void ampel_effect()
{
  strip.setPixelColor(0, 0, 255, 0);
  strip.setPixelColor(1, 255, 234, 0);
  strip.setPixelColor(2, 255, 0, 0);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Effect -> Ampel"));
  }
  else
  {
  }
}

/* color wipe

  colorWipe(0x00, 0xff, 0x00, 50);
  colorWipe(0x00, 0x00, 0x00, 50);

*/

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, red, green, blue);
    strip.show();
    delay(SpeedDelay);
  }
}

/*


  Set all LED to color

  setAll(0xff, 0x00, 0x00); // red
  setAll(0xff, 0xff, 0xff); // white
  setAll(0x00, 0x00, 0xff); // blue
  setAll(0xEE, 0xFF, 0x00); // yellow
  setAll(0xFF, 0xAA, 0x00); // orange
  setAll(0x09, 0xFF, 0x00); // green


*/

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LED_COUNT; i++ ) {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}


/* color loop

  RGBLoop();

*/

void RGBLoop() {
  for (int j = 0; j < 3; j++ ) {
    // Fade IN
    for (int k = 0; k < LED_Brightness; k++) {
      switch (j) {
        case 0: setAll(k, 0, 0); break;
        case 1: setAll(0, k, 0); break;
        case 2: setAll(0, 0, k); break;
      }
      strip.show();
      delay(15);
    }
    // Fade OUT
    for (int k = LED_Brightness; k >= 0; k--) {
      switch (j) {
        case 0: setAll(k, 0, 0); break;
        case 1: setAll(0, k, 0); break;
        case 2: setAll(0, 0, k); break;
      }
      strip.show();
      delay(15);
    }
  }
}

/*

   Fade IN/Out

  FadeInOut(0xff, 0x00, 0x00); // red
  FadeInOut(0xff, 0xff, 0xff); // white
  FadeInOut(0x00, 0x00, 0xff); // blue
  FadeInOut(0xEE, 0xFF, 0x00); // yellow
  FadeInOut(0xFF, 0xAA, 0x00); // orange
  FadeInOut(0x09, 0xFF, 0x00); // green

*/

void FadeInOut(byte red, byte green, byte blue) {
  float r, g, b;

  for (int k = 0; k < LED_Brightness; k = k + 1) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    strip.show();
    delay(15);
  }

  for (int k = LED_Brightness; k >= 0; k = k - 2) {
    r = (k / 256.0) * red;
    g = (k / 256.0) * green;
    b = (k / 256.0) * blue;
    setAll(r, g, b);
    strip.show();
    delay(15);
  }
}

/* Single LED Blink. Change the 0 to the LED that you want to activate. ! 0=LED1, 1=LED2... also you can change the time 1000.

  Blinkred(0, 1000);
  Blinkblue(0, 1000);
  Blinkyellow(0, 1000);

*/

// use: Blinkred(0, 1000);
// change 0 to Led number that should be used in the stripe
// change 1000 to the delay Time the Blink should have

//Blinkred
void Blinkred(int wich_led, int blink_delay)
{
  strip.setPixelColor(wich_led, 255, 0, 0);
  strip.show();
  delay(blink_delay);
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Blink -> Red On"));
  }
  else
  {
  }
  strip.setPixelColor(wich_led, 0, 0, 0);
  strip.show();
  delay(blink_delay);
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Blink -> Red Off"));
  }
  else
  {
  }
}
void Blinkyellow(int wich_led, int blink_delay)
{
  strip.setPixelColor(wich_led, 255, 234, 0);
  strip.show();
  delay(blink_delay);
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Blink -> Yellow On"));
  }
  else
  {
  }
  strip.setPixelColor(wich_led, 0, 0, 0);
  strip.show();
  delay(blink_delay);
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Blink -> Yellow Off"));
  }
  else
  {
  }
}

void Blinkblue(int wich_led, int blink_delay)
{
  strip.setPixelColor(wich_led, 0, 0, 255);
  strip.show();
  delay(blink_delay);
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Blink -> Blue On"));
  }
  else
  {
  }
  strip.setPixelColor(wich_led, 0, 0, 0);
  strip.show();
  delay(blink_delay);
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Blink -> Blue Off"));
  }
  else
  {
  }
}

/* single color, change the 0 to the led you want to activate. ! 0=LED1, 1=LED2 ....

  red(0);
  green(0);
  blue(0);
  white(0);
  yellow(0);
  orange(0);

*/

void red(int wich_led)
{
  strip.setPixelColor(wich_led, 255, 0, 0);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Red"));
  }
  else
  {
  }
}

void green(int wich_led)
{
  strip.setPixelColor(wich_led, 0, 255, 0);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Green"));
  }
  else
  {
  }
}

void blue(int wich_led)
{
  strip.setPixelColor(wich_led, 0, 0, 255);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Blue"));
  }
  else
  {
  }
}

void white(int wich_led)
{
  strip.setPixelColor(wich_led, 255, 255, 255);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> White"));
  }
  else
  {
  }
}

void yellow(int wich_led)
{
  strip.setPixelColor(wich_led, 255, 234, 0);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Yellow"));
  }
  else
  {
  }
}

void orange(int wich_led)
{
  strip.setPixelColor(wich_led, 251, 129, 0);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> Orange"));
  }
  else
  {
  }
}

/* Turn off all LEDS

  void Led_off();

*/

void Led_off()
{
  strip.clear();
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println(F("Neopixel -> OFF"));
  }
  else
  {
  }
}


#endif
