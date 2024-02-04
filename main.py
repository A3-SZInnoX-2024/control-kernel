import time
import sys
from lib.robocontroller.robocontroller import RoboController
from utils.utils import Utils
import setup
import numpy as np
import ctypes
import os

utl = Utils("Main Prosses")
rc = RoboController()

utl.INFO ("Version:", sys.version)
count = 1
while True:
    rc.move(count, 0, np.pi)
    time.sleep(1)
    rc.Mec_Velocity (count, 1, 0)
    time.sleep(1)
    count += 1
