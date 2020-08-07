use <utils.scad>
use <gear.scad>

d=38; //distance between motor and wheel
rd = 22; //distance between re and wheel
mh = 12.5 + 2.5; //motor height
wh = 6; //wheel height
md = sqrt(d*d - pow(mh-wh,2)); //horizontal motor distance
l = md*4; //length of long holder
wd = 25; //wheel depth
fr = false;
if(!fr){
    l = md*2+40;
}

module baseHoles(){
    translate([l/2-16,5])circle(r=2.25,$fn=25);
    translate([l/2-16,13])circle(r=2.25,$fn=25);
    translate([l/2+16,5])circle(r=2.25,$fn=25);
    translate([l/2+16,13])circle(r=2.25,$fn=25);
    
    translate([l/2-68,5])circle(r=2.25,$fn=25);
    translate([l/2-68,13])circle(r=2.25,$fn=25);
    translate([l/2+68,5])circle(r=2.25,$fn=25);
    translate([l/2+68,13])circle(r=2.25,$fn=25);
}

module baseA(front=true){
    //base layer
    difference(){
        if(!front){
            translate([l/2-50,0,0])cube([100,18,8.5]);
        }else{
            cube([l,18,8.5]);
        }
        translate([l/2-md,0,mh])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        translate([l/2+md,0,mh])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        linear_extrude(8.5)baseHoles();
        translate([l-12,0,4.5])cube([12,18,4]);
        translate([0,0,4.5])cube([12,18,4]);
    }
    if(front){
        translate([l/2,18,wh])rotate([-90,0,0])
        cylinder(r=2.5,h=4.5,$fn=40);
    }
}//translate([0,0,-8.7])baseA(fr);

module baseB(front=true){
    //second layer
    if(front){
        difference(){
            translate([l/2-8,3,5])cube([16,18-3,rd-7.5]);
            translate([l/2,0,rd-2.5])rotate([-90,0,0])
            cylinder(r=3.3,h=18,$fn=40);
        }
    }
    difference(){
        union(){
            translate([l/2-40,0,0])cube([80,18,22]);
        }
        translate([l/2-20,0,5])cube([40,18,17]);
        translate([l/2-md,0,mh-8.5])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        translate([l/2+md,0,mh-8.5])rotate([-90,0,0])
        cylinder(r=12.5,h=18,$fn=70);
        linear_extrude(22)baseHoles();
    }
}//baseB(fr);

module baseC(front=true){
    //rotary holder
    if(front){
        difference(){
            union(){
                translate([l/2-19.5,0,0])
                cube([10,18,3]);//wh+rd+6.2-8.5-5+3]);
                translate([l/2+19.5-10,0,0])
                cube([10,18,3]);
                translate([l/2-11.5,3,0])cube([23,18-3,20]);
            }
            
            translate([l/2-8.5,0,0])cube([17,18,rd-7.5]);
            translate([l/2,0,rd-7.5])rotate([-90,0,0])
            cylinder(r=3.3,h=18,$fn=40);

            linear_extrude(3)baseHoles();
        }
    }
}//translate([0,0,5.2])baseC(fr);

module baseD(){
    //clamps wheel from other side
    difference(){
        cube([l,wd+8+18,6+4]);
        
        translate([12,0,0])cube([l-24,18+wd,10]);
        //translate([0,0,0])cube([l,18.2,4]);
        linear_extrude(10)baseHoles();
    }
    difference(){
        translate([l/2,18+wd-4.5,2.5-1])rotate([-90,0,0])
        cylinder(r=2.5,h=4.5,$fn=40);
        translate([l/2-3,18+wd-4.5,-2])cube([6,4.5,2]);
    }
}//translate([0,0,-4])baseD();
    
module wheel(){
    difference(){
        union(){
            cylinder(r=30-3,h=12,$fn=100);
            translate([0,0,12+3])gear(5,24,6,0);
            translate([0,0,18+3])gear(5,15,6,0);
        }
        cylinder(r=8.05,h=5,$fn=50);
        translate([0,0,19])cylinder(r=8.05,h=5,$fn=50);
        translate([0,0,5])cylinder(r1=8,r2=0,h=8,$fn=50);
    }
}//translate([l/2,18+25,0])rotate([90,0,0])wheel();

module motorGear(){
    difference(){
        translate([0,0,2.5])gear(4.9,24,5,0);
        translate([8,0,0])cylinder(r1=4,r2=1.75,h=5,$fn=25);
        translate([-8,0,0])cylinder(r1=4,r2=1.75,h=5,$fn=25);
        translate([0,8,0])cylinder(r1=4,r2=1.75,h=5,$fn=25);
        translate([0,-8,0])cylinder(r1=4,r2=1.75,h=5,$fn=25);
    }
}//translate([d,0,0])motorGear();

module rotaryGear(){
    difference(){
        translate([0,0,2.5])gear(5,13,5,0);
        difference(){
            cylinder(r=3.1,h=5,$fn=40);
            translate([1.6,-3,0])cube([2,6,5]);
        }
    }
}translate([rd,0,0])rotaryGear();




