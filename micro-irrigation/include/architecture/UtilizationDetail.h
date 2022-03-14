#ifndef UTILIZZATION_DETAIL
#define UTILIZATION_DETAIL

#include <Arduino.h>

class UtilizationDetail {
public:
    UtilizationDetail(uint32_t core, unsigned long timeSpent)
        : core(core), timeSpent(timeSpent) {}

public:
    uint32_t core;
	unsigned long timeSpent;
};

#endif