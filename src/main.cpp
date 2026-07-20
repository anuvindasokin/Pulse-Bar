#include <Arduino.h>
#include "app/AppController.h"

AppController controller;

void setup() {
  Serial.begin(115200);
  controller.begin();
}

void loop() {
  controller.loop();
}
