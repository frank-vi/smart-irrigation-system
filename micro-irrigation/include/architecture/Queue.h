#ifndef QUEUE_WRAPPER_H
#define QUEUE_WRAPPER_H

#include <Arduino.h>
#include "Event.h"

class Queue {
    public:
        explicit Queue() {
            this->queue = xQueueCreate(10, sizeof(Event));
        }

        bool push(Event& message) {
            return xQueueSend(this->queue, &message, (TickType_t) 10) == pdPASS;
        }

        bool pop(Event& message) {
            return xQueueReceive(this->queue, &message, (TickType_t) 10);
        }

    private:
        QueueHandle_t queue;
};

#endif