use <utils.scad>
use <gear.scad>

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
module partT3(){
    //motor gear for wheel
    difference(){
        union(){
            translate([0,0,2.5])gear(5,32,5,0);
            cylinder(r=6,h=12,$fn=30);
        }
        linear_extrude(12)dmotorHole();
    }
}//partT3();
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
}

module sideMarkerA(){
    //base of marker for sides
    difference(){
        union(){
            cube([3,50,20]);
            cube([80,3,20]);
        }
        translate([0,40,10])rotate([0,90,0])
        cylinder(r=2.75,h=3,$fn=40);
        translate([0,20,10])rotate([0,90,0])
        cylinder(r=2.75,h=3,$fn=40);
        
        translate([75,0,10])rotate([-90,0,0])
        cylinder(r=2.25,h=3,$fn=40);
        translate([60,0,10])rotate([-90,0,0])
        cylinder(r=2.25,h=3,$fn=40);
    }
}sideMarkerA();

module sideMarkerB(){
    //marker back
    difference(){
        union(){
            cube([70,70,1.5]);
            translate([-25,35-5,0])cube([25,10,1.5]);
        }
        translate([10,10,0])cube([20,50,1.5]);
        translate([40,10,0])cube([20,50,1.5]);
        
        translate([-5,35,0])cylinder(r=2.25,h=1.5,$fn=30);
        translate([-20,35,0])cylinder(r=2.25,h=1.5,$fn=30);
    }
}//sideMarkerB();



