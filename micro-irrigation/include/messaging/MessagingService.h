#ifndef MESSAGING_SERVICE
#define MESSAGING_SERVICE

#include "EventListener.h"

class MessagingService {
public:
    virtual void publishOn(String data, String topic) = 0;
    virtual void subscribeTo(EventListener* subscriber, String topic) = 0;
    virtual void unsubscribeFrom(EventListener* subscriber, String topic) = 0;
};

#endif