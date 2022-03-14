#!/home/smartcity/virtualenv/bin python
# -*- coding: utf-8 -*-
# pylint: disable=W0613, C0116
# type: ignore[union-attr]

"""
Communication between gateway and edge
"""

import asyncio

from boundary.proxy import Proxy
from boundary.database import Database
from boundary.bluetooth import Bluetooth
from boundary.serial_ble import BluetoothAdapter
from boundary.awsclient import MqttClientImpl
from boundary.aggregator import DataAggregator
from boundary.collector import DataCollector


async def main():
    db = Database()
    #  ble = Bluetooth()
    ble = BluetoothAdapter()

    mqtt = MqttClientImpl()
    proxy = Proxy(ble, mqtt)
    mqtt.set_proxy(proxy)
    ble.set_proxy(proxy)
    collector = DataCollector(ble)
    aggregator = DataAggregator(db, mqtt)

    await asyncio.gather(collector.start(), aggregator.start())

if __name__ == "__main__":
    asyncio.run(main())
