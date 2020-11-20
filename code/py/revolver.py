import motors,servos

"""
Functions for manipulating revolver
"""

# Either empty string (empty and closed), "OPEN", "RED" or "GREEN"
holders = ["","","",""]

# Index into holders
current = 0

def redCount():
    total = 0
    for h in holders:
        if h == "RED":
            total += 1

    return total

def freeCount():
    free_counter = 0 
    for holder in holders:
        if holder == "OPEN" or holder == "":
            free_counter = free_counter + 1
            
    return free_counter

def greenCount():
    greens = 0
    for holder in holders:
        if holder == "GREEN":
            greens += 1
    return greens

def count():
    return greenCount() + redCount()

def isFull():
    return (count() == 4)

def isEmpty():
    if count() == 0:
        return True
    return False

"""
True if currently active slot has a buoy in it
"""
def currentFull():
    if holders[current] == "GREEN" or holders[current] == "RED":
        return True
    return False

"""
True if holder i has a buoy in it
"""
def holderFull(i):
    if holders[i] == "" or holders[i] == "OPEN":
        return False
    return True

"""
Returns the index of a nearby empty holder, or None if all are full
"""
def findFreeSpace():
    if isFull() == True:
        return None
    if currentFull() == False:
        return current
    if not holderFull((current+1) % 4):
        return (current+1) % 4
    if not holderFull((current-1) % 4):
        return (current-1) % 4
    return (current+2) % 4

"""
Rotate to holder i
"""
def goToIndex(i):
    turnRevolver(i - current)

"""
Rotates to a nearby free space

True/False on success/failure
"""
def goToFreeSpace():
    i = findFreeSpace()
    if i is None:
        return False
    setHolder(i,"RELEASED")
    goToIndex(i)
    return True

"""
Rotates to a nearby buoy of the specified colour

True on success, False if there are no buoys of the specified colour
"""
def goToColour(colour):
    for x in range(len(holder)):
        if holders[x] == colour:
            goToIndex(x)
            return True
    return False

def goToGreen():
    return goToColour("GREEN")

def goToRed():
    return goToColour("RED")

"""
release current holder
"""
def release():
    servos.setHolder(current, "RELEASE")

"""
grab the current holder, setting its colour to red if isRed is True,
green if False, and empty+closed if None
"""
def grab(isRed):
    servos.setHolder(current, "GRABBED")
    if isRed is None:
        holders[current] = ""
    elif isRed:
        holders[current] = "RED"
    else:
        holders[current] = "GREEN"
