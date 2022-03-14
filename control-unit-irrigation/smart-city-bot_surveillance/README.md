# Surveillance Bot
This is part of Smart City project. This module intercepts any motion and sends a picture to the @SmartCityUniboBot.
From this bot it is also possible to get a picture o record a 5 seconds video.

## Install:
Please install requirements before run, possibly in a dedicate python3 virtual environment:
```
$ pip3 install -r requirements.txt
```
Those requirements will be very useful for the other Python Smart City Project modules.


## Run:
```
$ python3 start.py
```
Or use a systemctl script to run it forever. For instance:

```
$ systemctl start surveillance_bot.service
```

Smart Agriculture - University of Bologna
