


green_buoy_number = 0
red_buoy_number = 0

claw_to_centre = 200
buoy_diameter = 75

green_start_location = Pt(510,claw_to_centre + buoy_diameter/2)
red_start_location = Pt(1080,claw_to_centre + buoy_diameter/2)

def dropoffGreens(ret_path):
    global green_buoy_number
    
    pt = green_start_location
    pt.y += buoy_diameter * green_buoy_number

    path.followPath(ret_path)
    while revolver.greenCount() > 0:
        route.goto(pt)
        route.orientation(maths.correctBearing(-90))

        revolver.goToGreen()
        buoys.placeBuoy()

        green_buoy_number += 1
        pt.y += buoy_diameter
    
    
