#include "Sensors.h"

#include "architecture/Agent.h"
#include "architecture/Behaviour.h"
#include "architecture/Configuration.h"

#include "domain/behaviours/Watering.h"
#include "domain/behaviours/CollectRainfall.h"
#include "domain/behaviours/CollectSoilMoisture.h"
#include "domain/behaviours/CollectBatteryLevel.h"
#include "domain/behaviours/CollectAirParameters.h"
#include "domain/behaviours/CollectLightIntensity.h"

#include "messaging/MessagingProxy.h"
#include "messaging/MessagingService.h"

#include "architecture/PerformanceAnalyzer.h"
 

void setup() {
    PerformanceAnalyzer::startTime(millis());

    Serial.begin(115200);
    delay(1000);

    BleMessaging* ble = new BleMessaging();
    LocalMessaging* local = new LocalMessaging();
    MessagingProxy* service = new MessagingProxy(ble, local);
    new Agent(service);

    Configuration* airConfig = new Configuration("CollectAirParameters", 2, 5000);
    SimpleBehaviour* airMonitoring = new CollectAirParameters(airConfig, 1000, Sensors::getThermoHygrometer());
    
    Configuration* soilConfig = new Configuration("CollectSoilMoisture", 2, 5000);
    SimpleBehaviour* soilMonitoring = new CollectSoilMoisture(soilConfig, 1000, Sensors::getMoistureMeter());

    Configuration* sunLightConfig = new Configuration("CollectLightIntensity", 2, 5000);
    SimpleBehaviour* sunLightMonitoring = new CollectLightIntensity(sunLightConfig, 1000, Sensors::getLightSensor());

    Configuration* rainfallConfig = new Configuration("CollectRainfall", 2, 5000);
    SimpleBehaviour* rainfallMonitoring = new CollectRainfall(rainfallConfig, 1000, Sensors::getDistanceMeter());

    Agent* environemntalAgent = new Agent();
    environemntalAgent->setMessagingService(service);
    environemntalAgent->addBehaviour(airMonitoring);
    environemntalAgent->addBehaviour(soilMonitoring);
    environemntalAgent->addBehaviour(sunLightMonitoring);
    environemntalAgent->addBehaviour(rainfallMonitoring);

    Configuration* batteryConfig = new Configuration("CollectBatteryLevel", 2, 5000);
    SimpleBehaviour* batteryLevelMonitoring = new CollectBatteryLevel(batteryConfig, 1000, Sensors::getBattery());

    Agent* deviceAgent = new Agent(batteryLevelMonitoring);
    deviceAgent->setMessagingService(service);

    Configuration* irrigationConfig = new Configuration("ManageIrrigation", 2, 5000);
    Watering* watering = new Watering(irrigationConfig, 1000, Sensors::getPump());

    Agent* irrigationAgent = new Agent();
    irrigationAgent->setMessagingService(service);
    irrigationAgent->addBehaviour(watering);
    irrigationAgent->reactToEvent(watering, IRRIGATION_COMMANDS);
	irrigationAgent->reactToEvent(watering, INTERNAL_EVENT);

    List<SimpleBehaviour*>* behaviours = new List<SimpleBehaviour*>(10);
    behaviours->add(airMonitoring);
    behaviours->add(soilMonitoring);
    behaviours->add(sunLightMonitoring);
    behaviours->add(rainfallMonitoring);
    behaviours->add(batteryLevelMonitoring);
    behaviours->add(watering);
    PerformanceAnalyzer::startCpUtilization(behaviours);
    pinMode(18, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(18), PerformanceAnalyzer::ExternalInterrupt, FALLING);
}

void loop() {}