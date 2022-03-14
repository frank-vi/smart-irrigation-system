#ifndef EVENT_BEHAVIOUR
#define EVENT_BEHAVIOUR

#include <Arduino.h>

#define SIZE_OF_TYPE 23
#define SIZE_OF_DESCRIPTION 16

struct Event {
	char type[SIZE_OF_TYPE];
	char description[SIZE_OF_DESCRIPTION];
};

#endif