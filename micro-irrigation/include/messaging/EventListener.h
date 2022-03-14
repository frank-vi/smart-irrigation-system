#ifndef EVENT_LISTENER
#define EVENT_LISTENER

#include <Arduino.h>
#include "../Comparable.h"

class EventListener : public Comparable<EventListener> {
    public:
        EventListener() {
            this->setUUID(random(1000));
        }

        long getUUID() {
            return this->UUID;
        }

        bool compareTo(EventListener* object) {
            return this->getUUID() == object->getUUID();
        }

        virtual void notify(String topic, String data) = 0;
    
    private:
        long UUID;
        void setUUID(long UUID) {
            this->UUID = UUID;
        }
};


#endif