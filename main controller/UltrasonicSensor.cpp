#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint8_t t, uint8_t e) {
    trigPin = t;
    echoPin = e;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

long UltrasonicSensor::readDistanceCM() {
    long duration;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    return (duration * 0.034 / 2);
}
