import buttons

#functions for interacting with C++ position code

def position():
    pass

def bearing():
    pass

def opponent():
    pass

def init_team():
    #check switch, then set team_colour
    global team_colour

    if buttons.buttonPressed("TEAM_IS_BLUE"):
        team_colour = "BLUE"
    else:
        team_colour = "YELLOW"

    #notify C++ code

def team():
    return team_colour
