import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)

button_pins = {"CLAW_OPEN":1,"RIGHT_WAY_UP":2,"UPSIDE_DOWN":3,
               "LIFT_UP":4,"LIFT_DOWN":5}

for b in button_pins:
    GPIO.setup(button_pins[b],GPIO.IN,pull_up_down=GPIO.PUD_UP)

def buttonPressed(b):
    return not GPIO.input(button_pins[b])
