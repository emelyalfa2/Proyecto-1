// FASE 2 - HAL del motor
#include <Arduino.h>
#include "hal_motor.h"

namespace {
constexpr uint8_t  MOTOR_A_PIN = 12;
constexpr uint8_t  MOTOR_B_PIN = 14;
constexpr uint32_t PWM_FREQ_HZ = 1000;
constexpr uint8_t  PWM_RES_BITS = 10;                       // 0..1023
constexpr uint8_t  CH_A = 0;
constexpr uint8_t  CH_B = 1;                                 // canales 0 y 1 comparten LEDC_TIMER_0
constexpr uint32_t DUTY_MAX = (1u << PWM_RES_BITS) - 1;      // 1023
}  // namespace

void motorInit() {
  // El timer queda definido por frecuencia + resolucion; luego se asignan pines
  ledcSetup(CH_A, PWM_FREQ_HZ, PWM_RES_BITS);
  ledcSetup(CH_B, PWM_FREQ_HZ, PWM_RES_BITS);
  ledcAttachPin(MOTOR_A_PIN, CH_A);
  ledcAttachPin(MOTOR_B_PIN, CH_B);
  motorStop();                                               // estado seguro al arrancar
}

void motorPWM(uint32_t cicloTrabajo) {
  if (cicloTrabajo > DUTY_MAX) {
    cicloTrabajo = DUTY_MAX;
  }
  ledcWrite(CH_A, cicloTrabajo);
  ledcWrite(CH_B, 0);
}

void motorBrake() {
  ledcWrite(CH_A, DUTY_MAX);
  ledcWrite(CH_B, DUTY_MAX);
}

void motorStop() {
  ledcWrite(CH_A, 0);
  ledcWrite(CH_B, 0);
}