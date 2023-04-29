from time import sleep
import RPi.GPIO as GPIO
class stepper:
    def __init__(self,p,d):
        self.pp = p
        self.dp = d
        self.pos = 0
        self.exe_complete_flag=0
        self.pulse_output_flag=0
    def DRVA(self,target,delay):
        if(self.pos != target):
            GPIO.output(self.pp, GPIO.HIGH)
            sleep(delay)
            GPIO.output(self.pp, GPIO.LOW)
            sleep(delay)
            self.pos = self.pos +1
            self.exe_complete_flag = 0
        else:
            self.exe_complete_flag = 1
        return