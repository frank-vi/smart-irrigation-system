#ifndef THERMOHYGROMETER_H
#define THERMOHYGROMETER_H

#include <DHT.h>
#include "domain/sensors/Thermometer.h"
#include "domain/sensors/Hygrometer.h"

class ThermoHygrometer : public Thermometer, public Hygrometer {
private:
    DHT* sensor;

public:
    explicit ThermoHygrometer(int pin);
    float getCelsius() override;
    float getFahrenheit() override;
    float getRelativeHumidity() override;
};

#endif
