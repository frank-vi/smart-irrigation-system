#ifndef COLLECT_SOIL_MOISTURE_
#define COLLECT_SOIL_MOISTURE

#include "Events.h"
#include "../sensors/MoistureMeter.h"
#include "../../architecture/TickerBehaviour.h"

class CollectSoilMoisture : public TickerBehaviour {
public:
	explicit CollectSoilMoisture(Configuration* configuration, int period, MoistureMeter* meter):
		TickerBehaviour(configuration, period), meter(meter) {}
	
	void onTick() override {
		float moisture = meter->getWaterTraces();
		SimpleBehaviour::emit("soil-moisture", String(moisture, 2));
	}
	
private:
	MoistureMeter* meter;
};

#endif