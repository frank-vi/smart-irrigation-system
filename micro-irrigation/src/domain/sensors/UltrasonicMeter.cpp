#include "domain/sensors/UltrasonicMeter.h"

UltrasonicMeter::UltrasonicMeter(int triggerPin, int echoPin)
    : triggerPin(triggerPin), echoPin(echoPin) {
    this->ultrasonic = new SHCSR04();
}

float UltrasonicMeter::getDistance() {
    float distance = this->ultrasonic->read(triggerPin, echoPin);
    return distance > 2000.0 ? 0 : distance;
}