#ifndef ULTRASONIC_DISTANCE_METER_H
#define ULTRASONIC_DISTANCE_METER_H

#include <Arduino.h>
#include <SHCSR04.h>
#include "DistanceMeter.h"

class UltrasonicMeter : public DistanceMeter {
    public:
        explicit UltrasonicMeter(int triggerPin, int echoPin);
        float getDistance() override;
        
    private:
        SHCSR04* ultrasonic;
        int triggerPin;
        int echoPin;
};

#endif