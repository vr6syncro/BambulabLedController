#ifndef colour_h
#define colour_h
#include <Arduino.h>

// use: red(0);
// change 0 to Led number that should be used in the stripe

void red(int wich_led)
{
  strip.setPixelColor(wich_led, 255, 0, 0);
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println("Neopixel -> Red");
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
    Serial.println("Neopixel -> Green");
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
    Serial.println("Neopixel -> Blue");
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
    Serial.println("Neopixel -> White");
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
    Serial.println("Neopixel -> Yellow");
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
    Serial.println("Neopixel -> Orange");
  }
  else
  {
  }
}

void Led_off()
{
  strip.clear();
  strip.show();
  if (Debug_LED_Console == true)
  {
    Serial.println("Neopixel -> OFF");
  }
  else
  {
  }
}

#endif
