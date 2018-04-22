#this is the controller

#import project-specific files
from accelerometer import *
from vision import *
from motor_controller import *
from pid import *
import settings

#python imports
import threading
import time

#define a function for each thread below

#accelerometer thread
class accelerometerThread(threading.Thread):
    def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
    def run(self):
        pass
		# run functions from acc file
		# return the values here

#motor control thread
class motorControlThread(threading.Thread):
	def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
	def run(self):
        #initialize motor driver
        motor_setup()

        ## add anti-windup?
        ## add error bounds checking to discard values

        while True:
            #determine what combination of motors to control
            if settings.error > settings.threshold_left:
                #go right
                move_right(settings.control)
            elif settings.error < settings.threshold_right:
                #go left
                move_left(settings.control)
            else:
                #go straight
                move_forwards(settings.control)

#PID Thread
class PID_Thread(threading.Thread):
	def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
	def run(self):
        # run PID and update motor control value
        p = PID(3.0,0.4,1.2)  #set constants for pid control (TBD)
        p.setPoint(settings.setPoint)
        while True:
             settings.control = p.update(settings.ref)
             settings.error = p.getError(p)

#vision thread
class visionThread(threading.Thread):
	def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
	def run(self):            
		# acquire and process frame
		vision_init()
        frame_capture()


#lock object for thread synchronization
threadLock = threading.Lock()

#initialize globals
settings.init() #call once

# Create new threads
vision = visionThread(1, "Vision", 1)
controller = PID_Thread(2, "Controller", 2)
driver = motorControlThread(3, "Driver", 3)

# Start new threads
vision.start()
controller.start()
driver.start()





