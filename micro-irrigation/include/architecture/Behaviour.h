#ifndef BEHAVIOUR_AGENT_H
#define BEHAVIOUR_AGENT_H

#include "Agent.h"
#include "Configuration.h"

class Agent;

class Behaviour {
public:
	virtual void action() = 0;

	virtual Agent* getAgent() = 0;
	
	virtual void setAgent(Agent* agent) = 0;
	
	virtual Configuration* getConfiguration() = 0;

};

#endif