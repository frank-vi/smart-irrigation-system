# Feeding Drone
This is part of Smart City project. The drone takes off automatically and executes the given commands to mock a agricolture field feeding.
The drone is being controlled by the Telegram Manager Bot. Please check out smart-city-bot_manager submodule for more infos.

Communication technologies: MQTT and AWS IoT Core.

## Install:
Please clone this repo and than run:

```
$ python setup.py install
```

**Note:** this code requires OpenCV-Python. If you don't have it installed, simply run:

```
pip install opencv-python
```

For more information on OpenCV-Python click [here](https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_tutorials.html).


If you prefer, you can install all the requirements needed for the other Python Smart City Project modules:
```
$ pip3 install -r requirements.txt
```
Also consider installing in a dedicate python3 virtual environment

## Run:
```
$ python3 start.py
```

Smart Agriculture - University of Bologna