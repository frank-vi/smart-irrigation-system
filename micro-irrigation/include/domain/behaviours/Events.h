#ifndef EVENTS_UTILS
#define EVENTS_UTILS

#include <Arduino.h>
#include "architecture/Event.h"

#define DECIMALS 2

namespace Events {
/*
    Event createEvent(String parameter, double value) {
        String sValue = String(value, DECIMALS);
        return createEvent(parameter, sValue);
    }
*/

    inline Event createEvent(String parameter, String description) {
        struct Event event;
		strcpy(event.type, parameter.c_str());
		strcpy(event.description, description.c_str());
        
		return event;
    }

    inline void printEvent(Event& event) {
        String type = String(event.type);
        String description = String(event.description);
        type.trim();
        description.trim();

        Serial.println(type + ":" + description);
    }
}

#endif