#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "Sensor.h"
 
class LightSensor {
    public:
        virtual float getLightIntensity() = 0;
};

#endif