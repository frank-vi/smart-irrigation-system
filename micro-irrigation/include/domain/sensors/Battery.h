#ifndef BATTERY_H
#define BATTERY_H

class Battery {
public:
    virtual float getVoltage() = 0;
    virtual int getPercentage() = 0;
};

#endif