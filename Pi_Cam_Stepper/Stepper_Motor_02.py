# Based on: https://www.raspberrypi.org/forums/viewtopic.php?t=242928\.
#
# Software to drive 4 wire stepper motor using a TB6600 Driver
# PRi - RPi 3B
#
# Route 3.3 VDC to the controller "+" input for each: ENA, PUL, and DIR
#
# Connect GPIO pins as shown below) to the "-" input for each: ENA, PUL, and DIR
#
#
from time import sleep
import RPi.GPIO as GPIO
import Stepper
#
PUL = 17  # Stepper Drive Pulses
DIR = 27  # Controller Direction Bit (High for Controller default / LOW to Force a Direction Change).
ENA = 22  # Controller Enable Bit (High to Enable / LOW to Disable).
# DIRI = 14  # Status Indicator LED - Direction
# ENAI = 15  # Status indicator LED - Controller Enable
#
# NOTE: Leave DIR and ENA disconnected, and the controller WILL drive the motor in Default direction if PUL is applied.
# 
GPIO.setmode(GPIO.BCM)
# GPIO.setmode(GPIO.BOARD) # Do NOT use GPIO.BOARD mode. Here for comparison only. 
#
GPIO.setup(PUL, GPIO.OUT)
GPIO.setup(DIR, GPIO.OUT)
GPIO.setup(ENA, GPIO.OUT)

#
print('PUL = GPIO 17 - RPi 3B-Pin #11')
print('DIR = GPIO 27 - RPi 3B-Pin #13')
print('ENA = GPIO 22 - RPi 3B-Pin #15')
#
print('Initialization Completed')
#
# Could have usesd only one DURATION constant but chose two. This gives play options.
duration = 160 # This is the duration of the motor spinning. used for forward direction
target = 0 # This is the duration of the motor spinning. used for reverse direction
#
delay = 0.001 # This is actualy a delay between PUL pulses - effectively sets the mtor rotation speed.
print('Speed set to ' + str(delay))
#
m1 = False
#
cycles = 10 # This is the number of cycles to be run once program is started.
cyclecount = 0 # This is the iteration of cycles to be run once program is started.
print('number of Cycles to Run set to ' + str(cycles))
#
#
stepper = Stepper.stepper(PUL,DIR)



def reverse():
    m1 = True
    GPIO.output(ENA, GPIO.HIGH)
    print('ENA set to HIGH - Controller Enabled')
    #
    sleep(.5) # pause due to a possible change direction
    GPIO.output(DIR, GPIO.HIGH)
    print('DIR set to HIGH - Moving Backward at ' + str(delay))
    print('Controller PUL being driven.')
    print('run status' + str(stepper.exe_complete_flag))
    while m1 == True:
        stepper.DRVA(target,delay)
        if(stepper.exe_complete_flag==1):
            m1 = False
    GPIO.output(ENA, GPIO.LOW)
    print('ENA set to LOW - Controller Disabled')
    sleep(.5) # pause for possible change direction
    return

while cyclecount < cycles:
    target = target + duration
    
    reverse()
    cyclecount = (cyclecount + 1)
    print('Number of cycles completed: ' + str(cyclecount))
    print('Number of cycles remaining: ' + str(cycles - cyclecount))
#
GPIO.cleanup()
print('Cycling Completed')
#