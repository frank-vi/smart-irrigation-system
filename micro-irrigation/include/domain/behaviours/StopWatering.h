#ifndef STOPPING_IRRIGATION
#define STOPPING_IRRIGATION

#include "Events.h"
#include "architecture/TimeOutBehaviour.h"

class StopWatering : public TimeOutBehaviour {
public:
    explicit StopWatering(Configuration* configuration, int timeout):
        TimeOutBehaviour(configuration, timeout) {}
    
    void onTimeout() override {
        SimpleBehaviour::emit("internal", "timeout");
    }
};

#endif