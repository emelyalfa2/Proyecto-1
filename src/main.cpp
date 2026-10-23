// Proyecto 1 - Firmware Controlador Banda Transportadora
// loop() sin delay(): captura eventos -> los despacha a la FSM
#include <Arduino.h>
#include "hal_motor.h"
#include "estop_isr.h"
#include "fsm_control.h"

namespace {
// Convierte las fuentes de entrada en un Event (no bloqueante)
Event readEvent() {
  if (estopTakeFlag()) {                       // bandera de la ISR (con spinlock)
    return Event::STOP;                        // el paro de emergencia tiene prioridad
  }
  if (Serial.available() > 0) {
    char c = static_cast<char>(Serial.read());
    if (c == 's' || c == 'S') {
      return Event::START;
    }
    if (c == 'r' || c == 'R') {
      if (estopReleased()) {
        return Event::RESET;
      }
      Serial.println("[AVISO] Suelte el paro de emergencia antes de hacer RESET");
    }
  }
  return Event::NONE;
}
}  // namespace

void setup() {
  Serial.begin(115200);
  motorInit();
  estopInit();
  fsmInit();
  Serial.println("Banda transportadora lista. Comandos: 's' = START, 'r' = RESET");
}

void loop() {
  Event ev = readEvent();
  if (ev == Event::NONE) {
    return;
  }

  State before = fsmGetState();
  fsmDispatch(ev);
  Serial.printf("[FSM] %s --%s--> %s | paros=%lu\n",
                fsmStateName(before), fsmEventName(ev), fsmStateName(fsmGetState()),
                static_cast<unsigned long>(estopCount()));
}