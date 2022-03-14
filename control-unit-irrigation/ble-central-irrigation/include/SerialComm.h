#ifndef SERIAL_COMM
#define SERIAL_COMM

#include <Arduino.h>

String serialInput = "";
bool inputAvailable = false;

namespace SerialComm {
    void println(String data) {
        Serial.println(data);
        Serial2.println(data);
    }

    void send(String data, double value) {
        String v = String(value);
        Serial.println(data + v);
        Serial2.println(data + v);
    }

    void send(String data, int value) {
        String v = String(value);
        Serial.println(data + v);
        Serial2.println(data + v);
    }

    void send(String data, const char* value) {
        Serial.println(data + value);
        Serial2.println(data + value);
    }
}

void serialEventRun() {
  while (Serial2.available()) {
    if (inputAvailable) {
      serialInput.clear();
      inputAvailable = false;
    }

    char inChar = (char)Serial2.read();
    serialInput += inChar;
    
    if (inChar == '\n') {
      inputAvailable = true;
    }
  }
}

#endif