
from tellosdk import tello
import time

from abc import ABCMeta, abstractmethod

GOOD_WIFI_QUALITY = 40 # Reliable enough Wifi quality signal to fly
SAFE_BATTERY_STATUS = 40  # Reliable enough Battery level to fly

class Drone(metaclass = ABCMeta):
  @staticmethod
  @abstractmethod
  def notify(observable, *args, **kwargs):
    """Receive notifications from MQTT Subscriber"""

class DroneImpl(Drone):
  def __init__(self, aws_client):
    
    self.aws_client = aws_client
    self.drone = tello.Tello()
    self.drone.subscribe_observer(self)
    self.drone.connect()

  def notify(self, observable, *args, **kwargs):
    if (args[0] == "drone_not_connected"):
      self.aws_client.publish_drone_not_connected()
    elif(args[0] == "drone_connected"):
      self.start_feeding()
    elif(args[0] == "error"):
      self.aws_client.publish_drone_status(None, True)

  def start_feeding(self):
    if(self._really_took_off()):
      self.drone.forward(200)
      time.sleep(2)
      self.drone.cw(180)
      self.drone.forward(200)
      time.sleep(2)
      self.drone.cw(180)
      battery = self.drone.get_battery()
      self.drone.land()
      print("Battery Level: " + str(battery))
      self.aws_client.publish_drone_status(battery, False) 
    self.aws_client.disconnect_broker()

  def _really_took_off(self):
    battery_level = self.drone.get_battery()
    print("Battery Level: "+ str(battery_level))
    if(battery_level > SAFE_BATTERY_STATUS):
      self.drone.takeoff()
      time.sleep(2)
      fly_time = self.drone.get_flight_time() 
      print("Fly time: "+ str(fly_time))
      if(self._is_wifi_signal_good(self.drone) and (fly_time > 0)): 
        print("publish_drone_took_off_success TRUE")
        self.aws_client.publish_drone_took_off_success(True, battery_level) # notify drone successfully took off
        return True
      else: 
        print("publish_drone_took_off_success FALSE")
        self.aws_client.publish_drone_took_off_success(False, None) # notify drone could not take off
        return False
    else: 
      print("publish_drone_took_off_success FALSE")
      self.aws_client.publish_drone_took_off_success(False, None) # notify drone could not take off
      return False

  def _is_wifi_signal_good(self, drone):
    wifi_signal = int(self.drone.get_wifi())
    print("Wifi signal: " + str(wifi_signal))
    return True if(wifi_signal >= GOOD_WIFI_QUALITY) else False


