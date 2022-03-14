#ifndef COLLECT_BATTERY_LEVEL
#define COLLECT_BATTERY_LEVEL

#include "Events.h"
#include "MathUtils.h"
#include "../sensors/Battery.h"
#include "../../architecture/TickerBehaviour.h"

class CollectBatteryLevel : public TickerBehaviour {
public:
	explicit CollectBatteryLevel(Configuration* configuration, int period, Battery* battery):
		TickerBehaviour(configuration, period), battery(battery) {}
	
	void onTick() override {
        float batteryLevel = battery->getPercentage();
		SimpleBehaviour::emit("battery-level", String(batteryLevel, 2));
	}
	
private:
	Battery* battery;
};

#endif