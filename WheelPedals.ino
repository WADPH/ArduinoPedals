#include "Joystick.h"

// Создаем объект Joystick с тремя активными осями: Steering (руль), Accelerator (газ), Brake (тормоз)
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 0, 0,  // 0 кнопок, 0 HAT-переключателей
  false, false, false, false,     // X, Y, Z, RX отключены
  false, false,                   // RY, RZ отключены
  false,  true,  true);            // Steering (руль), Accelerator (газ), Brake (тормоз) включены

// Настройки диапазонов осей
const int ThrottleRange = 1023; // Максимальное значение газа (из аналогового сигнала)
const int brakeRange = 1023;       // Максимальное значение тормоза

void setup() {
  // Установка диапазонов осей
  Joystick.setThrottleRange(0, ThrottleRange);
  Joystick.setBrakeRange(0, brakeRange);
  // Joystick.setSteeringRange(0, 300);

  // Инициализация джойстика
  Joystick.begin();
}

void loop() {
  // Читаем значения с аналоговых входов
  int ThrottleValue = analogRead(A0); // Сигнал газа
  int brakeValue = analogRead(A1);       // Сигнал тормоза
  // int steeringValue = analogRead(A2);    // Сигнал руля

  // Преобразуем значения газа и тормоза
  int mappedThrottle = map(ThrottleValue, 0, 1023, 0, 255/*acceleratorRange*/);
  int mappedBrake = map(brakeValue, 0, 1023, 0, 255/*brakeRange*/);
  // int mappedSteering = map(steeringValue, 0, 1023, 0, 300);

  // Отправляем значения на джойстик
  Joystick.setThrottle(mappedThrottle);
  Joystick.setBrake(mappedBrake);
