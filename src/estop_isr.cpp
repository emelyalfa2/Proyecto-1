// FASE 3 - ISR con debounce por CCOUNT y spinlock SMP
#include <Arduino.h>
#include <xtensa/core-macros.h>
#include "estop_isr.h"

namespace {
constexpr uint8_t  ESTOP_PIN = 33;
constexpr uint32_t DEBOUNCE_CYCLES = 4000000;       // 25 ms a 160 MHz

portMUX_TYPE      mux = portMUX_INITIALIZER_UNLOCKED;
volatile bool     estopFlag = false;
volatile uint32_t estopPulses = 0;
volatile uint32_t isrLastCycles = 0;
}  // namespace

// IRAM_ATTR: la ISR vive en RAM y no falla si la cache de flash esta deshabilitada.
// Prohibido aqui: delay(), Serial, mutex de FreeRTOS o cualquier llamada bloqueante.
static void IRAM_ATTR onEstopISR() {
  uint32_t now = XTHAL_GET_CCOUNT();
  // Resta sin signo de 32 bits: correcta aunque CCOUNT desborde (aritmetica mod 2^32)
  if ((uint32_t)(now - isrLastCycles) >= DEBOUNCE_CYCLES) {
    isrLastCycles = now;
    portENTER_CRITICAL_ISR(&mux);
    estopFlag = true;
    estopPulses = estopPulses + 1;
    portEXIT_CRITICAL_ISR(&mux);
  }
}

void estopInit() {
  pinMode(ESTOP_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ESTOP_PIN), onEstopISR, FALLING);
}

bool estopTakeFlag() {
  bool fired;
  portENTER_CRITICAL(&mux);
  fired = estopFlag;
  estopFlag = false;
  portEXIT_CRITICAL(&mux);
  return fired;
}

uint32_t estopCount() {
  portENTER_CRITICAL(&mux);
  uint32_t c = estopPulses;
  portEXIT_CRITICAL(&mux);
  return c;
}

bool estopReleased() {
  return digitalRead(ESTOP_PIN) == HIGH;
}