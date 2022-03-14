#ifndef COLLECT_RAINFALL
#define COLLECT_RAINFALL

#include "Events.h"
#include "../../MathUtils.h"
#include "../sensors/DistanceMeter.h"
#include "../../architecture/TickerBehaviour.h"

#define GAUGE_AREA 0.3848 //m^2
#define MAX_DISTANCE 0.13 //m

class CollectRainfall : public TickerBehaviour {
public:
	explicit CollectRainfall(Configuration* configuration, int period, DistanceMeter* meter):
		TickerBehaviour(configuration, period), meter(meter) {}
	
	void onTick() override {
		float waterDistanceInCm = this->meter->getDistance();
		
        float waterDistanceInMeter = waterDistanceInCm / 100;
        float filteredDistance = waterDistanceInMeter > MAX_DISTANCE ? MAX_DISTANCE : waterDistanceInMeter;
        float waterLevelInLiters = MathUtils::mapFloat(filteredDistance, 0, MAX_DISTANCE, 0.65, 0);
        float precipitationQuantity = waterLevelInLiters / GAUGE_AREA;
        
		SimpleBehaviour::emit("rainfall", String(precipitationQuantity, 2));
	}
	
private:
	DistanceMeter* meter;
	float lastRainfall;
};

#endif