#import serial

import time
import logging
import asyncio
import serial_asyncio
import platform
import sys
import re
from boundary import database
from datetime import datetime

MAC_ADDRESS = "AC:67:B2:05:12:2A"
LIGHT_INTENSITY = 'light-intensity'
SOIL_MOISTURE = 'soil-moisture'
TEMPERATURE = 'temperature'
IRRIGATION = "irrigation"
HUMIDITY = 'humidity'
BATTERY = 'battery'
RAIN = 'rain'


class BluetoothAdapter:
    def __init__(self):
        self.db = database.Database()
        self.irrigation_status = None
        self.first = True

    def set_proxy(self, proxy):
        self.__proxy = proxy

    async def communicate_with_edge(self):
        if self.first:
            self.ser, self.writer = await serial_asyncio.open_serial_connection(url='/dev/ttyAMA0', baudrate=115200)
            self.first = False

        raw_line = await self.ser.readline()
        line = raw_line.decode('utf-8').rstrip()

        if line.startswith(TEMPERATURE):
            self.notify(TEMPERATURE, line)
        elif line.startswith(HUMIDITY):
            self.notify(HUMIDITY, line)
        elif line.startswith("battery-level:"):
            self.notify(BATTERY, line)
        elif line.startswith("rainfall:"):
            self.notify(RAIN, line)
        elif line.startswith(LIGHT_INTENSITY):
            self.notify(LIGHT_INTENSITY, line)
        elif line.startswith(SOIL_MOISTURE):
            self.notify(SOIL_MOISTURE, line)
        elif line.startswith(IRRIGATION):
            formatted_value = re.sub('^.+:', '', line)
            print("Prova", formatted_value)
            self.__proxy.forward_to_cloud(formatted_value)

    def notify(self, parameter, value):
        print(value)
        formatted_value = re.sub('^.+:', '', value)
        self.store_data((parameter, formatted_value, MAC_ADDRESS, datetime.timestamp(datetime.now())))

    def send_irrigation_command(self, value):
        value += '\n'
        bytes_to_send = bytearray(value.encode())
        self.writer.write(bytes_to_send)
        asyncio.run(self.writer.drain())

    def store_data(self, values):
        self.db.connect()
        self.db.save_data(values)
        self.db.disconnect()
