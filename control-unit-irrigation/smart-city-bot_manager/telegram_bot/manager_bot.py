#!/usr/bin/env python
# -*- coding: utf-8 -*-
# pylint: disable=W0613, C0116
# type: ignore[union-attr]
# Marco Baldassarri <marco.baldassarri2@studio.unibo.it>
# Francesco Vignola <francesco.vignola@studio.unibo.it>

"""
Smart City Manager Bot
Bot Name : @SmartCityUniboManagerBot
"""

import logging
from telegram import Update, ParseMode
from telegram.ext import Updater, CommandHandler, MessageHandler, Filters, CallbackContext
from controller import awsclient

BOT_TOKEN = '1495524204:AAHERoNaMvx1jw-81uWD5TBq9eezVxoVpiw'
USER_MARCO = "@MarcusPan"
USER_FRANCESCO = "@frank_vi" 

class ManagerBot:

    def __init__(self):
        self.updater = Updater(BOT_TOKEN, use_context=True)
        dispatcher = self.updater.dispatcher
        dispatcher.add_handler(CommandHandler("start", self.start, Filters.user(username=[USER_MARCO,USER_FRANCESCO])))
        dispatcher.add_handler(CommandHandler("start_feeding", self.start_feeding, Filters.user(username=[USER_MARCO,USER_FRANCESCO])))
        dispatcher.add_handler(CommandHandler("start_irrigation", self.start_irrigation, Filters.user(username=[USER_MARCO,USER_FRANCESCO])))
        dispatcher.add_handler(CommandHandler("help", self.help, Filters.user(username=[USER_MARCO,USER_FRANCESCO])))

        self.client = awsclient.MqttClientImpl(self)
        self.updater.start_polling() # Start the Bot
        self.updater.idle()

    # Enable logging
    logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.INFO)
    logger = logging.getLogger(__name__)

    def start(self, update: Update, context: CallbackContext) -> None:
        self.rep = update
        
        self.rep.message.reply_text(
            text="Welcome to Smart City Bot Manager\n"
                 "\n"
                 "These are the available commands:\n"
                 "\n"
                 "/start_feeding - Activates the drone and start feeding process \n"
                 "\n"
                 "/start_irrigation - Manually activates the water pump for starting irrigation \n"
                 "\n"
                 "/help - Shows a descriptive text about this bot \n")

    def start_feeding(self, update: Update, context: CallbackContext) -> None:
        self.rep.message.reply_text('Asking the Drone to take off...')
        self.client.publish_start_drone()

    def start_irrigation(self, update: Update, context: CallbackContext) -> None:
        self.rep.message.reply_text('Sending irrigation command...')
        self.client.publish_start_irrigation()

    def help(self, update: Update, context: CallbackContext) -> None:
            update.message.reply_text(
            text="Welcome to Smart City Bot Manager\n"
                 "\n"
                 "This bot is being used to _fly a feeding drone_ and to handle _irrigation system_\n"
                 "\n"
                 "Please locate the drone on the agricultural field corner and make sure all the sensors are up and running for a correct manual irrigation and ground feeding\n", parse_mode=ParseMode.MARKDOWN)

    def drone_take_off_ok(self, battery_level):
        self.rep.message.reply_text('Drone take off successfully! Current battery status is: *' + str(battery_level) + '*. Starting feeding now...', parse_mode=ParseMode.MARKDOWN)

    def drone_take_off_not_ok(self):
        self.rep.message.reply_text('Drone not started. Battery is low or Wifi signal is too weak, please double check it manually.')

    def drone_not_connected(self):
        self.rep.message.reply_text('Warning. Drone not connected to Wifi Network. Please manually check')

    def drone_error(self):
        self.rep.message.reply_text('Error. Drone encountered an internal motor issue. Feeding not completed. Please manually check') 

    def drone_job_done(self, battery_level):
        self.rep.message.reply_text('Drone feeding done! Job finished successfully. Battery level is: ' + str(battery_level))

    def irrigation_status(self, status):
        if(status == 'on'):
            self.rep.message.reply_text('Irrigation started successfully! You can relax!')
        elif(status == 'already-started'):
            self.rep.message.reply_text('Water Pump already started. Irrigation in progress. Please wait until it finish before triggering this command again')

