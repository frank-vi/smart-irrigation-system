#ifndef AGENT
#define AGENT

#include <Arduino.h>

#include "Event.h"
#include "../List.h"
#include "Behaviour.h"
#include "../messaging/MessagingService.h"

class Behaviour; // forward declaration

class Agent {
public:
    Agent() = default;
    Agent(Behaviour* behaviour);
    Agent(List<Behaviour*>* behaviours);

    void addBehaviour(Behaviour* behaviour);
    void addBehaviours(List<Behaviour*>* behaviours);
	
	void emit(Event& event);
    void setMessagingService(MessagingService* service);
    void reactToEvent(EventListener* behaviour, String type);

private:
	MessagingService* messagingService;

    void startTask(Behaviour* behaviour);
};

#endif