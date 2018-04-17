# Import the necessary python libraries

from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
import settings

#Initialize camera and grab a reference to the raw camera capture
def vision_init():    
    res_width = 300
    res_length = 300
    fr = 30

    camera = PiCamera()
    camera.resolution = (res_width, res_length)
    camera.framerate = fr
    rawCapture = PiRGBArray(camera, size=(res_width, res_length))

    #set margins for HSV (using HSV ColorPicker)
    blackUpper = (360,360,40) #we want to isolate the color black
    blackLower = (0,0,0)

    numSamples = 100
    sum = 0
    avg = 0
    sample = 0 

    #allow camera to warm-up
    time.sleep(0.1)

# Capture rames from the camera
def frame_capture():
    
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

	    start_time = time.time()
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
	
	    contours = cv2.findContours(mask.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[-2]

        if len(contours) > 0:
		c = max(contours, key = cv2.contourArea)
		((x,y), radius) = cv2.minEnclosingCircle(c)
	
		if radius > 20:
			settings.ref_x = int(x)
            settings.ref_y = int(y)

	    rawCapture.truncate(0)
	
