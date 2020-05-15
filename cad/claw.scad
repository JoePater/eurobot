include <utils.scad>;
//include <gear.scad>;

module partK1a(h=92,mh=42){
    //base of arm
    w=4;
    difference(){
        union(){
            cube([15,50,47]);
            translate([0,20,0])cube([h+10,20,12]);
            translate([h-18-w,40,0])cube([32,10,12]);
        }
        //bolts
        translate([0,5,12])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
        translate([0,5,42])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
        translate([0,45,12])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
        translate([0,45,42])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
        
        translate([20,25,0])cylinder(r=2.25,h=12,$fn=25);
        translate([20,35,0])cylinder(r=2.25,h=12,$fn=25);
        
        translate([h-25-w,25,0])cylinder(r=2.25,h=12,$fn=25);
        translate([h-13-w,25,0])cylinder(r=2.25,h=12,$fn=25);
        translate([h-13-w,45,0])cylinder(r=2.25,h=12,$fn=25);
        //other
        translate([h,20,17])rotate([-90,0,0])
        cylinder(r=7.8,h=30,$fn=50);
    }
}
module partK1b(h=92,mh=42){
    //middle layer of arm
    w=4;
    m2d=18;
    difference(){
        union(){
            translate([15+0.2,20,12])cube([mh-6.2-0.2,20,w]);
            translate([mh-6.2-w,20,12])cube([12.4+2*w,20,m2d]);
            translate([mh+6.2+w,20,12])cube([h-26.2-mh-w,20,w]);
            translate([h-18-w,20,12])cube([10.2+w,30,w]);
            translate([h-8-w,20,12+w])cube([18+w,30,11]);
        }
        //bolts
        translate([20,25,12])cylinder(r=2.25,h=w,$fn=25);
        translate([20,35,12])cylinder(r=2.25,h=w,$fn=25);
        
        translate([h-25-w,25,12])cylinder(r=2.25,h=w,$fn=25);
        translate([h-13-w,25,12])cylinder(r=2.25,h=w,$fn=25);
        translate([h-13-w,45,12])cylinder(r=2.25,h=w,$fn=25);
        //other
        translate([mh-6.2,20,12])cube([12.4,20,10]);
        
        translate([h,20,17])rotate([-90,0,0])
        cylinder(r=7.8,h=30,$fn=50);
    }
}
module partK1c(h=92,mh=42){
    //top layer of arm
    w=4;
    m2d = 18;
    m2h = h-sqrt(pow(h-mh,2)-pow(m2d,2));
    echo(m2h);
    difference(){
        union(){
            translate([15+0.2,20,12+w])cube([mh-21.2-w-0.2,20,w]);
            translate([mh-6.2-w*2,20,12+w])
            cube([12.4+w*4,20,m2d+10]);
            translate([mh+6.2+2*w,20,12+w])
            cube([h-14.2-mh-3*w-0.2,20,w]);
            translate([h-18-w,20,12+w])cube([10-0.2,30,w]);
        }
        //bolts
        translate([20,25,12+w])cylinder(r=2.25,h=w,$fn=25);
        translate([20,35,12+w])cylinder(r=2.25,h=w,$fn=25);
        
        translate([h-25-w,25,12+w])cylinder(r=2.25,h=w,$fn=25);
        translate([h-13-w,25,12+w])cylinder(r=2.25,h=w,$fn=25);
        translate([h-13-w,45,12+w])cylinder(r=2.25,h=w,$fn=25);
        //other
        translate([mh-6.2-w-0.2,20,12+w])
        cube([12.4+w*2+0.4,20,m2d-w]);
        translate([mh-6.2-w*2,20,12+m2d+w])cube([5,20,20]);
        translate([m2h-6.2,20,12+m2d])cube([12.4,20,10]);
    }
}
module partU(){
    //front wheel holder
    difference(){
        union(){
            cube([22-0.2,30,10]);
            translate([6,30,0])cube([16-0.2,30,10]);
            translate([14,52,-12-4*2+8])
            cylinder(r=8-0.2,h=12+4*2-8,$fn=50);
        }
        translate([14,52,-12-4*2+8])
        cylinder(r=3.9,h=22,$fn=50);
        translate([5,5,0])cylinder(r=2.25,h=10,$fn=25);
        translate([17,5,0])cylinder(r=2.25,h=10,$fn=25);
        translate([17,25,0])cylinder(r=2.25,h=10,$fn=25);
    }
}
module clawBase(){
    partK1a();
    translate([0,0,0.2])partK1b();
    translate([0,0,0.4])partK1c();
    translate([92-25-4-5,20,12+2*4+0.6])partU();
}//clawBase();
module partK2(){
    //little gear for lifter
    difference(){
        translate([0,0,3])gear(5,8,6,0);
        linear_extrude(6)motorHole(0.1);
    }
}
module partI1(a=15){
    //big gear for claw
    difference(){
        translate([0,0,-4])cylinder(r=10,h=12,$fn=40);
        translate([0,0,-4])cylinder(r=2.15,h=12,$fn=20);
        translate([0,0,-4])cylinder(r=4,h=3.5,$fn=6);
    }
    difference(){
        translate([0,0,4])gear(5,55,8,4.5);
        
        cylinder(r=2.15,h=8,$fn=20);
        translate([-150,-150,0])cube([300,140,8]);
        translate([-150,-150,0])cube([140,300,8]);
        translate([-1,a+15,0])cylinder(r=2.25,h=8,$fn=20);
        translate([19,a+5,0])cylinder(r=2.25,h=8,$fn=20);
        translate([19,a+15,0])cylinder(r=2.25,h=8,$fn=20);
    }
}
module liftGears(){
    m2d = 18;
    m2h = sqrt(pow(50,2)-pow(m2d,2));
    partI1();
    translate([0,50,0])partK2();
    translate([m2d,m2h,0])partK2();
}
module partI2(b=47,c=25.5){
    //ataches claw to lifting gear
    linear_extrude(8)difference(){
        translate([0,0])square([25+12,20]);

        translate([12,15])circle(r=2.25,$fn=20);
        translate([20+12,5])circle(r=2.25,$fn=20);
        translate([20+12,15])circle(r=2.25,$fn=20);
    }
    cube([12,10,33]);
    difference(){
        translate([0,0,29])cube([40,33,10]);
        
        translate([30.5,0,39])rotate([-90,0,0])
        cylinder(r=4,h=33,$fn=25);
        translate([30.5,10,39])rotate([-90,0,0])
        cylinder(r=8,h=5.2,$fn=25);
        translate([30.5,25,39])rotate([-90,0,0])
        cylinder(r=8,h=5.2,$fn=25);
        
        translate([22,5,29])cylinder(r=2.25,h=10,$fn=20);
        translate([22,20,29])cylinder(r=2.25,h=10,$fn=20);
       
    }
}
module partI3(c=25.5){
    //cap, holds motor and pivot for claw
    d = 19;
    difference(){
        cube([40,33,13]);
        
        translate([30.5,0,0])rotate([-90,0,0])
        cylinder(r=4,h=33,$fn=25);
        translate([30.5,10,0])rotate([-90,0,0])
        cylinder(r=8,h=5.2,$fn=25);
        translate([30.5,25,0])rotate([-90,0,0])
        cylinder(r=8,h=5.2,$fn=25);
        
        translate([30.5-d-6.2,0,0])cube([12.4,33,10]);
        
        translate([22,5,0])cylinder(r=2.25,h=13,$fn=25);
        translate([22,20,0])cylinder(r=2.25,h=13,$fn=25);
    }
}






