#ifndef effects_h
#define effects_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

/* Assuming you have initialized the NeoPixel strip before calling this
  setAllColor("RED"); // Sets all LEDs to red color
  setAllColor("BLUE"); // Sets all LEDs to blue color
  setAllColor("GREEN"); // Sets all LEDs to green color
  setAllColor("DARK_GREEN"); // Sets all LEDs to green color
  setAllColor("WHITE"); // Sets all LEDs to white color
  setAllColor("YELLOW"); // Sets all LEDs to yellow color
  setAllColor("ORANGE"); // Sets all LEDs to orange color
  setAll(0x00, 0x00, 0x00); // Individual color
  blinkAllWithColor("GREEN", 100); // color (see above) and interval
  setIndividualLED(0, RED);    // LED 0-5 (for 6 Leds) to red (or see above) color
*/

// Define LED color constants
const uint32_t COLOR_RED = strip.Color(255, 0, 0);
const uint32_t COLOR_GREEN = strip.Color(0, 255, 0);
const uint32_t COLOR_BLUE = strip.Color(0, 0, 255);
const uint32_t COLOR_WHITE = strip.Color(255, 255, 255);
const uint32_t COLOR_YELLOW = strip.Color(255, 234, 0);
const uint32_t COLOR_ORANGE = strip.Color(251, 129, 0);
const uint32_t COLOR_DARK_GREEN = strip.Color(0, 28, 0);

uint32_t getColorFromName(const String& colorName)
{
  if (colorName.equalsIgnoreCase("RED"))
    return COLOR_RED;
  else if (colorName.equalsIgnoreCase("GREEN"))
    return COLOR_GREEN;
  else if (colorName.equalsIgnoreCase("BLUE"))
    return COLOR_BLUE;
  else if (colorName.equalsIgnoreCase("WHITE"))
    return COLOR_WHITE;
  else if (colorName.equalsIgnoreCase("YELLOW"))
    return COLOR_YELLOW;
  else if (colorName.equalsIgnoreCase("ORANGE"))
    return COLOR_ORANGE;
  else if (colorName.equalsIgnoreCase("DARK_GREEN"))
    return COLOR_DARK_GREEN;
  else
    return 0;
}

void setAllColor(const String& colorName)
{
  uint32_t colorValue = getColorFromName(colorName);

  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, colorValue);
  }
  strip.show();
}

void setIndividualLED(int ledIndex, uint32_t colorValue)
{
  if (ledIndex >= 0 && ledIndex < LED_COUNT)
  {
    strip.setPixelColor(ledIndex, colorValue);
    strip.show();
  }
}

void Led_off()
{
  strip.clear();
  strip.show();
}

void blinkAllWithColor(const String& colorName, uint32_t interval)
{
  uint32_t colorValue = getColorFromName(colorName);

  // Get the current time
  static uint32_t previousMillis = 0;
  uint32_t currentMillis = millis();

  // Calculate the time elapsed since the last update
  uint32_t elapsedTime = currentMillis - previousMillis;

  // Check if it's time to update the LEDs
  if (elapsedTime >= interval)
  {
    // Save the current time for the next update
    previousMillis = currentMillis;

    // Check if the LEDs are currently off
    if (strip.getPixelColor(0) == 0)
    {
      // Turn on all LEDs with the specified color
      for (int i = 0; i < LED_COUNT; i++)
      {
        strip.setPixelColor(i, colorValue);
      }
      strip.show();
    }
    else
    {
      // Turn off all LEDs
      strip.clear();
      strip.show();
    }
  }
}

#endif
