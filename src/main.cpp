#include <Arduino.h>

// ---------- HAL (Fase 2) ----------
constexpr int PIN_MOTOR_A = 12;
constexpr int PIN_MOTOR_B = 14;
constexpr int PWM_FREQ    = 1000;
constexpr int PWM_RES     = 10;          // 0..1023
constexpr int CH_A = 0, CH_B = 1;

void hal_init() {
  ledcSetup(CH_A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(PIN_MOTOR_A, CH_A);
  ledcAttachPin(PIN_MOTOR_B, CH_B);
}
void motor_forward(uint32_t duty) { ledcWrite(CH_A, duty); ledcWrite(CH_B, 0); }
void motor_brake()  { ledcWrite(CH_A, 1023); ledcWrite(CH_B, 1023); }
void motor_coast()  { ledcWrite(CH_A, 0);    ledcWrite(CH_B, 0); }

// ---------- Arduino ----------
void setup() {
  Serial.begin(115200);
  hal_init();
  motor_coast();
}

void loop() {
}