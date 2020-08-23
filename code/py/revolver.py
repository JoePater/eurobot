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
    if count() == 0:
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
    return True

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

def release():
    servos.setHolder(current, "RELEASE")

def grab(isRed):
    servos.setHolder(current, "GRABBED")
    if isRed is None:
        holders[current] = ""
    elif isRed:
        holders[current] = "RED"
    else:
        holders[current] = "GREEN"
