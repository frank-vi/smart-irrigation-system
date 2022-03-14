#ifndef BEHAVIOUR_TASK_MAPPER
#define BEHAVIOUR_TASK_MAPPER

#include "Behaviour.h"

namespace BehaviourInTaskMapper {
    void behaviourAsTask(void *parameters) {
        Behaviour* behaviour = (Behaviour*) parameters;
        behaviour->action();
        vTaskDelete(NULL);
    }
}

#endif