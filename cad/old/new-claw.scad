module gear (
	mm_per_tooth    = 3,    //this is the "circular pitch", the circumference of the pitch circle divided by the number of teeth
	number_of_teeth = 11,   //total number of teeth around the entire perimeter
	thickness       = 6,    //thickness of gear in mm
	hole_diameter   = 3,    //diameter of the hole in the center, in mm
	twist           = 0,    //teeth rotate this many degrees from bottom of gear to top.  360 makes the gear a screw with each thread going around once
	teeth_to_hide   = 0,    //number of teeth to delete to make this only a fraction of a circle
	pressure_angle  = 28,   //Controls how straight or bulged the tooth sides are. In degrees.
	clearance       = 0.0,  //gap between top of a tooth on one gear and bottom of valley on a meshing gear (in millimeters)
	backlash        = 0.0   //gap between two meshing teeth, in the direction along the circumference of the pitch circle
) {
	assign(pi = 3.1415926)
	assign(p  = mm_per_tooth * number_of_teeth / pi / 2)  //radius of pitch circle
	assign(c  = p + mm_per_tooth / pi - clearance)        //radius of outer circle
	assign(b  = p*cos(pressure_angle))                    //radius of base circle
	assign(r  = p-(c-p)-clearance)                        //radius of root circle
	assign(t  = mm_per_tooth/2-backlash/2)                //tooth thickness at pitch circle
	assign(k  = -iang(b, p) - t/2/p/pi*180) {             //angle to where involute meets base circle on each side of tooth
		difference() {
			for (i = [0:number_of_teeth-teeth_to_hide-1] )
				rotate([0,0,i*360/number_of_teeth])
					linear_extrude(height = thickness, center = true, convexity = 10, twist = twist)
						polygon(
							points=[
								[0, -hole_diameter/10],
								polar(r, -181/number_of_teeth),
								polar(r, r<b ? k : -180/number_of_teeth),
								q7(0/5,r,b,c,k, 1),q7(1/5,r,b,c,k, 1),q7(2/5,r,b,c,k, 1),q7(3/5,r,b,c,k, 1),q7(4/5,r,b,c,k, 1),q7(5/5,r,b,c,k, 1),
								q7(5/5,r,b,c,k,-1),q7(4/5,r,b,c,k,-1),q7(3/5,r,b,c,k,-1),q7(2/5,r,b,c,k,-1),q7(1/5,r,b,c,k,-1),q7(0/5,r,b,c,k,-1),
								polar(r, r<b ? -k : 180/number_of_teeth),
								polar(r, 181/number_of_teeth)
							],
 							paths=[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]]
						);
			cylinder(h=2*thickness+1, r=hole_diameter/2, center=true, $fn=20);
		}
	}
};	
//these 4 functions are used by gear
function polar(r,theta)   = r*[sin(theta), cos(theta)];                            //convert polar to cartesian coordinates
function iang(r1,r2)      = sqrt((r2/r1)*(r2/r1) - 1)/3.1415926*180 - acos(r1/r2); //unwind a string this many degrees to go from radius r1 to radius r2
function q7(f,r,b,r2,t,s) = q6(b,s,t,(1-f)*max(b,r)+f*r2);                         //radius a fraction f up the curved side of the tooth 
function q6(b,s,t,d)      = polar(d,s*(iang(b,d)+t));                              //point at radius d on the involute curve

//a rack, which is a straight line with teeth (the same as a segment from a giant gear with a huge number of teeth).
//The "pitch circle" is a line along the X axis.
module rack (
	mm_per_tooth    = 3,    //this is the "circular pitch", the circumference of the pitch circle divided by the number of teeth
	number_of_teeth = 11,   //total number of teeth along the rack
	thickness       = 6,    //thickness of rack in mm (affects each tooth)
	height          = 120,   //height of rack in mm, from tooth top to far side of rack.
	pressure_angle  = 28,   //Controls how straight or bulged the tooth sides are. In degrees.
	backlash        = 0.0   //gap between two meshing teeth, in the direction along the circumference of the pitch circle
) {
	assign(pi = 3.1415926)
	assign(a = mm_per_tooth / pi) //addendum
	assign(t = a*cos(pressure_angle)-1)         //tooth side is tilted so top/bottom corners move this amount
		for (i = [0:number_of_teeth-1] )
			translate([i*mm_per_tooth,0,0])
				linear_extrude(height = thickness, center = true, convexity = 10)
					polygon(
						points=[
							[-mm_per_tooth * 3/4,                 a-height],
							[-mm_per_tooth * 3/4 - backlash,     -a],
							[-mm_per_tooth * 1/4 + backlash - t, -a],
							[-mm_per_tooth * 1/4 + backlash + t,  a],
							[ mm_per_tooth * 1/4 - backlash - t,  a],
							[ mm_per_tooth * 1/4 - backlash + t, -a],
							[ mm_per_tooth * 3/4 + backlash,     -a],
							[ mm_per_tooth * 3/4,                 a-height],
						],
						paths=[[0,1,2,3,4,5,6,7]]
					);
};


/*
translate([0,0,12.8])rotate([0,45,0])gear(5,19,3,0);
difference(){
    translate([-1,0,0])scale([1/sqrt(2),1,1])cylinder(h=30,r=13.7,$fn=80);
    translate([0,0,11])rotate([0,45,0])
    translate([0,0,20])cube([60,60,40],center=true);
}
*/
module semicircle(r1,r2){
    difference(){
        circle(r=r2,$fn=50);
        translate([-r2,-r2])square([2*r2,r2]);
        circle(r=r1,$fn=50);
    }
}
module arc(r1,r2,x){
    intersection(){
        semicircle(r1,r2);
        rotate(180+x)semicircle(r1,r2);
    }
}

module claw(x=0){
    difference(){
        union(){
            //translate([0,0,4])rotate([0,0,x])gear(4,24,8,0);
            translate([0,-7,0])rotate([0,-70,15])cube([6,7,74]);
            translate([-70-20,-21,25])cube([40,8,20]);
        }
        translate([-70,15.5,0])cylinder(r=34,h=50,$fn=80);
        translate([-69,-21-8,19])cube([40,8,20]);
        translate([-55,-20,9.8])rotate([0,-70,15])cube([10,20,50]);
        cylinder(r=2.25,h=8,$fn=20);
    }
}
module motorHole(x=0.1){
    difference(){
        circle(r=1.45+x,$fn=30);
        translate([0.9+1.5+2*x,0])square([3,3],center=true);
    }
}

translate([-70,15.5,0])cylinder(r=31,h=50,$fn=80);
translate([-38,-40,0])cube([22,80,30]);
claw();
/*translate([0,31,0])mirror([0,1,0])claw(180/24);
translate([0,50.5,0])rotate([0,0,0])difference(){
    translate([0,0,4])gear(4,6,8,0);
    linear_extrude(8)motorHole(0.15);
}*/
