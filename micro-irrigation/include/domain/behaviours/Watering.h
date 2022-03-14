#ifndef MANAGE_IRRIGATION
#define MANAGE_IRRIGATION

#include "Events.h"
#include "../sensors/Pump.h"
#include "StopWatering.h"
#include "../../architecture/Configuration.h"
#include "../../architecture/ReceiverBehaviour.h"

#define EQUAL 0

#define INTERNAL_EVENT 		"internal"
#define IRRIGATION_STATUS 	"irrigation/status"
#define IRRIGATION_COMMANDS "irrigation/command"

#define ON 				"on"
#define OFF 			"off"
#define ALREADY_STARTED "already-started"
#define TIMEOUT 		"timeout"


class Watering : public ReceiverBehaviour {
public:
	explicit Watering(Configuration* configuration, int period, Pump* pump):
		ReceiverBehaviour(configuration, period) {		
		this->pump = pump;
	}
	
	void handle(Event& event) override {
		if (!this->pump->isOn() && isCommandOn(event)) {
			this->pump->switchOn();
			this->getAgent()->addBehaviour(timeout);
			SimpleBehaviour::emit(IRRIGATION_STATUS, ON);
		} else if (this->pump->isOn()) {
			if (isCommandOff(event) || isTimeout(event)) {
				this->pump->switchOff();
				SimpleBehaviour::emit(IRRIGATION_STATUS, OFF);
			} else if (isCommandOn(event)) {
				SimpleBehaviour::emit(IRRIGATION_STATUS, ALREADY_STARTED);
			}
		}
	}

private:
	Pump* pump;
	Configuration* configuration = new Configuration("StopWatering", 2, 5000);
	Behaviour* timeout = new StopWatering(configuration, 10000);
	
	bool isCommandOn(Event& event) {
		return checkEvent(event.type, IRRIGATION_COMMANDS, event.description, ON);
	}
	
	bool isCommandOff(Event& event) {
		return checkEvent(event.type, IRRIGATION_COMMANDS, event.description, OFF);
	}
	
	bool isTimeout(Event& event) {
		return checkEvent(event.type, INTERNAL_EVENT, event.description, TIMEOUT);
	}

	bool checkEvent(const char* type, const char* desiredType, const char* description, const char* desiredDescription) {
		return String(type) == String(desiredType)
            && String(description) == String(desiredDescription);
	}
};

#endif