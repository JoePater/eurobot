import location, move, maths

goto_drive_speed = 50
goto_max_deviation = 10
differential_constant = 1
arrived_dist = 15

orientation_deviation = 1
orientation_constant = 2

#angle of p2 from p1
def get_bearing(p1,p2):
    b = maths.atan((p2.y - p1.y)/(p2.x - p1.x))
    if p2.x < p1.x:
        b += 180
    return b % 360

def angle_diff(a,b):
    diff = a - b
    while diff > 180:
        diff -= 360
    while diff <= -180:
        diff += 360
    return diff

def check_arrived(curr,target,bearing):
    return (curr.dist(target) < arrived_dist)

#increase right, decrease left
def calculate_differential(target):
    curr_ang = location.bearing()
    tar_ang = get_bearing(location.position(),target)
    diff = angle_diff(tar_ang,curr_ang)
    
    dist = target.dist(location.position())

    differential = differential_constant * diff / dist
    return differential

"""
Go to a point in a straight line
"""
def goto(target):
    curr = location.position()
    b0 = b = get_bearing(curr,target)

    #face towards target
    route.orientation(b)

    #while not arrived
    while not check_arrived(curr,target,b0):
        d = calculate_differential(target)
        move.drive(goto_drive_speed - d,goto_drive_speed + d)

    #stop driving
    move.drive(0,0)

def rotate_arrived(bearing):
    curr_b = location.bearing()
    diff = bearing - curr_b

    return math.fabs(diff) < orientation_deviation

def rotate_speed(bearing):
    curr_b = location.bearing()
    diff = angle_diff(bearing,curr_b)

    x = diff * orientation_constant
    
    if math.fabs(x) > max_rotate_speed:
        if x > 0:
            return max_rotate_speed
        return -max_rotate_speed

    return x

"""
Rotate to a bearing
"""
def orientation(bearing):
    curr_b = location.bearing()
    diff = angle_diff(bearing,curr_b)

    while not rotate_arrived(bearing):
        move.rotate(rotate_speed(bearing))

    move.drive(0)
