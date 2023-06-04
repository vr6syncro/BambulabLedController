#ifndef effects_h
#define effects_h
#include <Arduino.h>

// use: yellow_effect(1000);
// change 1000 to delay that effekt the blink

//Blinkyellow
void yellow_effect(int led_delay)
{
  strip.setPixelColor(0, 255, 234, 0);
  strip.show();
  delay(led_delay);
  strip.setPixelColor(1, 255, 234, 0);
  strip.show();
  delay(led_delay);
  strip.setPixelColor(2, 255, 234, 0);
  strip.show();
  delay(led_delay);
  if (Debug_LED_Console == true)
  {
    Serial.println("Neopixel -> Effect -> Yellow");
  }
  else
  {
  }
  strip.clear();
  strip.show();
}


















#endif
