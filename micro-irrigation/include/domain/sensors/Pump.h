#ifndef PUMP
#define PUMP

class Pump {
public:
    virtual void switchOn() = 0;
    virtual void switchOff() = 0;
    virtual bool isOn() = 0;
};

#endif