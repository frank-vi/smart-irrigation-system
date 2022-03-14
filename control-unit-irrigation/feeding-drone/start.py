#!/home/smartcity/virtualenv/bin python
# -*- coding: utf-8 -*-
# pylint: disable=W0613, C0116
# type: ignore[union-attr]
# Marco Baldassarri <marco.baldassarri2@studio.unibo.it>
# Francesco Vignola <francesco.vignola@studio.unibo.it>

"""
Agricolture Drone launch script.
"""

import time
from controller import awsclient
from threading import Lock, Thread

def main():
  thread = Thread(target=_start_client, daemon=False)
  thread.start()
  _keep_running()

def _start_client():
  client = awsclient.MqttClientImpl()

def _keep_running():
  while(True):
    print("Still running...")
    time.sleep(3600) # 1 hour
    
if __name__ == '__main__':
    main()