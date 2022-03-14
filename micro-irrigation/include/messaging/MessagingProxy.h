#ifndef MESSAGING_PROXY
#define MESSAGING_PROXY

#include "../architecture/ReceiverBehaviour.h"

#include "BleMessaging.h"
#include "LocalMessaging.h"
#include "MessagingService.h"

class MessagingProxy : public MessagingService, public ReceiverBehaviour {
public:
    explicit MessagingProxy(BleMessaging* bleMessaging, LocalMessaging* localMessaging):
        ReceiverBehaviour(new Configuration("Proxy", 2, 50000), 1000),
        bleMessaging(bleMessaging), localMessaging(localMessaging) {
            this->bleMessaging->subscribeTo(this, "all");
        }

    void publishOn(String data, String topic) {
        String outTopic = String("out:") + topic;
        ReceiverBehaviour::notify(outTopic, data);
    }

    void subscribeTo(EventListener* subscriber, String topic) {
        localMessaging->subscribeTo(subscriber, topic);
    }

    void unsubscribeFrom(EventListener* subscriber, String topic) {
        localMessaging->unsubscribeFrom(subscriber, topic);
    }

    void handle(Event& event) override {
        String eventType = String(event.type);
        String data = String(event.description);

        if (eventType.startsWith("in:")) {
            eventType.remove(0, 3);
            localMessaging->publishOn(data, eventType);
        } else if (eventType.startsWith("out:")) {
            eventType.remove(0, 4);
            localMessaging->publishOn(data, eventType);

            if (eventType != "internal") {
                bleMessaging->publishOn(data, eventType);
            }
        }
    }

private:
    BleMessaging* bleMessaging;
    LocalMessaging* localMessaging;
};

#endif