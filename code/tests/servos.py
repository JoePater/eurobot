from bus import bus

servos_addr = 41

current_byte = 0

def setServo(num,x):
    global current_byte
    if x:
        current_byte |= 1 << num
    else:
        current_byte &= ~(1 << num)
        
    bus.write_byte(servos_addr,current_byte)

# "GRABBED" or "RELEASED"
def setHolder(num,stat):
    setServo(num,stat=="GRABBED")

def raiseFlag(x=1):
    setServo(5,x)
    
