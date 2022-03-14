from awscrt import io, mqtt, auth, http
from awsiot import mqtt_connection_builder
import json

PATH_TO_CERT = "certificates/091b9e802b-certificate.pem.crt"
PATH_TO_KEY = "certificates/091b9e802b-private.pem.key"
PATH_TO_ROOT = "certificates/root.pem"

from abc import ABCMeta, abstractmethod

class AWSInteraction(metaclass=ABCMeta):
  @staticmethod
  @abstractmethod
  def subscribe_observer(observer):
    """ The subscribe method """

  @staticmethod
  @abstractmethod
  def unsubscribe(observer):
    """ The unsubscribe method """

  @staticmethod
  @abstractmethod
  def notify(observer):
    """The notify method """

class AWSInteractionImpl(AWSInteraction):
  def __init__(self, endpoint, client_id):
    # Spin up resources
    self.endpoint = endpoint
    self.client_id = client_id
    event_loop_group = io.EventLoopGroup(1)
    host_resolver = io.DefaultHostResolver(event_loop_group)
    self.client_bootstrap = io.ClientBootstrap(event_loop_group, host_resolver)
    self._observers = set()

  def subscribe_observer(self, observer):
    self._observers.add(observer)
  
  def unsubscribe(self, observer):
    self._observers.remove(observer)

  def notify(self, *args, **kwargs):
    for observer in self._observers:
      observer.notify(self, *args, **kwargs)

  def connect(self):
    self.mqtt_connection = mqtt_connection_builder.mtls_from_path(
                endpoint=self.endpoint,
                cert_filepath=PATH_TO_CERT,
                pri_key_filepath=PATH_TO_KEY,
                client_bootstrap=self.client_bootstrap,
                ca_filepath=PATH_TO_ROOT,
                client_id=self.client_id,
                clean_session=False,
                keep_alive_secs=6)
    print("Connecting to {} with client ID '{}'...".format(self.endpoint, self.client_id))
    # Make the connect() call
    connect_future = self.mqtt_connection.connect()
    # Future.result() waits until a result is available
    connect_future.result()
    print("Connected!")

  # Subscribe
  def subscribe_topic(self, topic):
    print("Subscribing to topic '{}'...".format(topic))
    self.subscribe_future, self.packet_id = self.mqtt_connection.subscribe(
        topic=topic,
        qos=mqtt.QoS.AT_MOST_ONCE,
        callback=self.on_message_received)

  # Callback when the subscribed topic receives a message
  def on_message_received(self, topic, payload, **kwargs):
    print("Received message from topic '{}': {}".format(topic, payload))
    subscribe_result = self.subscribe_future.result()
    self.notify(json.loads(payload))
  
  def publish(self, topic, message):
    self.mqtt_connection.publish(topic=topic, payload=json.dumps(message), qos=mqtt.QoS.AT_MOST_ONCE)
    print("Published: '" + json.dumps(message) + "' to the topic: " + topic)
  
  def disconnect(self):
    print("Disconnecting...")
    disconnect_future = self.mqtt_connection.disconnect()
    disconnect_future.result()