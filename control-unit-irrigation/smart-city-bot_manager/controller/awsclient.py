import time
from awsiotcore import aws
from abc import ABCMeta, abstractmethod
from telegram_bot import manager_bot

DRONE_COMMAND_TOPIC = "drone/command"
SUBSCRIBE_DRONE_STATUS = "drone/status"
SUBSCRIBE_DRONE_FLY = "drone/fly"

IRRIGATION_COMMAND_TOPIC = "irrigation/command"
SUBSCRIBE_IRRIGATION_STATUS = "irrigation/status"

ENDPOINT = "a2vlodv4jxozlb-ats.iot.us-east-1.amazonaws.com" 
CLIENT_ID = "manager_bot"

class MqttClient(metaclass = ABCMeta):
  @staticmethod
  @abstractmethod
  def notify(observable, *args, **kwargs):
    """Receive notifications from MQTT Subscriber"""

class MqttClientImpl(MqttClient):
  def __init__(self, bot_manager_instance):
    self.bot = bot_manager_instance
    self.client = aws.AWSInteractionImpl(ENDPOINT, CLIENT_ID)
    self.client.connect()
    self.client.subscribe_topic(SUBSCRIBE_DRONE_STATUS)
    self.client.subscribe_topic(SUBSCRIBE_DRONE_FLY)
    self.client.subscribe_topic(SUBSCRIBE_IRRIGATION_STATUS)
    self.client.subscribe_observer(self)

  def publish_start_drone(self):
    message = {"drone" : "start"}
    self.client.publish(DRONE_COMMAND_TOPIC, message)

  def disconnect(self):
    self.client.disconnect()

  def notify(self, observable, *args, **kwargs):
    json = args[0]
    if('irrigation' in json):
      self.bot.irrigation_status(json['irrigation'])
    if('drone' in json):
      if(json['drone']['connected'] == True):
        if((json['drone']['takeoff_success'] == True)):
          self.bot.drone_take_off_ok(json['drone']['battery'])
        elif((json['drone']['takeoff_success'] == False)):
          self.bot.drone_take_off_not_ok()
        elif((json['drone']['error'] == None) and (json['drone']['battery'] != None)):
          self.bot.drone_job_done(json['drone']['battery'])
        elif((json['drone']['error'] == True) and (json['drone']['battery'] == None)):
          self.bot.drone_error()
      else: 
        self.bot.drone_not_connected()

  def publish_start_irrigation(self):
    message = {"irrigation" : "on"}
    self.client.publish(IRRIGATION_COMMAND_TOPIC, message)
