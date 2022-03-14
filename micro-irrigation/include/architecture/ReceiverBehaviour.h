#ifndef RECEIVER_BEHAVIOUR
#define RECEIVER_BEHAVIOUR

#include "Event.h"
#include "Queue.h"
#include "TickerBehaviour.h"
#include "../messaging/EventListener.h"

class ReceiverBehaviour : public TickerBehaviour, public EventListener {
public:
	explicit ReceiverBehaviour(Configuration* configuration, int period);
	
	void notify(String topic, String content) override;
	
	List<Event*>* getTemplates();

protected:
	void reactTo(String eventType, String eventDescription);
	
	void onTick() override;
	
	virtual void handle(Event& event) = 0;

private:
	Queue* queue;
	List<Event*>* templates;
};

#endif