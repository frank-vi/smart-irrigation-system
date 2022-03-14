#!/usr/bin/env python
# -*- coding: utf-8 -*-
# pylint: disable=W0613, C0116
# type: ignore[union-attr]
# Marco Baldassarri <marco.baldassarri2@studio.unibo.it>
# Francesco Vignola <francesco.vignola@studio.unibo.it>

"""
Surveillance Bot launch script.
Bot Name : @SmartCityUniboBot
"""
import runpy

runpy.run_path('surveillance_bot/main.py', run_name='__main__')
