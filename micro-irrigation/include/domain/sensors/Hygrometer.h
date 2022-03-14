#ifndef HYGROMETER_H
#define HYGROMETER_H

class Hygrometer {
    public:
        virtual float getRelativeHumidity() = 0;
};

#endif