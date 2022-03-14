#include "architecture/TimeOutBehaviour.h"

TimeOutBehaviour::TimeOutBehaviour(Configuration* configuration, int timeout):
	SimpleBehaviour(configuration) {
	this->timeout = timeout;
}

void TimeOutBehaviour::action() {
	vTaskDelay(timeout / portTICK_PERIOD_MS);
	SimpleBehaviour::setStartTime(millis());
	onTimeout();
	SimpleBehaviour::updateTime(millis());
}