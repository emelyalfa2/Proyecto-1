#include <Arduino.h>
#include "hal_motor.h"

void setup() {
  Serial.begin(115200);
  motorInit();
}

void loop() {
}