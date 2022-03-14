#include "architecture/ReceiverBehaviour.h"

#define MAX_SIZE 3

ReceiverBehaviour::ReceiverBehaviour(Configuration* configuration, int period):
	TickerBehaviour(configuration, period) {
	templates = new List<Event*>(MAX_SIZE);
	queue = new Queue();
}

void ReceiverBehaviour::notify(String topic, String content) {
	struct Event event;
	strncpy(event.type, topic.c_str(), 23);
	strncpy(event.description, content.c_str(), 16);
	queue->push(event);
}

List<Event*>* ReceiverBehaviour::getTemplates() {
	return templates;
}

void ReceiverBehaviour::reactTo(String eventType, String eventDescription) {
	struct Event event;
	strcpy(event.type, eventType.c_str());
	strcpy(event.description, eventType.c_str());
	Event* ev = &event;
	this->templates->add(ev);
}

void ReceiverBehaviour::onTick() {
	struct Event event;

	while(queue->pop(event)) {
		String type = String(event.type);
		String description = String(event.description);
		type.trim();
		description.trim();

		if (!type.isEmpty() && !description.isEmpty()) {
			handle(event);
		}	
	}
}
