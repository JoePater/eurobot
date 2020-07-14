use <utils.scad>
use <gear.scad>

d=38;

module baseHoles(){
    translate([5,5])circle(r=2.25,$fn=25);
    translate([5,13])circle(r=2.25,$fn=25);
    translate([40,5])circle(r=2.25,$fn=25);
    translate([40,13])circle(r=2.25,$fn=25);
    translate([100,5])circle(r=2.25,$fn=25);
    translate([100,13])circle(r=2.25,$fn=25);
}

module baseA(){
    difference(){
        cube([105,18,3]);
        translate([22.5,0,12.5+1.5])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        linear_extrude(3)baseHoles();
    }
}translate([0,0,-3.2])baseA();

module baseB(){
    a=sqrt(d*d-64);
    difference(){
        union(){
            cube([40+20,18,27]);
            translate([22.5+a-12.5,0,0])cube([25,18,d+3-3]);
            translate([a+22.5,18,3])rotate([-90,0,0])
            cylinder(r=2.5,h=5,$fn=30);
        }
        cube([22.5,18,12]);
        translate([22.5,0,11])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        translate([22.5,0,0])cube([12.5,18,11]);
        translate([22.5+a,0,d+3])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        
        linear_extrude(27)baseHoles();
    }
}baseB();