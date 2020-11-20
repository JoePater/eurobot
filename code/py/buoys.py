import claw, move, revolver
"""
This file contains functions for manipulating buoys with the claw.
"""

approach_speed = 25
approach_grab_delay = 0.2
approach_timeout = 5

def approachBuoy():
    move.drive(approach_speed,approach_speed)

    start = time.time()
    
    while time.time() - start < approach_timeout:
        if claw.colour() != "blue":
            time.sleep(0.2)
            move.drive(0,0)
            return True
    return False

"""
The claw needs to be in the following state:
down,right way up,open
Leaves the claw i the same way.
There should be a buoy in the claw.
Return True on success and False on failure.
"""
def pickupBuoy():
    claw.grab(sync=True)
    if claw.color() == "blue":
        claw.release(sync=True)
        return False
    claw.lift(sync=True)
    revolver.grab(claw.color()=="red",sync=True)
    claw.release(sync=True)
    claw.goDown(sync=True)
    return True
        
"""
The claw needs to be in the following state:
down,upside down,open
Leaves the claw down,right way up,open
"""
def pickupEdgeBuoy():
    claw.grab(sync=True)
    if claw.color() == "blue":
        claw.release(sync=True)
        return False
    claw.liftHalfway(sync=True)
    claw.turnRightWayUp(sync=True)
    claw.lift(sync=True)
    revolver.grab(claw.color()=="red",sync=True)
    claw.release(sync=True)
    claw.goDown(sync=True)
    return True

"""
The claw needs to be:
down,right way up,open
Leaves the claw the same way
"""
def placeBuoy():
    claw.lift(sync=True)
    claw.grab(sync=True)
    revolver.release(sync=True)
    claw.goDown(sync=True)
    claw.release(sync=True)
    return True

"""
Prepare the claw so that it is at the right rotation
to approach a given buoy
"""
def prepare(edge=False):
    if edge:
        claw.turnUpsideDown(sync=True)
    else:
        claw.turnRightWayUp(sync=True)

