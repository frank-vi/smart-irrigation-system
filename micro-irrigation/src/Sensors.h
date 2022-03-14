#ifndef SENSORS
#define  SENSORS


#include "SystemConfig.h"
#include "domain/sensors/BatteryImpl.h"
#include "domain/sensors/Photoresistor.h"
#include "domain/sensors/ImmersiblePump.h"
#include "domain/sensors/UltrasonicMeter.h"
#include "domain/sensors/ThermoHygrometer.h"
#include "domain/sensors/SoilMoistureMeter.h"


namespace Sensors {
    Pump* pump = new ImmersiblePump(RELAY_PIN);
    Battery* battery = new BatteryImpl(BATTERY_PIN, 9);
    LightSensor* photoresistor = new Photoresistor(PHOTO_RESISTOR_PIN);
    DistanceMeter* ultrasonicMeter = new UltrasonicMeter(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN);
    MoistureMeter* soilMoistureMeter = new SoilMoistureMeter(SOIL_MOISTURE_PIN);
    ThermoHygrometer* thermoHygrometer = new ThermoHygrometer(DHT_PIN);

    Pump* getPump() {
        return pump;
    }

    Battery* getBattery() {
        return battery;
    }

    LightSensor* getLightSensor() {
        return photoresistor;
    }

    DistanceMeter* getDistanceMeter() {
        return ultrasonicMeter;
    }

    MoistureMeter* getMoistureMeter() {
        return soilMoistureMeter;
    }

    ThermoHygrometer* getThermoHygrometer() {
        return thermoHygrometer;
    }
}

#endif