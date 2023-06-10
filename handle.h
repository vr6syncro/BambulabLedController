#ifndef handle_h
#define handle_h
#include <Arduino.h>
#include "effects.h"

int CurrentStage = -1;
bool hasHMSerror = false;
bool ledstate = false;
unsigned long finishstartms;
unsigned long lastmqttconnectionattempt;


/* Bambulab currentstage description

  -1: "Idle",
  0: "Printing",
  1: "Auto Bed Leveling",
  2: "Heatbed Preheating",
  3: "Sweeping XY Mech Mode",
  4: "Changing Filament",
  5: "M400 Pause",
  6: "Paused due to filament runout",
  7: "Heating Hotend",
  8: "Calibrating Extrusion",
  9: "Scanning Bed Surface",
  10: "Inspecting First Layer",
  11: "Identifying Build Plate Type",
  12: "Calibrating Micro Lidar",
  13: "Homing Toolhead",
  14: "Cleaning Nozzle Tip",
  15: "Checking Extruder Temperature",
  16: "Printing was paused by the user",
  17: "Pause of front cover falling",
  18: "Calibrating Micro Lidar",
  19: "Calibrating Extrusion Flow",
  20: "Paused due to nozzle temperature malfunction",
  21: "Paused due to heat bed temperature malfunction"
  hasHMSerror: HMS Error

*/

void handleLed() {
  if (ledstate == 1) {
    if (CurrentStage == -1) {
      // ↓ Printer Idle
      FadeInOut(0x09, 0xFF, 0x00);
      return;
    };
    if (CurrentStage == 0) {
      // ↓ Printer Printing
      setAll(0x09, 0xFF, 0x00);
      return;
    };
    if (CurrentStage == 1 ||  CurrentStage == 9 || CurrentStage == 13 ||  CurrentStage == 10) {
      // ↓ Printer Leveling
      setAll(0xff, 0xff, 0xff); // white
      blue(0);
      blue(3);
      return;
    };
    // Preheating
    if (CurrentStage == 2 || CurrentStage == 7 || CurrentStage == 15) {
      // ↓ Printer Heating
      setAll(0xff, 0xff, 0xff); // white
      orange(0);
      orange(3);
      return;
    };
    if (CurrentStage == 4) {
      // ↓ Printer Filament Change
      setAll(0xEE, 0xFF, 0x00); // yellow
      return;
    };
    if (CurrentStage == 5) {
      // ↓ Printer M400 Pause
      setAll(0xFF, 0xAA, 0x00); // orange
      return;
    };
    if (CurrentStage == 6) {
      // ↓ Printer Filament Runout
      setAll(0xff, 0x00, 0x00); // red
      return;
    };
    // Calibration
    if (CurrentStage == 8 || CurrentStage == 18 || CurrentStage == 19 || CurrentStage == 12 ||  CurrentStage == 14 ||  CurrentStage == 3) {
      // ↓ Printer Calibrating
      setAll(0xff, 0xff, 0xff); // white
      blue(0);
      blue(3);
      return;
    };
    if (CurrentStage == 16) {
      // ↓ Printer Paused by User
      setAll(0xEE, 0xFF, 0x00); // yellow
      return;
    };
    if (CurrentStage == 17 || CurrentStage == 20 || CurrentStage == 21) {
      // ↓ Printer Error and Temp Runaway
      FadeInOut(0xff, 0x00, 0x00); // red
      return;
    };
  } else {
    Led_off();
  };
}

#endif
