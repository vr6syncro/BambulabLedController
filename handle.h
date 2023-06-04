#ifndef handle_h
#define handle_h
#include <Arduino.h>

int CurrentStage = -1;
bool hasHMSerror = false;
bool ledstate = false;
unsigned long finishstartms;
unsigned long lastmqttconnectionattempt;

// Bambu MQTT LED Handler

// -1: "Idle",
// 0: "Printing",
// 1: "Auto Bed Leveling",
// 2: "Heatbed Preheating",
// 3: "Sweeping XY Mech Mode",
// 4: "Changing Filament",
// 5: "M400 Pause",
// 6: "Paused due to filament runout",
// 7: "Heating Hotend",
// 8: "Calibrating Extrusion",
// 9: "Scanning Bed Surface",
// 10: "Inspecting First Layer",
// 11: "Identifying Build Plate Type",
// 12: "Calibrating Micro Lidar",
// 13: "Homing Toolhead",
// 14: "Cleaning Nozzle Tip",
// 15: "Checking Extruder Temperature",
// 16: "Printing was paused by the user",
// 17: "Pause of front cover falling",
// 18: "Calibrating Micro Lidar",
// 19: "Calibrating Extrusion Flow",
// 20: "Paused due to nozzle temperature malfunction",
// 21: "Paused due to heat bed temperature malfunction"
// hasHMSerror: HMS Error


void handleLed() {
  if (ledstate == 1) {
    if (CurrentStage == -1) {
      // Neopixel Led - see colour.h or ledblink.h
      ampel_effect();
      return;
    };
    if (CurrentStage == 0) {
      // Neopixel Led - see colour.h or ledblink.h
      green(0);
      green(1);
      green(2);
      return;
    };
    // Homing, Bed Level
    if (CurrentStage == 1 ||  CurrentStage == 9 || CurrentStage == 13 ||  CurrentStage == 10) {
      // Neopixel Led - see colour.h or ledblink.h
      blue(0);
      white(1);
      white(2);
      return;
    };
    // Preheating
    if (CurrentStage == 2 || CurrentStage == 7 || CurrentStage == 15) {
      // Neopixel Led - see colour.h or ledblink.h
      orange(0);
      white(1);
      white(2);
      return;
    };
    if (CurrentStage == 4) {
      // Neopixel Led - see colour.h or ledblink.h
      blue(1);
      return;
    };
    if (CurrentStage == 5) {
      // Neopixel Led - see colour.h or ledblink.h
      yellow(0);
      yellow(1);
      yellow(2);
      return;
    };
    if (CurrentStage == 6) {
      // Neopixel Led - see colour.h or ledblink.h
      red(0);
      yellow(1);
      red(2);
      return;
    };
    // Calibration
    if (CurrentStage == 8 || CurrentStage == 18 || CurrentStage == 19 || CurrentStage == 12 ||  CurrentStage == 14 ||  CurrentStage == 3) {
      // Neopixel Led - see colour.h or ledblink.h
      blue(0);
      blue(1);
      blue(2);
      return;
    };
    if (CurrentStage == 16) {
      // Neopixel Led - see colour.h or ledblink.h
      yellow(0);
      green(1);
      yellow(2);
      return;
    };
    // Temp runaway
    if (CurrentStage == 17 || CurrentStage == 20 || CurrentStage == 21) {
      // Neopixel Led - see colour.h or ledblink.h
      Blinkred(0, 1000);
      Blinkred(1, 1000);
      Blinkred(2, 1000);
      return;
    };
  } else {
    Led_off();
  };
}

#endif
