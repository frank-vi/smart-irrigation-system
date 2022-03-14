#ifndef SIMPLE_BEHAVIOUR
#define SIMPLE_BEHAVIOUR

#include "Behaviour.h"

class SimpleBehaviour : public Behaviour {
public:
	explicit SimpleBehaviour(Configuration* configuration);

	Agent* getAgent() override;

	void setAgent(Agent* agent) override;
	
	Configuration* getConfiguration() override;

protected:
	void emit(String topic, String content);

private:
	Agent* agent;
    Configuration* configuration;
};

#endif