// FASE 2 - HAL del motor (puente H L9110S en IO12 / IO14, PWM por hardware ledc)
#pragma once
#include <stdint.h>

void motorInit();                      // timer + canales ledc (10 bits, 1000 Hz)
void motorPWM(uint32_t cicloTrabajo);  // avance: 0..1023 (se satura en 1023)
void motorBrake();                     // frenado dinamico (ambas entradas en alto)
void motorStop();                      // giro por inercia / coast (ambas en bajo)