#include "architecture/Agent.h"
#include "architecture/Behaviour.h"
#include "architecture/BehaviourInTaskMapper.h"

#include "architecture/PerformanceAnalyzer.h"

#define MAX_BEHAVIOURS 2

Agent::Agent(List<Behaviour*>* behaviours) {
    this->addBehaviours(behaviours);
}
    
Agent::Agent(Behaviour* behaviour) {
    this->addBehaviour(behaviour);
}

void Agent::addBehaviour(Behaviour* behaviour) {
    behaviour->setAgent(this);
    startTask(behaviour);
}

void Agent::addBehaviours(List<Behaviour*>* behaviours) {
    for(int i = 0; i < behaviours->size(); i++) {
        this->addBehaviour(behaviours->get(i));
    }
}

void Agent::emit(Event& event) {
    if (messagingService != NULL) {
        messagingService->publishOn(event.description, event.type);
    }
}

void Agent::setMessagingService(MessagingService* service) {
    this->messagingService = service;
}

void Agent::reactToEvent(EventListener* behaviour, String type) {    
    this->messagingService->subscribeTo(behaviour, type);
}

void Agent::startTask(Behaviour* behaviour) {
    Configuration* configuration = behaviour->getConfiguration();

    xTaskCreate( BehaviourInTaskMapper::behaviourAsTask,
                configuration->getName(),
                configuration->getStack(),
                behaviour,
                configuration->getPriority(),
                NULL );
}