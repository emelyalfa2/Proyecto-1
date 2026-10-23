#include <Arduino.h>
#include "hal_motor.h"
#include "estop_isr.h"

void setup() {
  Serial.begin(115200);
  motorInit();
  estopInit();
}

void loop() {
  if (estopTakeFlag()) {
    Serial.printf("[ESTOP] paros=%lu\n", static_cast<unsigned long>(estopCount()));
  }
}