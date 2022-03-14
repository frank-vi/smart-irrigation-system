#include "architecture/TickerBehaviour.h"

TickerBehaviour::TickerBehaviour(Configuration* configuration, int period):
	SimpleBehaviour(configuration) {
	this->period = period;
}

void TickerBehaviour::action() {
	for (;;) {
		SimpleBehaviour::setStartTime(millis());
		onTick();
		SimpleBehaviour::updateTime(millis());
		vTaskDelay(period / portTICK_PERIOD_MS);
	}
}