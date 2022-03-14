# -*- coding: utf-8 -*-

# ENV_SERVICE = "0000181a-1000-8000-00805f9b34fb"
    # TEMP_CHARACTERISTIC_UUID = "00002a6e-0000-1000-8000-00805f9b34fb"
    # HUM_CHARACTERISTIC_UUID = "00002a6f-0000-1000-8000-00805f9b34fb"
# BATTERY_SERVICE = "0000180f-1000-8000-00805f9b34fb"
    # BATTERY_CHARACTERISTIC_UUID = "00002a19-0000-1000-8000-00805f9b34fb"
# WEATHER_SERVICE = "05b35eee-baf4-44c9-b5a0-f07a6b4ece7a"
    # RAIN_CHARACTERISTIC_UUID = "fb38da7a-8c18-4ba2-b1a8-17889f5e7f84"
    # LIGHT_CHARACTERISTIC_UUID = "b2cdbfab-714e-4fe2-bb45-59de64f6112e"
# SOIL_SERVICE = "253b1e1f-5c26-4503-a1fe-a639dd09a01b"
    # SOIL_CHARACTERISTIC_UUID = "2e4288d0-6e8c-4521-9dec-c189e93438ce"
# IRRIGATION_SERVICE = "03ff88af-a16a-4444-9ba5-81e563adfcce"
    # IRRIGATION_CHARACTERISTIC_UUID = "feda4fab-5b29-4178-9864-58b677cac98d"
# IRRIGATION_CHARACTERISTIC_UUID = "03ff88af-a16a-4444-9ba5-81e563adfcce"

from binascii import hexlify
import time
import logging
import asyncio
import platform
import sys
from binascii import hexlify
from bleak import BleakClient
from bleak import _logger as logger
from boundary import database
from datetime import datetime

MAC_ADDRESS = "AC:67:B2:05:12:2A"
SOIL_CHARACTERISTIC_UUID = "2e4288d0-6e8c-4521-9dec-c189e93438ce"
LIGHT_CHARACTERISTIC_UUID = "b2cdbfab-714e-4fe2-bb45-59de64f6112e"
RAIN_CHARACTERISTIC_UUID = "fb38da7a-8c18-4ba2-b1a8-17889f5e7f84"
BATTERY_CHARACTERISTIC_UUID = "00002a19-0000-1000-8000-00805f9b34fb"
HUM_CHARACTERISTIC_UUID = "00002a6f-0000-1000-8000-00805f9b34fb" 
TEMP_CHARACTERISTIC_UUID = "00002a6e-0000-1000-8000-00805f9b34fb"

IRRIGATION_CHARACTERISTIC_UUID = "feda4fab-5b29-4178-9864-58b677cac98d"

CONNECTION_TIME = 5 # seconds

class Bluetooth():
    def __init__(self):
        self.db = database.Database()
        self.irrigation_status = None
        self.client = BleakClient(MAC_ADDRESS)
        
    def set_proxy(self, proxy):
        self.__proxy = proxy

    def soil_notification_handler(self, sender, data):
        soil = self.decode(data)
        print("{0}: {1}".format("Soil-moisture", soil))
        self.store_data(('soil-moisture', soil, MAC_ADDRESS, datetime.timestamp(datetime.now())))

    def light_notification_handler(self, sender, data):
        light = self.decode(data)
        print("{0}: {1}".format("Light-intensity", light))
        self.store_data(('light-intensity',light, MAC_ADDRESS, datetime.timestamp(datetime.now())))

    def rain_notification_handler(self, sender, data):
        rain = self.decode(data)
        print("{0}: {1}".format("Rain", rain))
        self.store_data(('rain', rain, MAC_ADDRESS, datetime.timestamp(datetime.now())))

    def humidity_notification_handler(self, sender, data):
        humidity = self._make_data_readable(data) / 100
        print("{0}: {1}".format("Humidity", humidity))
        self.store_data(('humidity', humidity, MAC_ADDRESS, datetime.timestamp(datetime.now())))

    def temperature_notification_handler(self, sender, data):
        temperature = self._make_data_readable(data) / 100
        print("{0}: {1}".format("Temperature", temperature))
        self.store_data(('temperature', temperature, MAC_ADDRESS, datetime.timestamp(datetime.now())))

    def battery_read_handler(self, data):
        battery = self._make_data_readable(data)
        print("{0}: {1}".format("Battery", battery))
        self.store_data(('battery', battery, MAC_ADDRESS, datetime.timestamp(datetime.now())))

    def irrigation_notification_handler(self, sender, data):
        status = str(data, 'utf-8')
        self.__proxy.forward_to_cloud(status)

    def _make_data_readable(self, data):
        return int.from_bytes(data, byteorder='little', signed=False)

    def decode(self, data):
        return float(data.decode()) / 100 

    def store_data(self, values):
        self.db.connect()
        self.db.save_data(values)
        self.db.disconnect()

    async def communicate_with_edge(self):
        try:
            if (not await self.client.is_connected()):
                await self.client.connect()
            await self.client.start_notify(SOIL_CHARACTERISTIC_UUID, self.soil_notification_handler)
            await self.client.start_notify(LIGHT_CHARACTERISTIC_UUID, self.light_notification_handler)
            await self.client.start_notify(RAIN_CHARACTERISTIC_UUID, self.rain_notification_handler)
            await self.client.start_notify(HUM_CHARACTERISTIC_UUID, self.humidity_notification_handler)
            await self.client.start_notify(TEMP_CHARACTERISTIC_UUID, self.temperature_notification_handler)
            await self.client.start_notify(IRRIGATION_CHARACTERISTIC_UUID, self.irrigation_notification_handler)
            battery = await self.client.read_gatt_char(BATTERY_CHARACTERISTIC_UUID)
            self.battery_read_handler(battery)

            await asyncio.sleep(CONNECTION_TIME)
            await self.client.stop_notify(SOIL_CHARACTERISTIC_UUID)
            await self.client.stop_notify(LIGHT_CHARACTERISTIC_UUID)
            await self.client.stop_notify(RAIN_CHARACTERISTIC_UUID)
            await self.client.stop_notify(HUM_CHARACTERISTIC_UUID)
            await self.client.stop_notify(TEMP_CHARACTERISTIC_UUID)
            await self.client.stop_notify(IRRIGATION_CHARACTERISTIC_UUID)
            
            if(self.irrigation_status is not None):
                await self._write_irrigation_command(self.irrigation_status)
                self.irrigation_status = None
        except:
            print("ERROR: Could not connect to peripheral!")

    def send_irrigation_command(self, value):
        self.irrigation_status = value

    async def _write_irrigation_command(self, value):
        try:
            bytes_to_send = bytearray(map(ord, value))
            await self.client.write_gatt_char(IRRIGATION_CHARACTERISTIC_UUID, bytes_to_send)
        except Exception as e:
            print("Write Exception: ", e)

    

    