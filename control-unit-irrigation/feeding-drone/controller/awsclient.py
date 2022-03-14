import time
from awsiotcore import aws
from controller import drone
from abc import ABCMeta, abstractmethod

SUBSCRIBE_TOPIC = "drone/command"
DRONE_STATUS_TOPIC = "drone/status"
DRONE_FLY_TOPIC = "drone/fly"

ENDPOINT = "a2czsk7uhveyq3-ats.iot.us-west-2.amazonaws.com"
CLIENT_ID = "drone_device"

class MqttClient(metaclass = ABCMeta):
  @staticmethod
  @abstractmethod
  def notify(observable, *args, **kwargs):
    """Receive notifications from MQTT Subscriber"""

class MqttClientImpl(MqttClient):
  def __init__(self):
    self.aws = aws.AWSInteractionImpl(ENDPOINT, CLIENT_ID)
    self.aws.connect()
    self.aws.subscribe_topic(SUBSCRIBE_TOPIC)
    self.aws.subscribe_observer(self)

  def reconnect(self):
    self.aws.connect()
    
  def publish_drone_status(self, battery_level, error):
    message = {
          "drone" : {
            "takeoff_success": None,
            "error": error,
            "battery":battery_level,
            "connected": True
          }
        }
    self.aws.publish(DRONE_STATUS_TOPIC, message)

  def publish_drone_took_off_success(self, success, battery_level):
    message = {
          "drone" : {
            "takeoff_success": success,
            "error": None,
            "battery": battery_level,
            "connected": True
          }
        }
    self.aws.publish(DRONE_FLY_TOPIC, message)

  def publish_drone_not_connected(self):
    print("Sending drone not connected")
    message = {
          "drone" : {
            "takeoff_success": None,
            "error": None,
            "battery": None,
            "connected": False  # Not properly connected to Wifi network
          }
        }
    self.aws.publish(DRONE_FLY_TOPIC, message)

  def disconnect_broker(self):
    self.aws.disconnect()

  def notify(self, observable, *args, **kwargs):
    if (args[0]['drone'] == "start"):
      drone.DroneImpl(self)


