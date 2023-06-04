#ifndef effects_h
#define effects_h
#include <Arduino.h>


void ampel_effect()
{
  strip.setPixelColor(0, 0, 255, 0);
  strip.setPixelColor(1, 255, 234, 0);
  strip.setPixelColor(2, 255, 0, 0);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println("Neopixel -> Effect -> Ampel");
  }
  else
  {
  }
}

















#endif
