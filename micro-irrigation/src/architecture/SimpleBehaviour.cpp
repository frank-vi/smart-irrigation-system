#include "architecture/SimpleBehaviour.h"
#include "domain/behaviours/Events.h"

SimpleBehaviour::SimpleBehaviour(Configuration* configuration): configuration(configuration) {}

Agent* SimpleBehaviour::getAgent() {
	return this->agent;	
}

void SimpleBehaviour::setAgent(Agent* agent) {
	this->agent = agent;
}

Configuration* SimpleBehaviour::getConfiguration() {
	return configuration;
}

void SimpleBehaviour::emit(String topic, String content) {
	Event event = Events::createEvent(topic, content);
	Events::printEvent(event);
	this->agent->emit(event);
}