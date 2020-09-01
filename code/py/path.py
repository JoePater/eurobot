import route
import buoys

buoy_path_list = [
    [Pt(0,0),Pt(0,10)],
    [Pt(3,0),Pt(0,3)],
    ]
    
buoy_orientation_list = [
    20,40
    ]

# p = list Pt objects
def followPath(p,orient=None):
    for pt in p:
        if not route.goto(pt):
            return False

    if not orient is None:
        route.orientation(orient)

    return True

def followGroundBuoy(buoy):
    if not followPath(buoy_path_list[buoy],buoy_orientation_list[buoy]):
        return False

    if not buoys.prepare():
        return False
    
    if not buoys.approachBuoy():
        return False
    
    if not buoys.pickupBuoy():
        return False
    
    return True
