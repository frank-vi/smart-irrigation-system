#ifndef COLLECT_LIGHT_INTENSITY
#define COLLECT_LIGHT_INTENSITY

#include "Events.h"
#include "../sensors/LightSensor.h"
#include "../../architecture/TickerBehaviour.h"

class CollectLightIntensity : public TickerBehaviour {
public:
	explicit CollectLightIntensity(Configuration* configuration, int period, LightSensor* sensor):
		TickerBehaviour(configuration, period), sensor(sensor) {}
	
	void onTick() override {
		float sunLight = sensor->getLightIntensity();
		SimpleBehaviour::emit("light-intensity", String(sunLight, 2));
	}
	
private:
	LightSensor* sensor;
};

#endif