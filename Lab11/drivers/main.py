#this is the controller

#import project-specific files
#from accelerometer import *
#from vision import *
from motor_controller import *
from pid import *
import settings
import cv2
import numpy as np
from picamera.array import PiRGBArray
from picamera import PiCamera

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
	        L,R = motor_setup()
		
	        ## add anti-windup?
	        ## add error bounds checking to discard values
	
		while True:
			threadLock.acquire()
			print('dont starve me------------------------')
			#determine what combination of motors to control
			if settings.error > settings.threshold_left:
				#go right
				print('RIGHT')
				move_right(L,R,settings.control)
			elif settings.error < settings.threshold_right:
				#go left
				print('LEFT')
				move_left(L,R,-1*settings.control)
			else:
				#go straight
				print('FORWARD')
				move_forwards(L,R,settings.control)
			threadLock.release()
			time.sleep(0.05)		

#PID Thread
class PID_Thread(threading.Thread):
	def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
	def run(self):
	        # run PID and update motor control value
	        p = PID(0.35,0.1,0.4)  #set constants for pid control (TBD)
	        p.setPoint(settings.setPoint)
	        while True:
			threadLock.acquire()
	        	settings.control = p.update(settings.ref)
             		settings.error = p.getError()
			print('PID value: ' + str(settings.control))
			print('error: ' + str(settings.error))
			print('ref: ' + str(settings.ref))
			threadLock.release()
			time.sleep(0.01)

#vision thread
class visionThread(threading.Thread):
	def __init__(self, threadID, name, counter):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.counter = counter
	def run(self):
		#Initialize camera and grab a reference to the raw camera capture
		res_width = 300
		res_length = 300
		fr = 10
		
		camera = PiCamera()
		camera.resolution = (res_width, res_length)
		camera.framerate = fr
		rawCapture = PiRGBArray(camera, size=(res_width, res_length))
		
		#set margins for HSV (using HSV ColorPicker)
		blackUpper = (360,360,40) #we want to isolate the color black
		blackLower = (0,0,0)
		
		#allow camera to warm-up
		time.sleep(0.1)
		
		# Capture rames from the camera
		for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
			
			threadLock.acquire()
		    	# Read a frame from the camera
			frame = frame.array
		
			# Convert to HSV (hue, saturation, value) color space
			frameHSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
			
			# Create a binary image where pixels that fall in range are white and the rest are black
			mask = cv2.inRange(frameHSV, np.asarray(blackLower), np.asarray(blackUpper))
			
			kernel_size = 5
			blur = cv2.GaussianBlur(frame,(kernel_size, kernel_size),0)
			# These transformations help clean up a noisy image
			mask = cv2.erode(mask, None, iterations=2)
			mask = cv2.dilate(mask, None, iterations=2)
			
			contours = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[-2]
			# Uncomment the line below to see the boundaries found by findContours
			cv2.drawContours(frame, contours, -1, (0,255,0), 3)
			cv2.drawContours(mask, contours, -1, (0,255,0), 3)
			
			if len(contours) > 0:
				c = max(contours, key = cv2.contourArea)
				((x,y), radius) = cv2.minEnclosingCircle(c)
			
				if radius > 20:
					cv2.circle(frame,(int(x),int(y)),int(radius), (0,255,255),2)
					cv2.circle(frame, (int(x), int(y)), 5, (0, 0, 255), -1)
					cv2.circle(mask,(int(x),int(y)),int(radius), (0,255,255),2)
					cv2.circle(mask, (int(x), int(y)), 5, (0, 0, 255), -1)
					settings.ref = int(x)
			
			# Display the frame in a window
			cv2.imshow('image', frame)
			cv2.imshow('mask', mask)
			
			rawCapture.truncate(0)
			threadLock.release()
			time.sleep(0.001)
		
		    # This code will quit the program if 'q' is pressed
			if cv2.waitKey(1) & 0xFF == ord('q'):
				break


#lock object for thread synchronization
threadLock = threading.Lock()

try:
	#initialize globals
	settings.init() #call once
	
	settings.ref = 150
	settings.control = 0
	settings.error = 0
	settings.threshold_left = 20
	settings.threshold_right = -20
	settings.setPoint = 150
	
	# Create new threads
	vision = visionThread(1, "Vision", 1)
	controller = PID_Thread(2, "Controller", 2)
	driver = motorControlThread(3, "Driver", 3)
	
	# Start new threads
	vision.start()
	controller.start()
	driver.start()
	
	vision.join()
	controller.join()
	driver.join()

except (KeyboardInterrupt):
	raise exit()


