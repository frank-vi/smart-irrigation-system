# Irrigation System
Irrigation System module. The main purpose of this part of the system is to enable Bluetooth Low Energy communication between the Gateway (i.e. BLE Client raspberry Pi) and its Edge (i.e. BLE Server ESP32). 

When this script runs, it subscribes to all the Characteristics Notify property and start receiving data from the Edge attached sensors. It also interacts with the AWS IoT Core and subscribes to specific gateway topics, allowing to receive the irrigation command and send it back to the Edge via BLE.
This module was developed for gathering/sending data using BLE communication as well as for temporarily storing received measurements to the SQLite3 database.

The data exchanged between the Server and the Client is the following:

- Temperature
- Humidity
- Soil Moisture
- Light Intensity
- Rain status
- Device Battery status

## Install:
Please clone this repo and than run:
```
$ pip3 install bleak
```
Sqlite3 library for python is not needed since it has been included in the python standard library (>= python 2.5)


If you prefer, you can install all the requirements before run, possibly in a dedicate python3 virtual environment:
```
$ pip3 install -r requirements.txt
```
Those requirements will be very useful for the other Python Smart City Project modules.

## Run:
```
$ python3 start.py
```

Smart Agriculture - University of Bologna
