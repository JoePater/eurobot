import motors,servos

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
    if Count() == 0:
        return True
    return False

def currentFull():
    if holders[current] == "GREEN" or holders[current] == "RED":
        return True
    return False

def holderFull(i):
    if holders[i] == "" or holders[i] == "OPEN":
        return False
    return True

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

def goToIndex(i):
    turnRevolver(i - current)

def goToFreeSpace():
    i = findFreeSpace()
    if i is None:
        return False
    setHolder(i,"RELEASED")
    goToIndex(i)


def holderFull(i):
    if holders[i] == "" or holders[i] == "OPEN":
        return False
    return True


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

def goToFreeSpace():
    i = findFreeSpace()
    if i is None:
        return False
    goToIndex(i)

def goToColour(colour):
    for x in range(len(holder)):
        if holders[x] == colour:
            goToIndex(x)


def goToGreen():
    goToColour("GREEN")

def goToRed():
    gotToColour("RED")

def release():
    setHolder(current, "RELEASE")

def grab(isRed):
    setHolder(current, "GRABBED")
    if isRed is None:
        holders[current] = ""
    elif isRed:
        holders[current] = "RED"
    else:
        holders[current] = "GREEN"
