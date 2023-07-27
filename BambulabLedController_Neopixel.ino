#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <cstring>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "eeprom_utils.h"
#include "variables.h"
#include "html.h"
#include <Adafruit_NeoPixel.h>
#include "handle.h"
#include "effects.h"

bool rawdata = false;
int sequenceId = 0;

const char* wifiname = "Bambulab Led Controller Neopixel";
const char* setuppage = html_setuppage;
const char* finishedpage = html_finishpage;

char Printerip[Max_ipLength + 1] = "";
char Printercode[Max_accessCode + 1] = "";
char PrinterID[Max_DeviceId + 1] = "";
char EspPassword[Max_EspPassword + 1] = "";
char DeviceName[20];

ESP8266WebServer server(80);
IPAddress apIP(192, 168, 1, 1);

WiFiClientSecure WiFiClient;
WiFiManager wifiManager;
PubSubClient mqttClient(WiFiClient);

char* generateRandomString(int length) {
  static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  int charsetLength = strlen(charset);

  char* randomString = new char[length + 1];
  for (int i = 0; i < length; i++) {
    int randomIndex = random(0, charsetLength);
    randomString[i] = charset[randomIndex];
  }
  randomString[length] = '\0';

  return randomString;
}


void handleTouchInput() {
  static unsigned long lastTouchTime = 0;
  unsigned long currentMillis = millis();

  // Check if enough time has passed to avoid multiple inputs due to debouncing
  if (currentMillis - lastTouchTime >= 500) {
    if (digitalRead(touchPin1) == HIGH) {
      // Touch sensor is triggered, toggle the chamber light
      if (ledstate) {
        handleCommand("chamber_light=off");
        Serial.println("Touch sensor is triggered, turning chamber light off");
      } else {
        handleCommand("chamber_light=on");
        Serial.println("Touch sensor is triggered, turning chamber light on");
      }

      lastTouchTime = currentMillis; // Update last touch time to avoid rapid toggling
    }
  }
}

void publishMessage(const String& topic, const String& message) {
  mqttClient.publish(topic.c_str(), message.c_str());
  Serial.print("Publishing to topic: ");
  Serial.println(topic);
  Serial.print("JSON message: ");
  Serial.println(message);
}

void handleCommand(const String& command) {
  // Increment the sequence_id
  sequenceId++;

  // Construct the JSON message based on the command
  String message;
  if (command == "print=stop") {
    message = "{\"print\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"stop\",\"param\":\"\"}}";
  } else if (command == "print=pause") {
    message = "{\"print\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"pause\",\"param\":\"\"}}";
  } else if (command == "print=resume") {
    message = "{\"print\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"resume\",\"param\":\"\"}}";
  } else if (command.startsWith("set_chamber_fan_speed=")) {
    String fanSpeedStr = command.substring(21);
    int fanSpeed = fanSpeedStr.toInt();

    // Adjust fan speed value if needed (0 to 255)
    if (fanSpeed < 0) fanSpeed = 0;
    if (fanSpeed > 255) fanSpeed = 255;

    message = "{\"print\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"gcode_line\",\"param\":\"M106 P3 S" + String(fanSpeed) + "\\n\"}}";
  } else if (command.startsWith("set_part_cooling_fan_speed=")) {
    String fanSpeedStr = command.substring(27);
    int fanSpeed = fanSpeedStr.toInt();

    // Adjust fan speed value if needed (0 to 255)
    if (fanSpeed < 0) fanSpeed = 0;
    if (fanSpeed > 255) fanSpeed = 255;

    message = "{\"print\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"gcode_line\",\"param\":\"M106 P1 S" + String(fanSpeed) + "\\n\"}}";
  } else if (command.startsWith("set_aux_fan_speed=")) {
    String fanSpeedStr = command.substring(18);
    int fanSpeed = fanSpeedStr.toInt();

    // Adjust fan speed value if needed (0 to 255)
    if (fanSpeed < 0) fanSpeed = 0;
    if (fanSpeed > 255) fanSpeed = 255;

    message = "{\"print\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"gcode_line\",\"param\":\"M106 P2 S" + String(fanSpeed) + "\\n\"}}";
  } else if (command == "chamber_light=on" || command == "chamber_light=off") {
    // Toggle the chamber light state (ON to OFF or OFF to ON)
    ledstate = (command == "chamber_light=on");

    // Construct the message based on the ledstate
    if (ledstate) {
      message = "{\"system\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"ledctrl\",\"led_node\":\"chamber_light\",\"led_mode\":\"on\",\"led_on_time\":500,\"led_off_time\":500,\"loop_times\":1,\"interval_time\":1000}}";
    } else {
      message = "{\"system\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"ledctrl\",\"led_node\":\"chamber_light\",\"led_mode\":\"off\",\"led_on_time\":500,\"led_off_time\":500,\"loop_times\":1,\"interval_time\":1000}}";
    }

    // Publish the message
    publishMessage("device/" + String(PrinterID) + "/request", message);

    // Output to the serial monitor
    Serial.print("Chamber light is ");
    Serial.println(ledstate ? "ON" : "OFF");
  } else {
    // Invalid command
    Serial.println(F("Invalid command."));
    return;
  }
}

