#include "architecture/PerformanceAnalyzer.h"

#include <Arduino.h>

#define DEBOUNCE_TIME 10

namespace PerformanceAnalyzer {
    unsigned long epoc = 0L;
    TaskHandle_t handler = NULL;
    portMUX_TYPE mmux = portMUX_INITIALIZER_UNLOCKED;
    
    void ExternalInterrupt() {
        Serial.println("In interrupt");
        if (handler != NULL) {
            BaseType_t xYieldRequired;

            // Resume the suspended task.
            xYieldRequired = xTaskResumeFromISR(handler);

            if (xYieldRequired == pdTRUE) {
                //detachInterrupt(digitalPinToInterrupt(18));

                // We should switch context so the ISR returns to a different task.
                // NOTE:  How this is done depends on the port you are using.  Check
                // the documentation and examples for your port.
                portYIELD_FROM_ISR();
            }
        }
    }

    void startTime(unsigned long start) {
        epoc = start;
    }
    
    unsigned long timeForCore(SimpleBehaviour* behaviour, int core, unsigned long now) {
        return behaviour->getTime(core) + (behaviour->isOver() ? 0 : now - behaviour->getStart());
    }

    void cpUtilization(void* parameters) {
        List<SimpleBehaviour*>* behaviours = (List<SimpleBehaviour*>*) parameters;

        for (;;) {
            vTaskSuspend(NULL);
            portENTER_CRITICAL(&mmux);

            unsigned long now = millis();
            unsigned long timeSpentByCore0 = 0;
            unsigned long timeSpentByCore1 = 0;
        
            for (int i = 0; i < behaviours->size(); i++) {
                SimpleBehaviour* behaviour = behaviours->get(i);
                timeSpentByCore0 += timeForCore(behaviour, 0, now);
                timeSpentByCore1 += timeForCore(behaviour, 1, now);
                //behaviour->clearTime();
            }

            unsigned long timeTravelled = now - epoc;
            float utilization0 = (timeSpentByCore0 * 1.0 / timeTravelled) * 100;
            float utilization1 = (timeSpentByCore1 * 1.0 / timeTravelled) * 100;

            Serial.print("Epoc: ");
            Serial.println(epoc);
            Serial.print("Now: ");
            Serial.println(now);
            Serial.print("Core 0: ");
            Serial.println(timeSpentByCore0);
            Serial.print("Time travelled: ");
            Serial.println(timeTravelled);
            Serial.println(" ############################ ");
            Serial.println("    CPU Utilization  ");
            Serial.println(" ############################ ");
            Serial.print("Time spent by tasks on core 0: ");
            Serial.println(utilization0);
            Serial.print("Time spent by tasks on core 1: ");
            Serial.println(utilization1);
            Serial.println(" ############################ ");
            
            Serial.println("    Waiting 2 seconds ...");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            portEXIT_CRITICAL(&mmux);
            //epoc = millis();
            epoc -= 2000;
        }
    }

    void startCpUtilization(List<SimpleBehaviour*>* behaviours) {
        xTaskCreate(cpUtilization, "CPU Utilization", 1000, behaviours, 10, &handler);
        //Serial.println("handler is created: " + (handler != NULL));
    }
}