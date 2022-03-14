#include <Arduino.h>
#include "BLEDevice.h"
#include "SerialComm.h"

#define EnvironmentalService  "181a"
#define BatteryService        "180f"
#define WeatherService        "05b35eee-baf4-44c9-b5a0-f07a6b4ece7a"
#define SoilService           "253b1e1f-5c26-4503-a1fe-a639dd09a01b"
#define IrrigationService     "03ff88af-a16a-4444-9ba5-81e563adfcce"

#define UserDescriptor              "2901"
#define HumidityCharacteristic      "2a6f"
#define TemperatureCharacteristic   "2a6e"
#define BatteryCharacteristic       "2a19"
#define SoilMoistureCharacteristic  "2e4288d0-6e8c-4521-9dec-c189e93438ce"
#define RainCharacteristic          "fb38da7a-8c18-4ba2-b1a8-17889f5e7f84"
#define LightCharacteristic         "b2cdbfab-714e-4fe2-bb45-59de64f6112e"
#define IrrigationCharacteristic    "feda4fab-5b29-4178-9864-58b677cac98d"

#define SERVER_ADDRESS              "24:6f:28:83:66:8a"


bool doScan = false;
bool doConnect = false;
bool connected = false;
BLEAdvertisedDevice* myDevice;
BLERemoteCharacteristic* irrigationRemoteCharacteristic;

void notifyCallback(
    BLERemoteCharacteristic* pBLERemoteCharacteristic,
    uint8_t* pData,
    size_t length,
    bool isNotify) {
      
    String characteristicUUID = String(pBLERemoteCharacteristic->getUUID().toString().c_str());
    String shortUUID = characteristicUUID.substring(4, 8);

    if (shortUUID == TemperatureCharacteristic) {
        int temperature = *(int*)pData;
        SerialComm::send("temperature: ", temperature / 100.0);
    } else if (shortUUID == HumidityCharacteristic) {
        int rawData = *(int*)pData;
        float humidity = rawData / 100.0;
        SerialComm::send("humidity: ", humidity);
    } else if (shortUUID == BatteryCharacteristic) {
        int batteryLevel = *(int*)pData;
        SerialComm::send("battery-level: ", batteryLevel);
    } else if (characteristicUUID == RainCharacteristic) {
        char* rainfall = (char*)pData;
        SerialComm::send("rainfall: ", rainfall);
    } else if (characteristicUUID == LightCharacteristic) {
        char* lightIntensity = (char*)pData;
        SerialComm::send("light-intensity: ", lightIntensity);
    } else if (characteristicUUID == SoilMoistureCharacteristic) {
        char* soilMoisture = (char*)pData;
        SerialComm::send("soil-moisture: ", soilMoisture);
    } else if (characteristicUUID == IrrigationCharacteristic) {
        char* irrigationStatus = (char*)pData;
        String v = String(irrigationStatus);
        v.replace('0', ' ');
        v.trim();
        SerialComm::send("irrigation:", v.c_str());
    }
}

class ClientConnectionListener : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    SerialComm::println("onDisconnect");
  }
};

void listenNotification(BLEClient* pclient) {
    BLERemoteService* environmentalRemoteService = pclient->getService(EnvironmentalService);
    BLERemoteService* batteryRemoteService = pclient->getService(BatteryService);
    BLERemoteService* weatherRemoteService = pclient->getService(WeatherService);
    BLERemoteService* soilRemoteService = pclient->getService(SoilService);
    BLERemoteService* irrigationRemoteService = pclient->getService(IrrigationService);

    BLERemoteCharacteristic* temperatureRemoteCharacteristic = environmentalRemoteService->getCharacteristic(TemperatureCharacteristic);
    BLERemoteCharacteristic* humidityRemoteCharacteristic = environmentalRemoteService->getCharacteristic(HumidityCharacteristic);
    BLERemoteCharacteristic* batteryRemoteCharacteristic = batteryRemoteService->getCharacteristic(BatteryCharacteristic);
    BLERemoteCharacteristic* rainRemoteCharacteristic = weatherRemoteService->getCharacteristic(RainCharacteristic);
    BLERemoteCharacteristic* lightRemoteCharacteristic = weatherRemoteService->getCharacteristic(LightCharacteristic);
    BLERemoteCharacteristic* soilMoistureRemoteCharacteristic = soilRemoteService->getCharacteristic(SoilMoistureCharacteristic);
    irrigationRemoteCharacteristic = irrigationRemoteService->getCharacteristic(IrrigationCharacteristic);

    temperatureRemoteCharacteristic->registerForNotify(notifyCallback);
    humidityRemoteCharacteristic->registerForNotify(notifyCallback);
    batteryRemoteCharacteristic->registerForNotify(notifyCallback);
    rainRemoteCharacteristic->registerForNotify(notifyCallback);
    lightRemoteCharacteristic->registerForNotify(notifyCallback);
    soilMoistureRemoteCharacteristic->registerForNotify(notifyCallback);
    irrigationRemoteCharacteristic->registerForNotify(notifyCallback);
}

void connectToServer() {
    SerialComm::send("Forming a connection to ", myDevice->getAddress().toString().c_str());
    
    BLEClient*  pclient  = BLEDevice::createClient();
    SerialComm::println(" - Created client");

    pclient->setClientCallbacks(new ClientConnectionListener());

    pclient->connect(myDevice);
    SerialComm::println(" - Connected to server");

    listenNotification(pclient);
    connected = true;
}


class AdvertisingListener: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String deviceDeatils = advertisedDevice.toString().c_str();
    String deviceAddress = advertisedDevice.getAddress().toString().c_str();
    SerialComm::println("BLE Advertised Device found: " + deviceDeatils);

    if (deviceAddress == SERVER_ADDRESS) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  BLEDevice::init("Control-Unit-Irrigation");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisingListener());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

  serialInput.reserve(20);
}


void loop() {
  if (doConnect) {
    connectToServer();
    doConnect = false;
  }

  if (inputAvailable && connected) {
    irrigationRemoteCharacteristic->writeValue(serialInput.c_str());
    Serial.println(serialInput.c_str());
    serialInput.clear();
    inputAvailable = false;
  }
  
  if(doScan && !connected){
    BLEDevice::getScan()->start(0);
  }

  serialEventRun();
}
