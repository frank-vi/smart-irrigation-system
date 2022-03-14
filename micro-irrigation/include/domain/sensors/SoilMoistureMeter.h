#ifndef SOIL_MOISTURE_METER_H
#define SOIL_MOISTURE_METER_H

#include "MoistureMeter.h"

class SoilMoistureMeter: public MoistureMeter {
    private:
        int pin;
        
    public:
        explicit SoilMoistureMeter(int pin);
        int getWaterTraces() override;
};

#endif