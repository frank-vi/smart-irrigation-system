#ifndef PERFOMANCE_ANALYZER
#define PERFOMANCE_ANALYZER

#include "List.h"
#include "architecture/SimpleBehaviour.h"

namespace PerformanceAnalyzer {
    void ExternalInterrupt();

    void startTime(unsigned long start);
    
    void cpUtilization(void* parameters);

    void startCpUtilization(List<SimpleBehaviour*>* behaviours);
}

#endif