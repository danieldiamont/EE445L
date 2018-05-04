import RPi.GPIO as GPIO
import time
 
GPIO.setmode(GPIO.BOARD)

#make sure to set the correct GPIO pins according to SCH
Motor1A = 12
Motor1B = 18
Motor1E = 16
 
Motor2A = 35
Motor2B = 36
Motor2E = 22

#set up GPIO

def motor_setup():
	#setup motor one
	GPIO.setup(Motor1A,GPIO.OUT)
	GPIO.setup(Motor1B,GPIO.OUT)
	GPIO.setup(Motor1E,GPIO.OUT)
	#setup motor two
	GPIO.setup(Motor2A,GPIO.OUT)
	GPIO.setup(Motor2B,GPIO.OUT)
	GPIO.setup(Motor2E,GPIO.OUT)

	#set up PWM modules
	leftPWM = GPIO.PWM(Motor1A, 50) #channel, frequency
	rightPWM = GPIO.PWM(Motor2A, 50)
	
	leftPWM.start(50) # start at 50% duty cycle
	rightPWM.start(50) # start at 50% duty cycle

	return leftPWM, rightPWM

def move_forwards(leftPWM,rightPWM,dc):
	#left motor forwards
	print('forward')
	if(dc > 95):
		dc = 95
	if(dc < 20):
		dc = 20

	leftPWM.ChangeDutyCycle(45)
	#GPIO.output(Motor1A,GPIO.HIGH)
	GPIO.output(Motor1B,GPIO.LOW)
	GPIO.output(Motor1E,GPIO.HIGH)

	#right motor forwards
	rightPWM.ChangeDutyCycle(45)
	#GPIO.output(Motor2A,GPIO.HIGH)
	GPIO.output(Motor2B,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.HIGH)
 
def move_right(leftPWM,rightPWM,dc):

	print('right')
	if(dc > 95):
		dc = 95
	if(dc < 20):
		dc = 20

	leftPWM.ChangeDutyCycle(10)
	#GPIO.output(Motor1A,GPIO.HIGH)
	GPIO.output(Motor1B,GPIO.LOW)
	GPIO.output(Motor1E,GPIO.HIGH)

	#right motor forwards
	#GPIO.output(Motor2A,GPIO.LOW)
	rightPWM.ChangeDutyCycle(dc)
	GPIO.output(Motor2B,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.HIGH)

def move_left(leftPWM,rightPWM,dc):

	print('left')
	if(dc > 95):
		dc = 95
	if(dc < 20):
		dc = 20

	leftPWM.ChangeDutyCycle(dc)
	GPIO.output(Motor1B,GPIO.LOW)
	GPIO.output(Motor1E,GPIO.HIGH)

	#right motor forwards
	rightPWM.ChangeDutyCycle(10)
	GPIO.output(Motor2B,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.HIGH)


def move_backwards(leftPWM,rightPWM,dc):
	leftPWM.ChangeDutyCycle(0)
	GPIO.output(Motor1B,GPIO.HIGH)
	GPIO.output(Motor1E,GPIO.HIGH)
	 
	rightPWM.ChangeDutyCycle(0)
	GPIO.output(Motor2B,GPIO.HIGH)
	GPIO.output(Motor2E,GPIO.HIGH)
 
def stop_motors():
	GPIO.output(Motor1E,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.LOW)

def shutdown_motors(leftPWM,rightPWM):
    leftPWM.stop()
    rightPWM.stop()
    GPIO_cleanup()
