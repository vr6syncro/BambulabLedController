#ifndef config_h
#define config_h
#include <Arduino.h>

// Extra LED Debug Output to Console
#define Debug_LED_Console true

// LED overall Brightness (0-255)
#define LED_Brightness 64

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN D2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 3

#endif
