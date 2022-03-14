#ifndef IMMERSIBLE_PUMP
#define IMMERSIBLE_PUMP

#include <Arduino.h>
#include "Pump.h"

class ImmersiblePump : public Pump {
public:
    explicit ImmersiblePump(int pin) : pin(pin) {
        pinMode(pin, OUTPUT);
        this->switchOff();
    }

    void switchOn() {
        digitalWrite(pin, LOW);
        this->status = true;
    }

    void switchOff() {
        digitalWrite(pin, HIGH);
        this->status = false;
    }

    bool isOn() {
        return this->status;
    }

private:
    int pin;
    bool status;
};

#endif