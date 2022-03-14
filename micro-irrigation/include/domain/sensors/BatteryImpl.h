#ifndef BATTERY_IMPL_H
#define BATTERY_IMPL_H

#include "Battery.h"
#include "../../MathUtils.h"

#define MAX_ADC_READING 1024.0
#define ESP32_VOLTAGE 3.3

class BatteryImpl : public Battery {
public:
    explicit BatteryImpl(int pin, float maxVoltage):
        pin(pin), maxVoltage(maxVoltage),
        divisionFactor(maxVoltage/ESP32_VOLTAGE),
        conversionFactor((ESP32_VOLTAGE / MAX_ADC_READING) * divisionFactor)  {}

    float getVoltage() {
        int rawData = analogRead(pin);
        float voltage = rawData * conversionFactor;
        return voltage;
    }

    int getPercentage() {
        float batteryVoltage = getVoltage();
        return MathUtils::mapFloat(batteryVoltage, 0, maxVoltage, 0, 100.0);
    }

private:
    int pin;
    const float maxVoltage;
    const float divisionFactor;
    const float conversionFactor;
};

#endif