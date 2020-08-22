from bus import bus
import time

rot_addr = 50

def readRotary(n):
    bus.write_byte(rot_addr,n)
    return bus.read_byte(rot_addr)

def pollRotary(n):
    while 1:
        print(readRotary(n))
        time.sleep(1)
