from machine import *


RPM_Pin = PWM(Pin(8))
RPM_Pin.duty_u16(32767)

def update_RPM:
    if(RPM_val > 10 && RPM_val < 10000): 
        RPM_Pin.freq(K*RPM_val)
tim = Timer(period=1000, mode=Timer.ONE_SHOT, callback=update_RPM)
tim.init(period=1000, mode=Timer.PERIODIC, callback=update_RPM)
