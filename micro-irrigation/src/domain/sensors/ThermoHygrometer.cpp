#include "domain/sensors/ThermoHygrometer.h"

#define DHTTYPE DHT11

ThermoHygrometer::ThermoHygrometer(int pin) {
    this->sensor = new DHT(pin, DHT11);
    this->sensor->begin();
}

float ThermoHygrometer::getCelsius() {
    float t = sensor->readTemperature();
    return t;
}

float ThermoHygrometer::getFahrenheit() {
    return sensor->convertCtoF(getCelsius());
}

float ThermoHygrometer::getRelativeHumidity() {
    float h = sensor->readHumidity();
    return h;
}