import abc
import json
from awsiotcore import aws

IRRIGATION_TELEMETRY = "irrigation/telemetry"
IRRIGATION_COMMAND = "irrigation/command"
IRRIGATION_STATUS_TOPIC = "irrigation/status"

ENDPOINT = "a2vlodv4jxozlb-ats.iot.us-east-1.amazonaws.com"
CLIENT_ID = "gateway-client"

class MqttClient(metaclass = abc.ABCMeta):
  @staticmethod
  @abc.abstractmethod
  def notify(observable, *args, **kwargs):
    """Receive notifications from MQTT Subscriber"""

class MqttClientImpl(MqttClient):
    def __init__(self):
        self.__subscriber = {}
        self.aws = aws.AWSInteractionImpl(ENDPOINT, CLIENT_ID)
        self.aws.connect()
        self.aws.subscribe_topic(IRRIGATION_COMMAND)
        self.aws.subscribe_observer(self)

    def set_proxy(self, proxy):
        self.__proxy = proxy

    def publish_telemetry(self, data):
        self.aws.publish(IRRIGATION_TELEMETRY, data)

    def publish_irrigation_status(self, status):
        self.aws.publish(IRRIGATION_STATUS_TOPIC, status)

    def notify(self, observable, *args, **kwargs):
        if (args[0]['irrigation'] == "on"):
            self.__proxy.forward_to_edge("on")
        elif (args[0]['irrigation'] == "off"):
            self.__proxy.forward_to_edge("off")