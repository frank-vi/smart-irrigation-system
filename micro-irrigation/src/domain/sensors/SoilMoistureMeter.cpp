#include "domain/sensors/SoilMoistureMeter.h"

#include <Arduino.h>

SoilMoistureMeter::SoilMoistureMeter(int pin): pin(pin) {}

int SoilMoistureMeter::getWaterTraces() {
    int moisture = analogRead(pin);
    int percentage = map(moisture, 0, 1023, 100, 0);
    return percentage;
}
