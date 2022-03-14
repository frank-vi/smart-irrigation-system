#ifndef BEHAVIOUR_CONFIGURATION_H
#define BEHAVIOUR_CONFIGURATION_H

#include <Arduino.h>

class Configuration {
public:
    explicit Configuration(String name, int priority, int stack):
        name(name), priority(priority), stack(stack) {}

    const char* getName() { return this->name.c_str(); }
    int getPriority() {return this->priority; }
    int getStack() { return this->stack; }

private:
    String name;
    int priority;
    int stack;
};

#endif