#include "domain/sensors/Photoresistor.h"

#include <Arduino.h>
#include "MathUtils.h"

#define MAX_LUX 130000.00
#define REF_RESISTANCE 10000
#define USED_PHOTOCELL LightDependentResistor::GL5528

Photoresistor::Photoresistor(int pin) {
    this->photocell = new LightDependentResistor(pin, REF_RESISTANCE, USED_PHOTOCELL);
    this->photocell->setPhotocellPositionOnGround(false);
}

float Photoresistor::getLightIntensity() {
    float lux = photocell->getCurrentLux();
    float standardLux = MathUtils::mapFloat(lux, 0.0, 987703.56, 0.002, MAX_LUX);
    return standardLux;
}