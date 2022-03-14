#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

#include "LightSensor.h"
#include <LightDependentResistor.h>

class Photoresistor : public LightSensor {
    public:
        Photoresistor(int pin);
        float getLightIntensity() override;
    
    private:
        LightDependentResistor* photocell;
};

#endif