#ifndef BLE_MESSAGING
#define BLE_MESSAGING

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "EventListener.h"
#include "MessagingService.h"

#define EnvironmentalService        "181A"
#define BatteryService 		        "180F"
#define HumidityCharacteristic      "2A6F"
#define TemperatureCharacteristic   "2A6E"
#define BatteryCharacteristic       "2A19"
#define UserDescriptor              "2901"

#define WeatherService		        "05b35eee-baf4-44c9-b5a0-f07a6b4ece7a"
#define SoilService			        "253b1e1f-5c26-4503-a1fe-a639dd09a01b"
#define SoilMoistureCharacteristic 	"2e4288d0-6e8c-4521-9dec-c189e93438ce"
#define RainCharacteristic			"fb38da7a-8c18-4ba2-b1a8-17889f5e7f84"
#define LightCharacteristic			"b2cdbfab-714e-4fe2-bb45-59de64f6112e"
#define IrrigationService			"03ff88af-a16a-4444-9ba5-81e563adfcce"
#define IrrigationCharacteristic	"feda4fab-5b29-4178-9864-58b677cac98d"

class Characteristic {
public:
    Characteristic* withUUID(String uuid) {
        this->uuid = uuid;
        return this;
    }

    Characteristic* withDescription(String description) {
        this->description = description;
        return this;
    }

    Characteristic* withProperties(uint32_t properties) {
        this->properties = properties;
        return this;
    }

    Characteristic* dataSource(BLEDescriptorCallbacks* handler) {
        this->handler = handler;
        return this;
    }

    BLECharacteristic* build() {
        BLECharacteristic* characteristic = new BLECharacteristic(this->uuid.c_str(), properties);
        BLEDescriptor* descriptor = new BLEDescriptor(UserDescriptor);
        descriptor->setValue(this->description.c_str());
        characteristic->addDescriptor(descriptor);
        characteristic->addDescriptor(new BLE2902());
        return characteristic;
    }

private:
    uint32_t properties = 0U;
    String uuid;
    String description;
    BLEDescriptorCallbacks* handler;
};

class Service {
public:
    Service* addCharacteristic(String uuid, String description) {
        createCharacteristic(uuid, description);
        return this;
    }

    Service* addCharacteristic(String uuid, String description, BLECharacteristicCallbacks* handler) {
        createCharacteristic(uuid, description, BLECharacteristic::PROPERTY_WRITE)
            ->setCallbacks(handler);
        return this;
    }

    void start() {
        this->service->start();
    }

    static Service* createService(BLEServer* server, const char* uuid) {
        return new Service(server, uuid);
    }

private:
    BLEService* service;

    Service() = default;
    Service(BLEServer* server, const char* uuid) {
        this->service = server->createService(uuid);
    }
    
    BLECharacteristic* createCharacteristic(String uuid, String description, uint32_t property = 0U) {
        Characteristic* characteristic = new Characteristic();
        BLECharacteristic* bleCharacteristic = 
            characteristic
                ->withUUID(uuid)
                ->withDescription(description)
                ->withProperties(
                    BLECharacteristic::PROPERTY_READ |
                    BLECharacteristic::PROPERTY_NOTIFY | property)
                ->build();
        service->addCharacteristic(bleCharacteristic);
        return bleCharacteristic;
    }
};

class BleMessaging : public MessagingService, public BLECharacteristicCallbacks {
    public:
        explicit BleMessaging() {
            BLEDevice::init("Micro-irrigation");
            this->server = BLEDevice::createServer();
            
            Service::createService(server, EnvironmentalService)
                ->addCharacteristic(HumidityCharacteristic, "Humidity 0 to 100%")
                ->addCharacteristic(TemperatureCharacteristic, "Temperature 0-60°C")
                ->start();

            Service::createService(server, BatteryService)
                ->addCharacteristic(BatteryCharacteristic, "Battery Level 0 to 100%")
                ->start();

            Service::createService(server, SoilService)
                ->addCharacteristic(SoilMoistureCharacteristic, "Soil Moisture 0 to 100%")
                ->start();

            Service::createService(server, WeatherService)
                ->addCharacteristic(RainCharacteristic, "Amount of precipitations in millimeters")
                ->addCharacteristic(LightCharacteristic, "Light intensity in lux")
                ->start();

            Service::createService(server, IrrigationService)
                ->addCharacteristic(IrrigationCharacteristic, "Sprinkler status: On or Off", this)
                ->start();
            
            server->getAdvertising()->start();
        }

        void subscribeTo(EventListener* subscriber, String topic) {
            this->eventListener = subscriber;
        }

        void unsubscribeFrom(EventListener* subscriber, String topic) {
            this->eventListener = NULL;
        }

        void publishOn(String content, String topic) {
            if (topic == "irrigation/status") {
                content.trim();
                notifyStatus(IrrigationService, IrrigationCharacteristic, content.c_str());
            } else if (topic == "temperature") {
                notifyMeasurement(EnvironmentalService, TemperatureCharacteristic, content.toDouble() * 100);
            } else if (topic == "humidity") {
                notifyMeasurement(EnvironmentalService, HumidityCharacteristic, content.toDouble() * 100);
            } else if (topic == "battery-level") {
                notifyPercentage(BatteryService, BatteryCharacteristic, content.toInt());
            } else if (topic == "soil-moisture") {
                notifyStatus(SoilService, SoilMoistureCharacteristic, content.c_str());
            } else if (topic == "rainfall") {
                notifyStatus(WeatherService, RainCharacteristic, content.c_str());
            } else if (topic == "light-intensity") {
                notifyStatus(WeatherService, LightCharacteristic, content.c_str());
            }
        }

        void notifyMeasurement(const char* serviceUUID, const char* characteristicUUID, int measurement) {
            BLECharacteristic* characteristic = getCharacteristic(serviceUUID, characteristicUUID);
            characteristic->setValue(measurement);
            characteristic->notify();
        }

        void notifyPercentage(const char* serviceUUID, const char* characteristicUUID, int measurement) {
            BLECharacteristic* characteristic = getCharacteristic(serviceUUID, characteristicUUID);
            characteristic->setValue(measurement);
            characteristic->notify();
        }

        void notifyStatus(const char* serviceUUID, const char* characteristicUUID, const char* status) {
            BLECharacteristic* characteristic = getCharacteristic(serviceUUID, characteristicUUID);
            characteristic->setValue(status);
            characteristic->notify();
        }

        BLECharacteristic* getCharacteristic(String uuid, String characteristicUUUID) {
            return server->getServiceByUUID(uuid.c_str())
                        ->getCharacteristic(characteristicUUUID.c_str());
        }

        void onRead(BLECharacteristic* characteristic) {
        }

        void onWrite(BLECharacteristic* characteristic) {
            String s = String(characteristic->getValue().c_str());
            s.trim();
            Serial.println(s);
            this->eventListener->notify("in:irrigation/command", s);
        }

    private:
        BLEServer* server;
        EventListener* eventListener;
};

#endif