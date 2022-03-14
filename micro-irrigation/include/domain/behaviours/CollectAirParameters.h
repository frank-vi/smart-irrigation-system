#ifndef COLLECT_AIR_PARAMETERS
#define COLLECT_AIR_PARAMETERS

#include "Events.h"
#include "../sensors/ThermoHygrometer.h"
#include "../../architecture/TickerBehaviour.h"

class CollectAirParameters : public TickerBehaviour {
public:
	explicit CollectAirParameters(Configuration* configuration, int period, ThermoHygrometer* thermoHygrometer):
		TickerBehaviour(configuration, period), thermoHygrometer(thermoHygrometer) {}
	
	void onTick() override {
		float temperature = thermoHygrometer->getCelsius();
		if (!isnan(temperature)) {
			SimpleBehaviour::emit("temperature", String(temperature, 2));
		}

		float humidity = thermoHygrometer->getRelativeHumidity();
		if (!isnan(humidity)) {
			SimpleBehaviour::emit("humidity", String(humidity, 2));
		}
	}
	
private:
	ThermoHygrometer* thermoHygrometer;
};

#endif