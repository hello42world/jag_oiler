#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "app.h"


App* app = nullptr;

void setup() {
  // WiFi.mode(WIFI_OFF);

  // Initialize serial communication
  Serial.begin(115200); 

  app = new App();
}

void mui_loop(void);


void loop() {
  app->loop();
}
