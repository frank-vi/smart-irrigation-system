
class Proxy:
    def __init__(self, ble, messenger):
        self.__ble = ble
        self.__messenger = messenger

    def forward_to_edge(self, command):
        self.__ble.send_irrigation_command(command)

    def forward_to_cloud(self, status):
        self.__messenger.publish_irrigation_status({'irrigation': status})