/* Some Debugging Stuff
  change Neopixel Brightness with brightness=xxx (0 - 255)
  change currentstage with currentstage=x (-1 - 21), will be overwritten on next mqtt loop!
  turn on off the rawdata with rawdata=true/false
*/

void handleSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Check if the input is empty
    if (input.isEmpty()) {
      Serial.println(F("Empty command. Please enter a valid command."));
      return;
    }

    // Find the position of the separator character '=' or space
    int separatorPos = input.indexOf('=');
    if (separatorPos == -1) {
      separatorPos = input.indexOf(' ');
    }

    // Check if the separator character was found
    if (separatorPos == -1) {
      Serial.println(F("Invalid command format."));
      return;
    }

    // Extract the command and value parts
    String command = input.substring(0, separatorPos);
    String value = input.substring(separatorPos + 1);

    // Trim leading and trailing spaces from the value
    value.trim();

    // Process the command
    if (command == "brightness") {
      int brightness = value.toInt();

      // Validate brightness value
      if (brightness < 0 || brightness > 255) {
        Serial.println(F("Invalid brightness value. Please enter a number between 0 and 255."));
        return;
      }

      strip.setBrightness(brightness);
      strip.show();

      Serial.print(F("LED brightness set to "));
      Serial.println(brightness);
    }
    else if (command == "currentstage") {
      int stage = value.toInt();

      // Validate stage value
      if (stage < 0) {
        Serial.println(F("Invalid stage value. Please enter a non-negative number."));
        return;
      }

      CurrentStage = stage;

      Serial.print(F("Current stage set to "));
      Serial.println(CurrentStage);
    } else if (command == "rawdata=true") {
      rawdata = true;
      Serial.println(F("JSON data output is enabled."));
    } else if (command == "rawdata=false") {
      rawdata = false;
      Serial.println(F("JSON data output is disabled."));
    } else if (command == "chamber_light") {
      if (value == "on") {
        publishMessage("device/" + String(PrinterID) + "/request", "{\"system\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"ledctrl\",\"led_node\":\"chamber_light\",\"led_mode\":\"on\",\"led_on_time\":500,\"led_off_time\":500,\"loop_times\":1,\"interval_time\":1000}}");
      } else if (value == "off") {
        publishMessage("device/" + String(PrinterID) + "/request", "{\"system\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"ledctrl\",\"led_node\":\"chamber_light\",\"led_mode\":\"off\",\"led_on_time\":500,\"led_off_time\":500,\"loop_times\":1,\"interval_time\":1000}}");
      } else {
        Serial.println(F("Invalid chamber_light command. Please use 'chamber_light=on' or 'chamber_light=off'."));
      }
    } else if (command == "print" || command == "chamber_light") {
      // Handle the commands for print and chamber_light
      handleCommand(input);
    }
    else if (command == "set_chamber_fan_speed" || command == "set_part_cooling_fan_speed" || command == "set_aux_fan_speed") {
      int fanSpeed = value.toInt();

      // Validate fan speed value
      if (fanSpeed < 0 || fanSpeed > 255) {
        Serial.println(F("Invalid fan speed value. Please enter a number between 0 and 255."));
        return;
      }

      String fanIndex;
      if (command == "set_part_cooling_fan_speed") {
        fanIndex = "1";
      } else if (command == "set_aux_fan_speed") {
        fanIndex = "2";
      } else { // set_chamber_fan_speed
        fanIndex = "3";
      }

      String message = "{\"print\":{\"sequence_id\":\"" + String(sequenceId) + "\",\"command\":\"gcode_line\",\"param\":\"M106 P" + fanIndex + " S" + String(fanSpeed) + "\\n\"}}";
      publishMessage("device/" + String(PrinterID) + "/request", message);
    } else {
      Serial.println(F("Invalid command."));
    }
  }
}



