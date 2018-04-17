#this is the controller

import accelerometer
import vision
import motor_controller
import settings

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
        pass

#PID Thread
class PID_Thread(threading.Thread):
	def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
	def run(self):
        pass
		# run PID and update motor control value

#vision thread
class visionThread(threading.Thread):
	def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
	def run(self):            
		# acquire and process frame
		vision.init()
        vision.frame_capture()


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





