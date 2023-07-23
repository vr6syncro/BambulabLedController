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
  if (!ledstate) {
    Led_off();
    return;
  }

  if (hasHMSerror) {
    blinkAllWithColor("ORANGE", 100);
    return;
  }

  switch (CurrentStage) {
    case -1: // Printer Idle
      setAllColor("DARK_GREEN");
      break;

    case 0: // Printer Printing
      setAllColor("GREEN");
      break;

    case 1:
    case 9:
    case 13:
    case 10: // Printer Leveling
      setAllColor("BLUE");
      break;

    case 2:
    case 7:
    case 15: // Printer Heating
      setAllColor("ORANGE");
      break;

    case 4: // Printer Filament Change
      setAllColor("BLUE");
      break;

    case 5: // Printer M400 Pause
      setAllColor("YELLOW");
      break;

    case 6: // Printer Filament Runout
      setAllColor("RED");
      break;

    case 8:
    case 18:
    case 19:
    case 12:
    case 14:
    case 3: // Printer Calibrating
      setAllColor("BLUE");
      break;

    case 16: // Printer Paused by User
      setAllColor("YELLOW");
      break;

    case 17:
    case 20:
    case 21: // Printer Error and Temp Runaway
      blinkAllWithColor("RED", 100);
      break;

    default:
      Led_off();
      break;
  }
}

#endif
