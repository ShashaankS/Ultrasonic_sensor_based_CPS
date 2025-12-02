#include "KalmanFilter.h"
#include "UltrasonicSensor.h"
#include "TankFSM.h"
#include <Servo.h>

#define SERVO_PIN 7

// -------------------------
// Library Objects
// -------------------------
UltrasonicSensor us(9, 10);
KalmanFilter kf(0.5, 5, 40);
TankFSM fsm(30, 15, 50);

Servo valveServo;

void setup() {
  Serial.begin(9600);

  valveServo.attach(SERVO_PIN);
  valveServo.write(0);
}

void loop() {

  long rawDist = us.readDistanceCM();
  float filtered = kf.update(rawDist);

  fsm.update(filtered);
  TankState current = fsm.getState();

  // -------------------------
  // Servo control
  // -------------------------
  if (current == FILL)
    valveServo.write(90);
  else
    valveServo.write(0);

  // -------------------------
  // Debug Output
  // -------------------------
  Serial.print("Raw: ");
  Serial.print(rawDist);
  Serial.print(" | Filtered: ");
  Serial.print(filtered);
  Serial.print(" | State: ");
  Serial.println(fsm.getStateName());

  delay(150);
}