void replaceSubstring(char* string, const char* substring, const char* newSubstring) {
  char* substringStart = strstr(string, substring);
  if (substringStart) {
    char* substringEnd = substringStart + strlen(substring);
    memmove(substringStart + strlen(newSubstring), substringEnd, strlen(substringEnd) + 1);
    memcpy(substringStart, newSubstring, strlen(newSubstring));
  }
}

/*
  void handleSetTemperature() {
  if (!server.hasArg("api_key")) {
    return server.send(400, "text/plain", "Missing API key parameter.");
  };
  char shortened_key[7];
  strncpy(shortened_key, EspPassword, 7);
  shortened_key[7] = '\0';
  char received_api_key[8];

  server.arg("api_key").toCharArray(received_api_key, 8);
  if (!strcmp(received_api_key, shortened_key) == 0) {
    return server.send(401, "text/plain", "Unauthorized access.");
  }
  char mqttTopic[50];
  strcpy(mqttTopic, "device/");
  strcat(mqttTopic, PrinterID);
  strcat(mqttTopic, "/request");
  if (server.hasArg("bedtemp")) {
    float bedtemp = server.arg("bedtemp").toFloat();
    String message = "{\"print\":{\"sequence_id\":\"2026\",\"command\":\"gcode_line\",\"param\":\"M140 S" + String(bedtemp) + "\\n\"}}";
    mqttClient.publish(mqttTopic, message.c_str());
  }
  if (server.hasArg("nozzletemp")) {
    float bedtemp = server.arg("nozzletemp").toFloat();
    String message = "{\"print\":{\"sequence_id\":\"2026\",\"command\":\"gcode_line\",\"param\":\"M104 S" + String(bedtemp) + "\\n\"}}";
    mqttClient.publish(mqttTopic, message.c_str());
  }
  return server.send(200, "text/plain", "Ok");
  }
*/

void handleSetupRoot() { //Function to handle the setuppage
  if (!server.authenticate("BLLC", EspPassword)) {
    return server.requestAuthentication();
  }
  replaceSubstring((char*)setuppage, "ipinputvalue", Printerip);
  replaceSubstring((char*)setuppage, "idinputvalue", PrinterID);
  replaceSubstring((char*)setuppage, "codeinputvalue", Printercode);
  server.send(200, "text/html", setuppage);
}

void SetupWebpage() { //Function to start webpage system
  Serial.println(F("Starting Web server"));
  server.on("/", handleSetupRoot);
  server.on("/setupmqtt", savemqttdata);
  // server.on("/settemp", handleSetTemperature);
  server.begin();
  Serial.println(F("Web server started"));
}

void savemqttdata() {
  char iparg[Max_ipLength + 1];
  char codearg[Max_accessCode + 1];
  char idarg[Max_DeviceId + 1];

  // Copy the arguments from server to char arrays
  server.arg("ip").toCharArray(iparg, Max_ipLength + 1);
  server.arg("code").toCharArray(codearg, Max_accessCode + 1);
  server.arg("id").toCharArray(idarg, Max_DeviceId + 1);

  if (strlen(iparg) == 0 || strlen(codearg) == 0 || strlen(idarg) == 0) {
    return handleSetupRoot();
  }

  server.send(200, "text/html", finishedpage);

  Serial.println(F("Printer IP:"));
  Serial.println(iparg);
  Serial.println(F("Printer Code:"));
  Serial.println(codearg);
  Serial.println(F("Printer Id:"));
  Serial.println(idarg);

  writeToEEPROM(iparg, codearg, idarg, EspPassword);
  delay(1000); //wait for page to load
  ESP.restart();
}


void PrinterCallback(char* topic, byte * payload, unsigned int length) { //Function to handle the MQTT Data from the mqtt broker
  Serial.print(F("Message arrived in topic: "));
  Serial.println(topic);
  Serial.print(F("Message Length: "));
  Serial.println(length);

  StaticJsonDocument<10000> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  if (!doc.containsKey("print")) {
    return;
  }

  if (rawdata) {
    Serial.println(F("===== JSON Data Receiving ====="));
    serializeJsonPretty(doc, Serial);
    Serial.println(F("======================"));
  }

  if (doc["print"].containsKey("stg_cur")) {
    CurrentStage = doc["print"]["stg_cur"];
  }

  Serial.print(F("stg_cur: "));
  Serial.println(CurrentStage);

  if (doc["print"].containsKey("gcode_state")) {
    if (doc["print"]["gcode_state"] == "FINISH" && finishstartms <= 0) {
      finishstartms = millis();
    } else if (doc["print"]["gcode_state"] != "FINISH" && finishstartms > 0) {
      finishstartms = 0;
    }
  }

  if (doc["print"].containsKey("hms")) {
    hasHMSerror = false;
    for (const auto& hms : doc["print"]["hms"].as<JsonArray>()) {
      if (hms["code"] == 131073) {
        hasHMSerror = true;
      };
    }
  }

  Serial.print(F("HMS error: "));
  Serial.println(hasHMSerror);

  if (doc["print"].containsKey("lights_report")) {
    if (doc["print"]["lights_report"][0]["node"] == "chamber_light") {
      ledstate = doc["print"]["lights_report"][0]["mode"] == "on";
      Serial.print("Ledchanged: ");
      Serial.println(ledstate);
    }
  }

  Serial.print(F("cur_led: "));
  Serial.println(ledstate);


  Serial.println(F(" - - - - - - - - - - - -"));

  handleLed();
}

