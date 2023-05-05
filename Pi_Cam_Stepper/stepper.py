from time import sleep
import RPi.GPIO as GPIO
class stepper:
    def __init__(self,p,d):
        self.pp = p
        self.dp = d
        self.position = 0
        self.increment = 0
        self.exe_complete_flag=0
        self.pulse_output_flag=0
        self.direction = False
        self.interval = 0
        self.p_state = False
        self.d_state = False
    
    def DRVI(self,p,f):
        if (f == 0 or p == self.position):
            self.exe_complete_flag = True
            self.pulse_output_flag = False
            return
        else:
            self.exe_complete_flag = False

        self.interval = 1.0 / abs(f) * 0.5
        self.d_state = False if p - self.position < 0 else True
        #self.p_state = not self.p_state

        GPIO.output(self.pp, GPIO.HIGH)
        sleep(self.interval)
        GPIO.output(self.pp, GPIO.LOW)
        sleep(self.interval)

        GPIO.output(self.dp, self.d_state)

        self.position = self.position - 1 if p - self.position < 0 else  self.position + 1
        
        self.pulse_output_flag = True
        #print(self.position)
        
        
        
        