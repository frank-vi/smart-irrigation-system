#ifndef THERMOMETER_H
#define THERMOMETER_H

class Thermometer {
    public:
        virtual float getCelsius() = 0;
        virtual float getFahrenheit() = 0;
};

#endif
