# Import the necessary python libraries

from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2

#Initialize camera and grab a reference to the raw camera capture

res_width = 480
res_length = 320
fr = 30

camera = PiCamera()
camera.resolution = (res_width, res_length)
camera.framerate = fr
rawCapture = PiRGBArray(camera, size=(res_width, res_length))

#allow camera to warm-up
time.sleep(0.1)

# This code sets up a video feed from your system's default web cam
#cap = cv2.VideoCapture(0)
numSamples = 1000
sum = 0
avg = 0
sample = 0 
# Capture rames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

	start_time = time.time()
    # Read a frame from the camera
	image = frame.array

    # Display the frame in a window
    #cv2.imshow('frame', frame)
    #cv2.imshow('image', image)

	rawCapture.truncate(0)
	sample = sample + 1
	endTime = time.time() - start_time
	sum = sum + endTime
	
	if(sample == numSamples):
		avg = sum/numSamples
		print("\n\n--- Average is: %s seconds " % avg)
		break
    # This code will quit the program if 'q' is pressed
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

# Release system webcam and close all windows
cv2.destroyAllWindows()

time.sleep(1)

raise SystemExit
