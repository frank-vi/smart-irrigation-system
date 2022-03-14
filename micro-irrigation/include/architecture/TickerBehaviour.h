#ifndef TICKER_BEHAVIOUR
#define TICKER_BEHAVIOUR

#include "SimpleBehaviour.h"

class TickerBehaviour : public SimpleBehaviour {
public:
	explicit TickerBehaviour(Configuration* configuration, int period);
	
	void action() override;
	
	virtual void onTick() = 0;

private:
    int period;
};

#endif