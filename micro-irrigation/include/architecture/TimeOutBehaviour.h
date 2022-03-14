#ifndef TIME_OUT_BEHAVIOUR
#define TIME_OUT_BEHAVIOUR

#include "SimpleBehaviour.h"

class TimeOutBehaviour : public SimpleBehaviour {
public:
	explicit TimeOutBehaviour(Configuration* configuration, int timeout);
	
	void action() override;
	
	virtual void onTimeout() = 0;

private:
	int timeout;
};

#endif