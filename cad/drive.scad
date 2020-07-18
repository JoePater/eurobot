use <utils.scad>
use <gear.scad>

d=38;
rd = 22;
mh = 12.5 + 2.5;
wh = 6;
md = sqrt(d*d - pow(mh-wh,2));
l = 24 + md*2;

fr = false;

module baseHoles(){
    translate([33,5])circle(r=2.25,$fn=25);
    translate([33,13])circle(r=2.25,$fn=25);
    translate([l-33,5])circle(r=2.25,$fn=25);
    translate([l-33,13])circle(r=2.25,$fn=25);
}

module baseA(front=true){
    difference(){
        cube([l,18,8.5]);
        translate([12,0,mh])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        translate([12+2*md,0,mh])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        linear_extrude(8.5)baseHoles();
    }
    if(front){
        translate([12+md,18,wh])rotate([-90,0,0])
        cylinder(r=2.5,h=4.5,$fn=40);
    }
}translate([0,0,-8.7])baseA(fr);

module baseB(front=true){
    if(front){
        translate([l/2-8,0,5])cube([16,18,rd-7.5-6.2]);
    }
    difference(){
        union(){
            translate([10,0,0])cube([l-20,18,22]);
        }
        translate([l/2-20,0,5])cube([40,18,17]);
        translate([12,0,mh-8.5])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        translate([12+2*md,0,mh-8.5])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        linear_extrude(22)baseHoles();
    }
}baseB(fr);

module baseC(front=true){
    if(front){
        difference(){
            translate([l/2-19.5,0,0])cube([39,18,wh+rd+6.2-8.5-5+3]);
            
            translate([l/2-8.5,0,0])cube([17,18,rd-7.5+6.2]);
            translate([l/2-19.5,0,3])cube([11-3,18,30]);
            translate([l/2+19.5-8,0,3])cube([11-3,18,30]);
            linear_extrude(3)baseHoles();
        }
    }
}translate([0,0,5.2])baseC(fr);


