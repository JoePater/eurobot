import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

button_pins = {"CLAW_OPEN":5,"RIGHT_WAY_UP":6,"UPSIDE_DOWN":13,
               "LIFT_UP":19,"LIFT_DOWN":26}

for b in button_pins:
    GPIO.setup(button_pins[b],GPIO.IN,pull_up_down=GPIO.PUD_UP)

def buttonPressed(b):
    return not GPIO.input(button_pins[b])
