# Smart Irrigation System

The project was developed in the "Smart City e Tecnologie Mobili" (at Unibo) course and in the context of Smart Agriculture.

The goal is to automate crop irrigation based on soil and atmosphere characteristics in order to reduce water waste.

The project is divided into three modules:
- micro-irrigation: edge module for monitoring and controlling environmental and soil parameters; for this purpose the ESP32 microcontroller with the appropriate sensors and actuators was used.
- control-unit-irrigation: fog module that acts as a mediator between the edge and cloud levels to guarantee private access to the network of edge devices; bi-directional edge-to-cloud communication is organized into two channels: an edge-fog BLE channel and a cloud-fog TCP/IP channel. The device used at the physical level to implement the mediator is the Raspberry.
- cloud-backend: the brain of the entire system that determines the correct action through the analysis of the collected data; the cloud technologies adopted are AWS Lambda and DynamoDB.
