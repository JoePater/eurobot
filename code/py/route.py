import location
import move

goto_drive_speed = 50
goto_max_deviation = 10
differential_constant = 1

orientation_deviation = 1
orientation_constant = 2

def get_bearing(p1,p2):
    b = maths.atan((p2.y - p1.y)/(p2.x - p1.x))
    if p2.x < p1.x:
        b += 180
    return b % 360

def check_arrived(curr,target,bearing):
    return False

#increase right, decrease left
def calculate_differential(target):
    curr_ang = location.bearing()
    tar_ang = get_bearing(location.position(),target)
    diff = tar_ang - curr_ang
    dist = target.dist(location.position())

    differential = differential_constant * diff / dist
    return differential

def goto(target):
    curr = location.position()
    b0 = b = get_bearing(curr,target)
    
    route.orientation(b)

    
    while not check_arrived(curr,target,b0):
        d = calculate_differential(target)
        move.drive(goto_drive_speed - d,goto_drive_speed + d)

def rotate_arrived(bearing):
    curr_b = location.bearing()
    diff = bearing - curr_b

    return math.fabs(diff) < orientation_deviation

def rotate_speed(bearing):
    curr_b = location.bearing()
    diff = bearing - curr_b

    return diff * orientation_constant

def orientation(bearing):
    curr_b = location.bearing()
    diff = bearing - curr_b

    while not rotate_arrived(bearing):
        move.rotate(rotate_speed(bearing))
