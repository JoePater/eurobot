{
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

//These 5 functions let the user find the derived dimensions of the gear.
//A gear fits within a circle of radius outer_radius, and two gears should have
//their centers separated by the sum of their pictch_radius.
function circular_pitch  (mm_per_tooth=3) = mm_per_tooth;                     //tooth density expressed as "circular pitch" in millimeters
function diametral_pitch (mm_per_tooth=3) = 3.1415926 / mm_per_tooth;         //tooth density expressed as "diametral pitch" in teeth per millimeter
function module_value    (mm_per_tooth=3) = mm_per_tooth / pi;                //tooth density expressed as "module" or "modulus" in millimeters
function pitch_radius    (mm_per_tooth=3,number_of_teeth=11) = mm_per_tooth * number_of_teeth / 3.1415926 / 2;
function outer_radius    (mm_per_tooth=3,number_of_teeth=11,clearance=0.1)    //The gear fits entirely within a cylinder of this radius.
	= mm_per_tooth*(1+number_of_teeth/2)/3.1415926  - clearance;              

}

module intHoles(){
    linear_extrude(5){
        translate([0,0])circle(r=2.25,$fn=20);
        translate([30,0])circle(r=2.25,$fn=20);
        translate([0,30])circle(r=2.25,$fn=20);
        translate([30,30])circle(r=2.25,$fn=20);
    }
}


module stand(pivDist = 90){
    difference(){
        union(){
            translate([55,10,0])cube([5,25,pivDist]);
            cube([60,45,5]);
        }
        translate([7.5,7.5,0])intHoles();
        translate([55,22.5,pivDist-5])rotate([0,90,0])
        cylinder(r=2.25,h=5,$fn=20);
    }
}

module partA1(a=30,b=30,c=26){
    //unused
    difference(){
        union(){
            cube([10,a+5+10,b]);
            translate([0,a+5,b-5])cube([c+5+5,25,5]);
        }
        translate([0,0,10])cube([10,a+5,b-10]);
        translate([5,5,0])cylinder(r=2.25,h=10,$fn=20);
    }
}
module partA2(){
    //????
    difference(){
        cube([30,25,10]);
        translate([15-6.2,0,5])cube([12.4,25,5]);
        translate([15-10,15,0])cylinder(h=10,r=2.25,$fn=20);
        translate([15+10,15,0])cylinder(h=10,r=2.25,$fn=20);
    }
}
module cone1(){
    cylinder(r1=8/2,r2=3.5/2,h=3,$fn=20);
}
module motorHole(x=0.15){
    //mini motor
    difference(){
        circle(r=1.45+x,$fn=30);
        translate([0.9+1.5+2*x,0])square([3,3],center=true);
    }
}
module partB1_(h=7,l=32,w=20,d=20){
    difference(){
        cube([l,w,h]);
        translate([6,0,h])rotate([-90,0,0])cylinder(h=w,r=2.1,$fn=30);
        translate([6,5,h])rotate([-90,0,0])cylinder(h=3.2,r=4.1,$fn=30);
        
        translate([d+6,0,h])rotate([-90,0,0])cylinder(h=w,r=2.1,$fn=30);
        translate([d+6,5,h])rotate([-90,0,0])cylinder(h=3.2,r=4.1,$fn=30);
        
        translate([16,10,0])cylinder(h=h,r=2.25,$fn=20);        
    }
}

module partB1(h=7,l=32,w=28,d=20){
    //claw pivot holder thing
    difference(){
        cube([l,w,h]);
        translate([6,0,h])rotate([-90,0,0])cylinder(h=w,r=2.1,$fn=30);
        translate([6,5,h])rotate([-90,0,0])cylinder(h=3.2,r=5,$fn=30);
        
        translate([d+6,0,h])rotate([-90,0,0])cylinder(h=w,r=2.1,$fn=30);
        translate([d+6,5,h])rotate([-90,0,0])cylinder(h=3.2,r=5,$fn=30);
        
        translate([16,22,0])cylinder(h=h,r=2.25,$fn=20);        
        translate([16,22,0])cone1();
    }
}

