def dropBouy(location, colour):
    x = location.position()
    route.follow(route.find(x, location), sync = True)
    if location.bearing() == 00:
        move.rotate(??) # 180
    if route.arrived():
        count = lambda colour: return revolver.greenCount() if colour == "green" else revolver.redCount()
        for n in count:
        lambda colour: return revolver.goToGreen() if colour == "green" else revolver.goToRed()
        if revolver.finishedRotating():
            claw.turnRightWayUp()
            claw.grab()
            if claw.isGrabbed():
                claw.goDown()
                #claw.turnRightWayUp
                claw.turnUpsideDown()
                claw.release()
                return True
def startDrop():
    if revolver.isFull():
        if dropBouy((1650, 1065), green):
            if revolver.greenCount() == 0:
                dropBouy((1650, 1335), red)
                if revolver.isEmpty():
                    print("finished bouys")
                else:
                    print("failed")
    

    
    