void setup() { // Setup function
  Serial.begin(115200);
  EEPROM.begin(512);

  //clearEEPROM();

  //  Neopixel initialize
  strip.begin();
  strip.clear();
  strip.setBrightness(LED_Brightness);
  strip.show();

  WiFiClient.setInsecure();
  mqttClient.setBufferSize(10000);

  if (wifiManager.getWiFiIsSaved()) wifiManager.setEnableConfigPortal(false);
  wifiManager.autoConnect(wifiname);

  WiFi.hostname("bambuledcontroller");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Failed to connect to WiFi, creating access point..."));
    wifiManager.setAPCallback([](WiFiManager * mgr) {
      Serial.println(F("Access point created, connect to:"));
      Serial.print(mgr->getConfigPortalSSID());
    });
    wifiManager.setConfigPortalTimeout(300);
    wifiManager.startConfigPortal(wifiname);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.println(F("Connecting to Wi-Fi..."));
  }

  readFromEEPROM(Printerip, Printercode, PrinterID, EspPassword);

  if (strchr(EspPassword, '#') == NULL) { //Isue with eeprom giving �, so adding a # to check if the eeprom is empty or not
    Serial.println(F("No Password has been set, Resetting"));
    memset(EspPassword, 0, Max_EspPassword);
    memset(Printercode, '_', Max_accessCode);
    memset(PrinterID, '_', Max_DeviceId);
    memset(Printerip, '_', Max_ipLength);
    char* newEspPassword = generateRandomString(Max_EspPassword - 1);
    strcat(newEspPassword, "#");
    strcat(EspPassword, newEspPassword);
    writeToEEPROM(Printerip, Printercode, PrinterID, EspPassword);
    readFromEEPROM(Printerip, Printercode, PrinterID, EspPassword); //This will auto clear the eeprom
  };

  Serial.print(F("Connected to WiFi, IP address: "));
  Serial.println(WiFi.localIP());
  Serial.println(F("-------------------------------------"));
  Serial.print(F("Head over to http://"));
  Serial.println(WiFi.localIP());
  Serial.print(F("Login Details User: BLLC, Password: "));
  Serial.println(String(EspPassword));
  Serial.println(F(" To configure the mqtt settings."));
  Serial.println(F("-------------------------------------"));

  SetupWebpage();

  strcpy(DeviceName, "ESP8266MQTT");
  char* randomString = generateRandomString(4);
  strcat(DeviceName, randomString);

  mqttClient.setServer(Printerip, 8883);
  mqttClient.setCallback(PrinterCallback);
  pinMode(touchPin1, INPUT);
}

void loop() { //Loop function
  server.handleClient();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Connection lost! Reconnecting..."));
    //wifiManager.autoConnect(wifiname);
    //Serial.println(F("Connected to WiFi!"));
    ESP.restart();
  }
  if (WiFi.status() == WL_CONNECTED && strlen(Printerip) > 0 && (lastmqttconnectionattempt <= 0 || millis() - lastmqttconnectionattempt >= 10000)) {
    if (!mqttClient.connected()) {

      Serial.print(F("Connecting with device name:"));
      Serial.println(DeviceName);
      Serial.println(F("Connecting to mqtt"));

      if (mqttClient.connect(DeviceName, "bblp", Printercode)) {
        Serial.println(F("Connected to MQTT"));
        Led_off();
        char mqttTopic[50];
        strcpy(mqttTopic, "device/");
        strcat(mqttTopic, PrinterID);
        strcat(mqttTopic, "/report");
        Serial.println("Topic: ");
        Serial.println(mqttTopic);
        mqttClient.subscribe(mqttTopic);
        lastmqttconnectionattempt;
      } else {
        Led_off();
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 10 seconds");
        lastmqttconnectionattempt = millis();
      }
    }
  }
  //Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  mqttClient.loop();
  handleSerialInput(); // debug
  handleTouchInput();
}
