
route.goto(buoy coodinates, False)

#done while moving
if revolver.freeCount > 0:
    if revolver.currentFull():
        revolver.goToFreeSpace()

#done while moving
if claw.isGrabbed == False:
    claw.grab()
claw.turnRightWayUp


#prepare claw
if claw.isUp() == False:
    claw.lift()


#rotate to face buoy (don't know exactly how to find direction of buoy)
if route.arrived():
    route.orientation()
    claw.goDown()
    claw.grab()
    claw.lift()
    revolver.grab(claw.color())
    claw.goDown()
    