module partB2(h=7,l=32,w=20,d=20){
    //unused
    mirror([1,0,0])difference(){
        union(){
            linear_extrude(10)projection()translate([0,0,-0.01])
            partB1_(h,l,w,d);
            translate([0,0,10])partB1_(h,l,w,d);
        }
        translate([d+6+12-5.2,0,15-12])cube([10.4,w,12]);
    }
}
module gearSection(a,n,h,r=0){
    intersection(){
        translate([0,0,h/2])rotate([0,0,r])gear(3.13,n,h,0);
        linear_extrude(h)polygon(points=[[0,0],[100,0],
        [100*cos(a/5),100*sin(a/5)],
        [100*cos(2*a/5),100*sin(2*a/5)],[100*cos(3*a/5),100*sin(3*a/5)],
        [100*cos(4*a/5),100*sin(4*a/5)],[100*cos(a),100*sin(a)]]);
    }
}
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
module clawR(a=50,b=25){
    mirror([1,0,0])difference(){
        union(){
            rotate([0,0,-30])gearSection(65,20,6);
            translate([-4,-4,0])cube([8,a+4-15,6]);
            translate([-0.7,26,0])rotate([0,0,35]){
                cube([6,45,6]);
                translate([-5,25,0])cube([5,10,6]);
            }
            linear_extrude(6)translate([10,35+a,0])rotate(155)arc(36.8,39.8,65);
            translate([10,85,0])linear_extrude(11.5,scale=1+(18/72/10)){
                rotate([0,0,-27])translate([-33-5.5,-5,0])square([5.5,5]);
                rotate([0,0,-10])translate([-36-2.5,-2.5,0])square([2.5,5]);
                rotate([0,0,55])translate([-36-6,-2.5,0])square([6,5]);
            }
            translate([-21,-6,0])rotate([0,0,15])cube([25,25,6]);
            translate([4,14,1.5])rotate([0,90,0])cylinder(r=2.1,h=4,$fn=30);
        }
        cylinder(r=2.25,h=6,$fn=20);
        translate([0,b,0])cylinder(r=2.15,h=6,$fn=20);
        translate([-19,50,0])cylinder(r=1.75,h=6,$fn=20);
        translate([0,-8,0])cube([15,4,6]);
        translate([-16,-5,3])rotate([0,0,15])cube([12.3,26,3]);
        translate([0,0,-5])cube([50,50,5]);
    }
}
module clawL(a=50,b=25){
    difference(){
        union(){
            rotate([0,0,-30-180/20])gearSection(80,20,6);
            translate([-4,-4,0])cube([8,a+4-15,6]);
            translate([-0.7,26,0])rotate([0,0,35]){
                cube([6,45,6]);
                translate([-5,20,0])cube([5,10,6]);
            }
            linear_extrude(6)translate([10,35+a,0])rotate(155)arc(36.8,39.8,65);
            translate([10,85,0])linear_extrude(11.5,scale=1+(18/72/10)){
                rotate([0,0,-27])translate([-33-5.5,-5,0])square([5.5,5]);
                rotate([0,0,-10])translate([-36-2.5,-2.5,0])square([2.5,5]);
                rotate([0,0,55])translate([-36-6,-2.5,0])square([6,5]);
            }
            translate([4,14,1.5])rotate([0,90,0])cylinder(r=2.1,h=4,$fn=30);
        }
        cylinder(r=2.25,h=6,$fn=20);
        translate([0,b,0])cylinder(r=2.15,h=6,$fn=20);
        translate([-16,46,0])cylinder(r=1.75,h=6,$fn=20);
        translate([0,-8,0])cube([15,4,6]);
        translate([0,0,-5])cube([50,50,5]);
    }
}
module clawRa(){
    //pulley for claw
    difference(){
        union(){
            cylinder(r=2.5,h=8,$fn=30);
            translate([0,0,8])cylinder(r=6,h=2,$fn=30);
        }
        cylinder(r=1.75,h=10,$fn=30);
    }
}
module clawLa(){
    //winch thing
    mirror([1,0,0])difference(){
        union(){
            rotate_extrude($fn=50)difference(){
                union(){square([10,4.5]);
                square([12.5,2]);}
                translate([10.2,3])circle(r=1,$fn=20);
            }
            cylinder(r=3,h=6,$fn=30);
            translate([0,0,6])cylinder(r1=3,r2=7,h=3,$fn=30);
            translate([0,0,9])cylinder(r=7,h=0.5,$fn=30);
            translate([11,-5,0])rotate([0,-60,0])cube([3,5,4.5]);
            translate([8,2.5,3.8])rotate([0,45,0])cylinder(r=1,h=3,$fn=20);
        }
        linear_extrude(15)motorHole();
        translate([8,-7.5,0])cylinder(r=0.8,h=2,$fn=20);
        translate([8.8,0,3])cube([2,1.5,3]);
        translate([10.9,-5,3.5])rotate([0,-60,0])cube([2,5,2]);
    }
}
module littleMotor(){
    cube([12,25,10]);
    translate([6,25,5])rotate([-90,0,0])linear_extrude(10)motorHole();
}
{module partD(l){
    //unused
    difference(){
        translate([-4,-4,0])cube([8,l+8,3]);
        cylinder(r=2.15,h=3,$fn=20);
        translate([0,l,0])cylinder(r=2.15,h=3,$fn=20);
    }
}
module partC(l){
    //unused
    difference(){
        translate([-4,-3,0])cube([8,l+7,6]);
        translate([-4,3,3])cube([8,l+1,3]);
        linear_extrude(6)rotate(-90)motorHole();
        translate([0,l,0])cylinder(r=2.15,h=3,$fn=20);
    }
}

module partE(){
    //moving motor holder - unused
    difference(){
        cube([26,32,16]);
        translate([0,0,3])cube([22,32,1]);
        translate([0,15,4])cube([10,17,12]);
        translate([12,0,3])cube([12,32,10]);
        translate([5,27,0])cylinder(h=5,r=2.25,$fn=20);
        translate([5,7.5,0])cylinder(h=16,r=2.25,$fn=20);
    }
}

module partF(){
    //motor to screw adapter - unused
    difference(){
        cylinder(r=4,h=12,$fn=30);
        linear_extrude(6)motorHole();
        translate([0,0,6])cylinder(r=1.95,h=6,$fn=30);
    }
}

module partG(){
    //moving nut - unused
    difference(){
        cube([26,16,16]);
        translate([18,0,8])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=20);
        translate([18-3.5,10,4])cube([7,3.3,20]);
        translate([0,0,4])cube([10,25,12]);
        translate([5,5,0])cylinder(h=5,r=2.25,$fn=20);
    }
}

