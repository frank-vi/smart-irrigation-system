#ifndef LOCAL_MESSAGING
#define LOCAL_MESSAGING

#include <Arduino.h>

#include "../List.h"
#include "EventListener.h"
#include "MessagingService.h"

class LocalMessaging : public MessagingService {
    public:
        LocalMessaging() {
            this->listenerOnTopic = new List<EventListener*>(2);
        }

        void publishOn(String data, String topic) {
            int size = listenerOnTopic->size();
            for (int i = 0; i < size; i++) {
                EventListener* l = listenerOnTopic->get(i);
                l->notify(topic, data);
            }
        }

        void subscribeTo(EventListener* subscriber, String topic) {
            int element = 0;
            int size = this->listenerOnTopic->size();
            bool isAlreadySubscribed = false;
            while (!isAlreadySubscribed && element < size) {
                if (this->listenerOnTopic->get(element)->getUUID() ==
                        subscriber->getUUID()) {
                            isAlreadySubscribed = true;
                }
                element++;
            }

            if (!isAlreadySubscribed) {
                this->listenerOnTopic->add(subscriber);
            }
        }

        void unsubscribeFrom(EventListener* subscriber, String topic) {
            this->listenerOnTopic->remove(subscriber);
        }

    private:
        List<EventListener*>* listenerOnTopic;
};

#endif