module partJ(){
    //little gear for cup rotater
    difference(){
        translate([0,0,2])gear(4.95,6,4,0);
        translate([0,0,1])linear_extrude(3)motorHole(0.1);
    }
}
module partH(){
    //nut-gear
    difference(){
        translate([0,0,2.5])gear(4.95,19,5,0);
        
        translate([0,0,1.8])cylinder(r=3.95,h=3.2,$fn=6);
        cylinder(r=2.25,h=5,$fn=20);
    }
}
module partB1(h=7,l=32,w=28,d=20){
    //claw pivot holder thing
    difference(){
        cube([l,w,h]);
        translate([6,0,h])rotate([-90,0,0])
        cylinder(h=w,r=2.1,$fn=30);
        translate([6,5,h])rotate([-90,0,0])
        cylinder(h=3.2,r=5,$fn=30);
        
        translate([d+6,0,h])rotate([-90,0,0])
        cylinder(h=w,r=2.1,$fn=30);
        translate([d+6,5,h])rotate([-90,0,0])
        cylinder(h=3.2,r=5,$fn=30);
        
        translate([16,22,0])cylinder(h=h,r=2.25,$fn=20);        
        translate([16,22,0])cone1();
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
            linear_extrude(6)translate([10,35+a,0])
            rotate(155)arc(36.8,39.8,65);
            translate([10,85,0])
            linear_extrude(11.5,scale=1+(18/72/10)){
                rotate(-27)translate([-33-5.5,-5])
                square([5.5,5]);
                rotate(-10)translate([-36-2.5,-2.5])
                square([2.5,5]);
                rotate(55)translate([-36-6,-2.5])
                square([6,5]);
            }
            translate([-21,-6,0])rotate([0,0,15])
            cube([25,25,6]);
            translate([4,14,1.5])rotate([0,90,0])
            cylinder(r=2.1,h=4,$fn=30);
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
            linear_extrude(6)translate([10,35+a,0])
            rotate(155)arc(36.8,39.8,65);
            translate([10,85,0])
            linear_extrude(11.5,scale=1+(18/72/10)){
                rotate(-27)translate([-33-5.5,-5])
                square([5.5,5]);
                rotate(-10)translate([-36-2.5,-2.5])
                square([2.5,5]);
                rotate(55)translate([-36-6,-2.5])
                square([6,5]);
            }
            translate([4,14,1.5])rotate([0,90,0])
            cylinder(r=2.1,h=4,$fn=30);
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
module bothClaws(){
    clawL();
    translate([20,0,0])clawR();
    translate([39,31,8])rotate([90,0,-15])clawLa();
    translate([-16,46,6+0.2])clawRa();
}//bothClaws();
module clawPalm(){
    translate([-6,-7.1,0])rotate([-90,0,0])partB1();
    mirror([0,1,0])translate([-6,-7.1,0])rotate([-90,0,0])partB1();
    translate([0,0,0.2])bothClaws();
}//clawPalm();
module clawWrist(){
    clawPalm();
    translate([10,-7.2,-22])rotate([90,0,0])partH();
}//clawWrist();
module clawForearm(){
    liftGears();
    translate([-13,15,8+0.2])partI2();
    translate([-13,15,47+0.4])partI3();
    translate([17.5,2.5,47.3])rotate([0,90,180])
    translate([-10,0,22])clawWrist();
    translate([-2.5,14.5,52.2])rotate([90,0,0])partJ();
}//clawForearm();
module clawArm(){
    clawBase();
    translate([92,16-0.2,17])rotate([90,-90,0])clawForearm();
}clawArm();
module clawLift2(){
    clawBase();
    translate([92,16-0.2,17])rotate([90,-90,0])clawLift1();
}//clawLift2();

module partX(){
    //spring raiser for claw
    difference(){
        cylinder(r=4,h=1,$fn=30);
        cylinder(r=2.2,h=1,$fn=30);
    }
}