module partH(){
    //nut-gear
    difference(){
        translate([0,0,2.5])gear(5,18,5,0);
        
        translate([0,0,1.8])cylinder(r=3.95,h=3.2,$fn=6);
        cylinder(r=2.25,h=5,$fn=20);
    }
}
    
module sissorA(l=100,w=40){
    //H shape
    x=8;
    difference(){
        union(){
            cube([x,l+x,x]);
            translate([w-x,0,0])cube([x,l+x,x]);
            translate([0,x/2+l/2-20-x/2,0])cube([w,x,x]);
            translate([0,x/2+l/2+20-x/2,0])cube([w,x,x]);
        }
        translate([0,x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
    }
}
module sissorAx(l=100,w=40){
    //H with raised holes
    x=8;
    difference(){
        union(){
            cube([x,l+x,x]);
            translate([w-x,0,0])cube([x,l+x,x]);
            translate([0,x/2+l/2-20-x/2,0])cube([w,x,x]);
            translate([0,x/2+l/2+20-x/2,0])cube([w,x,x]);
            translate([0,0,8])cube([8,16,8]);
            translate([w-8,0,8])cube([8,16,8]);
        }
        translate([0,x/2,12])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,12,12])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
    }
}
module sissorB(l=100){
    //stick
    x=8;
    difference(){
        cube([x,l+x,x]);
        translate([0,x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
    }
}
module sissorBx(l=100){
    //raised stick
    x=8;
    difference(){
        union(){
            cube([x,l+x,x]);
            translate([0,0,x])cube([x,12,10]);
        }
        translate([0,6,3*x/2])rotate([0,90,0])cylinder(r=4,h=x,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
    }
}
module sissorC1(w=41){
    //unused
    x=8;
    difference(){
        cube([w+x,20,x]);
        translate([4,0,0])cube([w,10,x]);
        translate([4,10,4])cube([w,10,4]);
        translate([0,x/2,x/2])rotate([0,90,0])cylinder(h=w+x,r=2.25,$fn=20);
        translate([w/2-10+x/2,15,0])cylinder(h=w-x,r=2.25,$fn=20);
        translate([w/2+10+x/2,15,0])cylinder(h=w-x,r=2.25,$fn=20);
    }
}
module sissorC2(w=40){
    //unused
    x=8;
    difference(){
        cube([30,10,15]);
        translate([30/2-6.2,0,0])cube([12.4,10,9.7]);
        translate([5,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([25,5,0])cylinder(r=2.25,h=15,$fn=20);
    }
}
module sissorD(w=40){
    //unused
    x=8;
    difference(){
        cube([w,18,15]);
        translate([4,0,0])cube([w-8,10,15]);
        translate([w/2-3.45,10+3,3])cube([6.9,3.3,15]);
        translate([0,x/2,x/2])rotate([0,90,0])cylinder(h=w,r=2.25,$fn=20);
        translate([w/2,10,9])rotate([-90,0,0])cylinder(h=w,r=2.25,$fn=20);
    }
}
module partI1(a=15){
    //big gear for claw
    difference(){
        translate([0,0,-4])cylinder(r=10,h=12,$fn=40);
        translate([0,0,-4])cylinder(r=2.15,h=12,$fn=20);
    }
    difference(){
        union(){
            
            translate([0,0,4])gear(5,55,8,4.5);
        }
        cylinder(r=2.15,h=8,$fn=20);
        translate([-150,-150,0])cube([300,140,8]);
        translate([-150,-150,0])cube([140,300,8]);
        //translate([-1,a+5])circle(r=2.25,$fn=20);
        translate([-1,a+15,0])cylinder(r=2.25,h=8,$fn=20);
        translate([19,a+5,0])cylinder(r=2.25,h=8,$fn=20);
        translate([19,a+15,0])cylinder(r=2.25,h=8,$fn=20);
    }
}
module partI2(b=35+12,c=25.5){
    //ataches claw to lifting gear
    linear_extrude(8)difference(){
        translate([0,0])square([25+12,20]);
            
        //translate([5,5])circle(r=2.25,$fn=20);
        translate([12,15])circle(r=2.25,$fn=20);
        translate([20+12,5])circle(r=2.25,$fn=20);
        translate([20+12,15])circle(r=2.25,$fn=20);
    }
    cube([12,10,b-6-8]);
    difference(){
        translate([0,0,b-6-8])cube([36,25,6]);
        
        translate([5+c,0,b-8])rotate([-90,0,0])
        cylinder(r=2.15,h=25,$fn=25);
        translate([5+c,8,b-8])rotate([-90,0,0])
        cylinder(r=4,h=3.3,$fn=25);
        
        translate([22,5,b-8-6])cylinder(r=2.25,h=6,$fn=20);
        translate([22,20,b-8-6])cylinder(r=2.25,h=6,$fn=20);
       
    }
}
module partI3(c=25.5){
    //cap, holds motor and pivot for claw
    d = 19;
    difference(){
        cube([36,25,13]);
        
        translate([31-c,0,13])rotate([-90,0,0])
        cylinder(r=2.15,h=25,$fn=25);
        translate([31-c,8,13])rotate([-90,0,0])
        cylinder(r=4,h=3.3,$fn=25);
        
        translate([31-c+d-6.2,0,13-10])cube([12.4,25,20]);
        
        translate([36-22,5,0])cylinder(r=2.25,h=13,$fn=20);
        translate([36-22,20,0])cylinder(r=2.25,h=13,$fn=20);
    }
}
module partJ(){
    //little gear for cup rotater
    difference(){
        translate([0,0,2])gear(5,9,4,0);
        linear_extrude(4)motorHole(0.1);
    }
}
module partK1(h=97-5,mh=47-5){
    //big base for claw
    difference(){
        union(){
            cube([40+7,50,15]);
            translate([0,25-6,0])cube([40,12,h-10]);
            translate([0,6,h-10])cube([40,25,18]);
            translate([0,6,mh-20])cube([15-3,13,40]);
        }
        translate([5+7,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([35+7,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([5+7,45,0])cylinder(r=2.25,h=15,$fn=20);
        translate([35+7,45,0])cylinder(r=2.25,h=15,$fn=20);
        
        translate([25,36,h-10])cylinder(r=2.25,h=20,$fn=20);
        translate([5,36,h-10])cylinder(r=2.25,h=20,$fn=20);
        
        translate([20-3,6,h])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=20);
        
        translate([0,6+5,mh-15])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=20);
        translate([0,6+5,mh+15])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=20);
        
        translate([15-3,19,mh-6.3])cube([28,12,12.6]);
    }
}
module partK2(){
    //holds motor in place for lifter
    difference(){
        cube([40,13,15]);
        translate([5,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([35,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([20-6.3,0,5])cube([12.6,13,10]);
    }
}
module partK3(){
    //little gear for lifter
    difference(){
        translate([0,0,3])gear(5,8,6,0);
        linear_extrude(6)motorHole(0.1);
    }
}
module bolt(){
    cylinder(r=2.25,h=25,$fn=20);
}
module partL(){
    //unused?
    difference(){
        cube([25,113.5,20]);
        translate([0,89.5,4.95])cube([25,12.5,10.1]);
        translate([0,102,4.95])cube([25,11.5,1]);
        rotate([0,90,0])translate([-10,5.75,0])bolt();
        translate([5.75,107.75,0])bolt();
        translate([19.25,107.75,0])bolt();
        translate([5.75,20,0])bolt();
        translate([19.25,20,0])bolt();
    }
}
module partM1(d=25,l=135){
    //sissor base - unused
    difference(){
        union(){
            cube([l+5+6.25+d+5,80,4]);
            
            translate([0,2,4])cube([20,8,25]);
            translate([0,80-6-4,4])cube([20,8,25]);
            
            translate([l,0,4])cube([16.25+d,25,22]);
        }
        translate([10,0,4+15])rotate([-90,0,0])cylinder(r=4,h=80,$fn=30);
        
        translate([l+5+6.25+d,0,4+22])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=30);
        translate([l+5+6.25+d,10,4+22])rotate([-90,0,0])cylinder(r=4,h=3.5,$fn=30);
        translate([l+5,0,4+22-5])cube([12.5,25,5]);
        
        translate([l+5+6.25+d-10,15,0])cylinder(r=2.25,h=30,$fn=25);
        translate([l+5+6.25+d-10,15,0])cylinder(r1=5,r2=2.25,h=4,$fn=25);
        
        //slider holes
        translate([22.5+4,5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([110+22.5-4,5,0])cylinder(r=2.25,h=4,$fn=25);
        
        translate([22.5+4,80-5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([110+22.5-4,80-5,0])cylinder(r=2.25,h=4,$fn=25);
        
        //more holes
        translate([22.5+4,15,0])cylinder(r=2.25,h=4,$fn=25);
        translate([110,15,0])cylinder(r=2.25,h=4,$fn=25);
        
        translate([22.5+4,80-15,0])cylinder(r=2.25,h=4,$fn=25);
        translate([110,80-15,0])cylinder(r=2.25,h=4,$fn=25);
    }
}
module partM1a(){
    //axle holder thing for lighthouse
    difference(){
        union(){
            cube([30,76,4]);
            translate([0,0,4])cube([20,8,25]);
            translate([0,76-8,4])cube([20,8,25]);
        }
        translate([10,0,4+15])rotate([-90,0,0])cylinder(r=4,h=80,$fn=30);
        
        translate([25,5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([25,71,0])cylinder(r=2.25,h=4,$fn=25);
        translate([10,15,0])cylinder(r=2.25,h=4,$fn=25);
        translate([10,61,0])cylinder(r=2.25,h=4,$fn=25);
    }
}
module partM1b(d=25){
    //holds motor for LH
    difference(){
        union(){
            translate([0,0,4])cube([16.25+d,25,22]);
            translate([0,-10,0])cube([5+6.25+d+5,45,4]);
        }
        
        translate([5+6.25+d,0,4+22])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=30);
        translate([5+6.25+d,10,4+22])rotate([-90,0,0])cylinder(r=4,h=3.5,$fn=30);
        translate([5,0,4+22-5])cube([12.5,25,5]);
        
        translate([5+6.25+d-10,15,0])cylinder(r=2.25,h=30,$fn=25);
        translate([5+6.25+d-10,15,0])cylinder(r1=5,r2=2.25,h=4,$fn=25);
        
        translate([5,-5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([5+6.25+d,30,0])cylinder(r=2.25,h=4,$fn=25);
        translate([5+6.25+d,-5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([5,30,0])cylinder(r=2.25,h=4,$fn=25);
    }
}
module partM2(d=25){
    //motor holder cap for LH
    difference(){
        cube([16.25+d,25,8]);
        
        translate([5,0,0])cube([12.5,25,5]);
        translate([5+6.25+d-10,15,0])cylinder(r=2.25,h=30,$fn=25);
        //translate([25,15,0])cylinder(r=2.25,h=30,$fn=25);
        
        translate([5+6.25+d,0,0])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=30);
        translate([5+6.25+d,10,0])rotate([-90,0,0])cylinder(r=4,h=3.5,$fn=30);
    }
}
module partM3(){
    //string gear thing for LH
    difference(){
        union(){
            translate([0,0,2.5])gear(5,25,5,0);
            translate([0,0,5])cylinder(r1=13,r2=5,h=5,$fn=30);
            translate([0,0,10])cylinder(r=5,h=5,$fn=30);
            translate([0,0,15])cylinder(r1=5,r2=10,h=5,$fn=30);
        }
        cylinder(h=25,r=2.25,$fn=25);
        translate([1+5,0,15])cylinder(r=1,h=5,$fn=15);
    }
}
module partM4(r=2.25){
    //string pulley wheel for LH
    difference(){
        union(){
            cylinder(r1=8,r2=6,h=2,$fn=30);
            translate([0,0,1.5])cylinder(r1=6,r2=8,h=2,$fn=30);
        }
        cylinder(r=r,h=4,$fn=25);
    }
}
module partM4b(){
    //washer for LH
    difference(){
        cylinder(r=6,h=12.5,$fn=30);
        cylinder(r=4,h=12.5,$fn=30);
    }
}
module partM5(){
    //axle for LH
    difference(){
        union(){
            intersection(){
                rotate([-90,0,0])cylinder(r=3.75,h=88,$fn=40);
                translate([-20,-10,-3])cube([40,108,6]);
            }
            translate([-8,-3,-3])cube([16,3,6]);
        }
        translate([0,84,0])cylinder(r=2.25,h=10,$fn=20,center=true);
    }
        
}
module partM6(){
    //track thing for LH
    difference(){
        cube([110,10,22]);
        translate([8,0,15-2.25])cube([94,10,4.5]);
        
        translate([106,5,0])cylinder(r=2.25,h=30,$fn=20);
        translate([4,5,0])cylinder(r=2.25,h=30,$fn=20);
    }
}
module partM6b(){
    //unused
    difference(){
        cube([15,15,4]);
        translate([5,5,0])cylinder(r=2.25,h=4,$fn=25);
    }
}
module partM7(){
    //motor gear for LH
    difference(){
        translate([0,0,2.5])gear(5,6,5,0);
        linear_extrude(5)motorHole();
    }
}
module partN1(){
    //top platform
    difference(){
        union(){
            cube([115,22,3]);
            cube([10,4,3+8]);
            translate([0,18,0])cube([10,4,3+8]);
        }
        
        translate([5,0,3+4])rotate([-90,0,0])cylinder(r=2.25,h=80,$fn=25);
        translate([20,5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([20,22-5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([50,5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([50,22-5,0])cylinder(r=2.25,h=3,$fn=25);
    }
}
module partN2(){
    //motor holders for top of LH
    difference(){
        union(){
            cube([15,30,3]);
            translate([10,0,0])cube([10,30,25]);
        }
        translate([15,15-6.25,0])cube([5,12.5,25]);
        
        translate([5,15-6,0])cylinder(r=2.25,h=3,$fn=25);
        translate([5,15+6,0])cylinder(r=2.25,h=3,$fn=25);
        
        translate([10,4,15])rotate([0,90,0])cylinder(h=10,r=2.25,$fn=25);
        translate([10,26,15])rotate([0,90,0])cylinder(h=10,r=2.25,$fn=25);
    }
}
module partN3(){
    //spinning light platform
    difference(){
        cube([20,20,5]);
        translate([10,10,0])linear_extrude(5)
        motorHole();
    }
}
module partO1(){
    //front 'wheel' - unused
    difference(){
        union(){
            cube([10,10,4]);
            translate([0,10,0])cube([10,4,25]);
            translate([5,10,25])rotate([-90,0,0])cylinder(r=5,$fn=40,h=4);
        }
        translate([5,5,0])cylinder(r=2.25,h=4,$fn=30);
    }
}
module boltAlt(){
    //bolts for holding scissor lift together
    translate([0,-1.5,0])cube([6,3,3],center=true);
    intersection(){
        rotate([-90,0,0])cylinder(r=1.8,h=20,$fn=25);
        cube([10,50,3],center=true);
    }
}
module partP1(){
    //embedded beacon platform base
    difference(){
        union(){
            cube([30,50,5]);
            translate([0,50/2-20/2,5])cube([30,20,30]);
        }
        translate([5,5,0])cylinder(r=2.25,h=5,$fn=20);
        translate([30-5,5,0])cylinder(r=2.25,h=5,$fn=20);
        translate([5,50-5,0])cylinder(r=2.25,h=5,$fn=20);
        translate([30-5,50-5,0])cylinder(r=2.25,h=5,$fn=20);
        
        translate([30/2,50/2,5])cylinder(r=6.5/2+0.15,h=40,$fn=30);
    }
}
module partP2(){
    //embedded beacon platform top
    difference(){
        union(){
            translate([15,15,3])cylinder(r=8,h=10,$fn=40);
            cube([30,30,3]);
        }
        translate([15,15,2])cylinder(r=6.5/2+0.15,h=12,$fn=40);
    }
}
module partQ1(){
    //connector for holding wood together
    difference(){
        cube(20);
        translate([5,5,5])cube(15);
        translate([12.5,12.5,0])cylinder(r=2.25,h=20,$fn=30);
        translate([12.5,0,12.5])rotate([-90,0,0])cylinder(r=2.25,h=20,$fn=30);
        translate([0,12.5,12.5])rotate([0,90,0])cylinder(r=2.25,h=20,$fn=30);
    }
}
module partR1(){
    //holds flag servo down
    difference(){
        cube([51,10,16]);
        translate([0,0,4])cube([10,10,20]);
        translate([41,0,4])cube([10,10,20]);
        translate([14,0,0])cube([23,10,12]);
        translate([5,5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([46,5,0])cylinder(r=2.25,h=4,$fn=25);
    }
}
module partR2(){
    //lifts flag servo up
    difference(){
        cube([51,20,30]);
        translate([5,10,0])cylinder(r=2.25,h=50,$fn=25);
        translate([46,10,0])cylinder(r=2.25,h=50,$fn=25);
    }
}
module partS(){
    //unused
    difference(){
        rotate_extrude($fn=50){
            polygon(points=[[0,0],[0,15],[14,15],[4,10],[4,4],[8,0]]);
        }
        linear_extrude(20)motorHole();
        translate([11,0,0])cylinder(r=0.75,h=20,$fn=20);
    }
}

module partT1(r=30){
    //wheel
    cylinder(r=r-3,h=12,$fn=100);
    difference(){
        translate([0,0,12+2.5])gear(5,15,10,0);
        translate([0,0,12+2.5])cylinder(r=16.3/2,h=10,$fn=40,center=true);
    }
}
module partT2(){
    //bearing holder for wheel
    difference(){
        union(){
            translate([10,30,30-6-18])rotate([-90,0,0])cylinder(r=2.5,h=10,$fn=30);
            cube([20,30,12]);
        }
        translate([5,5,0])cylinder(r=2.25,h=30,$fn=25);
        translate([15,5,0])cylinder(r=2.25,h=30,$fn=25);
        translate([5,25,0])cylinder(r=2.25,h=30,$fn=25);
        translate([15,25,0])cylinder(r=2.25,h=30,$fn=25);
        translate([0,0,7.8])cube([20,50,5]);
    }
}
module dmotorHole(x=0.1){
    difference(){
        circle(r=2+x,$fn=25);
        translate([3.4-2+x,-2.5])square([3,5]);
    }
}
module partT3(){
    //motor gear for wheel
    difference(){
        union(){
            translate([0,0,2.5])gear(5,32,5,0);
            cylinder(r=6,h=12,$fn=30);
        }
        linear_extrude(12)dmotorHole();
    }
}
module partT4(){
    //motor holder for drive
    difference(){
        union(){
            translate([0,0,12.5])rotate([-90,0,0])cylinder(r=12.5+3,h=50,$fn=50);
            translate([-12.5-3,0,0])cube([31,50,12.5]);
            translate([-26,0,0])cube([52,50,4]);
        }
        translate([0,0,12.5])rotate([-90,0,0])cylinder(r=12.5,h=50,$fn=50);
        translate([-12.5,0,-3])cube([25,50,12.5+3]);
        
        translate([-26+5,5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([26-5,5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([-26+5,45,0])cylinder(r=2.25,h=4,$fn=25);
        translate([26-5,45,0])cylinder(r=2.25,h=4,$fn=25);
    }
}}
module partU(){
    //front wheel holder
    difference(){
        union(){
            cube([54,12,8]);
            cube([16,38,8]);
            translate([8,30,8])cylinder(r=8,h=19,$fn=40);
        }
        translate([8,30,0])cylinder(r=3.9,h=8+19,$fn=40);
        translate([48,6,0])cylinder(r=2.25,h=18,$fn=25);
        translate([18,6,0])cylinder(r=2.25,h=18,$fn=25);
    }
}
module partV(){
    //photointerrupter card thing
    difference(){
        cube([25,11,2]);
        translate([5,5,0])cylinder(r=2.1,h=2,$fn=30);
        translate([20,5,0])cylinder(r=2.1,h=2,$fn=30);
        translate([5,5-2.1,0])cube([15,4.2,2]);
    }
    translate([12.5-3,11,0])cube([6,2,5]);
}
module partW(){
    //cable holder for rev
    difference(){
        cube([14,8,8]);
        translate([2,0,0])cube([3,8,6]);
        translate([8,0,2])cube([3,8,4]);
        translate([10,0,0])cube([2,8,3]);
        translate([12,0,0])cube([2,8,1.5]);
    }
}
module partX(){
    //spring raiser for claw
    difference(){
        cylinder(r=4,h=1,$fn=30);
        cylinder(r=2.2,h=1,$fn=30);
    }
}



