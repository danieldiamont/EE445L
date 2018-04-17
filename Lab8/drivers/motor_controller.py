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
    leftPWM = GPIO.PWM(12, 0.5) #channel, frequency
    rightPWM = GPIO.PWM(35, 0.35)

    leftPWM.start(0) # start at 0% duty cycle
    rightPWM.start(0) # start at 0% duty cycle

def move_forwards(dc):
	#left motor forwards

    leftPWM.ChangeDutyCycle(dc)
	GPIO.output(Motor1B,GPIO.LOW)
	GPIO.output(Motor1E,GPIO.HIGH)

	#right motor forwards
    rightPWM.ChangeDutyCycle(dc)
	GPIO.output(Motor2B,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.HIGH)
 
def move_right(dc):
	#left motor forwards
	leftPWM.ChangeDutyCycle(dc)
	GPIO.output(Motor1B,GPIO.LOW)
	GPIO.output(Motor1E,GPIO.HIGH)
	#right motor stop
	rightPWM.ChangeDutyCycle(0)
	GPIO.output(Motor2B,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.LOW)

def move_left(dc):
	#left motor stop
	leftPWM.ChangeDutyCycle(0)
	GPIO.output(Motor1B,GPIO.LOW)
	GPIO.output(Motor1E,GPIO.LOW)
	#right motor high
	rightPWM.ChangeDutyCycle(dc)
	GPIO.output(Motor2B,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.HIGH)

def move_backwards(dc):
	leftPWM.ChangeDutyCycle(0)
	GPIO.output(Motor1B,GPIO.HIGH)
	GPIO.output(Motor1E,GPIO.HIGH)
	 
	rightPWM.ChangeDutyCycle(0)
	GPIO.output(Motor2B,GPIO.HIGH)
	GPIO.output(Motor2E,GPIO.HIGH)
 
def stop_motors():
	GPIO.output(Motor1E,GPIO.LOW)
	GPIO.output(Motor2E,GPIO.LOW)

def shutdown_motors():
    leftPWM.stop()
    rightPWM.stop()
    GPIO_cleanup()

