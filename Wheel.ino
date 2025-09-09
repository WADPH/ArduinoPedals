#include <Joystick.h>

// Создаем объект Joystick для эмуляции руля
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
                   JOYSTICK_TYPE_MULTI_AXIS, 
                   0, 0,  // 0 кнопок, 0 HAT-переключателей
                   false, false, false, false, // Оси X, Y, Z, RX отключены
                   false, false,               // Оси RY, RZ отключены
                   true, false, false);        // Steering включен, остальные оси отключены

#define phaseA 2  // Зеленый провод (фаза A)
#define phaseB 3  // Белый провод (фаза B)

volatile long encoderPosition = 0; // Текущая позиция энкодера
long lastPosition = 0;             // Предыдущее положение энкодера
const int pulsesPerRevolution = 2400; // Импульсы на один оборот (600 P/R * 4)
const int maxSteeringRange = 720;     // Максимальный угол поворота руля (±360°)

void setup() {
  // Настраиваем пины энкодера как входы с подтяжкой
  pinMode(phaseA, INPUT_PULLUP);
  pinMode(phaseB, INPUT_PULLUP);

  // Настраиваем прерывания для отслеживания фаз энкодера
  attachInterrupt(digitalPinToInterrupt(phaseA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(phaseB), updateEncoder, CHANGE);

  // Устанавливаем диапазон руля
  Joystick.setSteeringRange(-maxSteeringRange, maxSteeringRange);

  // Запускаем Joystick
  Joystick.begin();
}

void loop() {
  // Преобразуем положение энкодера в угол поворота
  long currentPosition = encoderPosition;
  int steeringAngle = map(currentPosition, 
                          -pulsesPerRevolution * (maxSteeringRange / 360), 
                          pulsesPerRevolution * (maxSteeringRange / 360), 
                          -maxSteeringRange, maxSteeringRange);

  // Ограничиваем значение угла в пределах диапазона
  if (steeringAngle > maxSteeringRange) steeringAngle = maxSteeringRange;
  if (steeringAngle < -maxSteeringRange) steeringAngle = -maxSteeringRange;

  // Обновляем только если положение изменилось
  if (currentPosition != lastPosition) {
    Joystick.setSteering(steeringAngle);
    lastPosition = currentPosition;
  }

  // Задержка для стабильной работы (можно убрать, если не нужно)
  delay(10);
}

void updateEncoder() {
  // Чтение сигналов A и B
  int MSB = digitalRead(phaseA); // Most Significant Bit
  int LSB = digitalRead(phaseB); // Least Significant Bit

  int encoded = (MSB << 1) | LSB; // Составляем 2-битное число
  static int lastEncoded = 0;     // Предыдущее значение
  int sum = (lastEncoded << 2) | encoded;

  // Изменяем позицию в зависимости от направления вращения
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPosition++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPosition--;

  lastEncoded = encoded;
}
