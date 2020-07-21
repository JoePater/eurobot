
/*
y-axis is front panel
x-axis is middle
x-y plane is floor
*/
module fakeClaw(){
    w=4;
    translate([-15,-45-36,41])difference(){
        cube([15,50,47]);
        //bolts
        translate([0,5,12])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
        translate([0,5,42])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
        translate([0,45,12])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
        translate([0,45,42])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=25);
    }
}fakeClaw();
d=38;
rd = 22;
mh = 12.5 + 2.5;
wh = 6;
md = sqrt(d*d - pow(mh-wh,2));
l = md*4;
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

module __fakeDrive(){
    translate([-5,-9,24])difference(){
        cube([l,18,5]);
        linear_extrude(5)baseHoles();
    }
}
module __fakeDrive2(){
    translate([-5,-9,24])difference(){
        translate([l/2-md,0,0])cube([md*2,18,5]);
        linear_extrude(5)baseHoles();
    }
}

module fakeDrive(){
    translate([40,68,0])__fakeDrive();
    translate([40,40])__fakeDrive2();
    translate([40,-68,0])__fakeDrive();
    translate([40,-40])__fakeDrive2();
}fakeDrive();

module fakeRev(){
    translate([0,0,193])difference(){
        translate([6,-100,0])cube([40,120,5]);
        
        translate([22-6.5,8.75,0])cylinder(r=2.25,h=5,$fn=20);
        translate([22-6.5,-8.75,0])cylinder(r=2.25,h=5,$fn=20);
        translate([42-6.5,8.75,0])cylinder(r=2.25,h=5,$fn=20);
        translate([42-6.5,-8.75,0])cylinder(r=2.25,h=5,$fn=20);
        
        translate([13.5,-30,0])cylinder(r=2.25,h=5,$fn=20);
        translate([13.5,-88,0])cylinder(r=2.25,h=5,$fn=20);
        translate([27.5,-30,0])cylinder(r=2.25,h=5,$fn=20);
        translate([27.5,-88,0])cylinder(r=2.25,h=5,$fn=20);
    }
    translate([-6.5,0,205.5])rotate([0,90,0])
    cylinder(r=28.5,h=10,$fn=60);
}fakeRev();

module fakeStopper(){
    translate([-5,-15,58])difference(){
        cube([5,30,25]);
        
        translate([0,5,5])rotate([0,90,0])
        cylinder(r=2.25,h=5,$fn=20);
        translate([0,25,5])rotate([0,90,0])
        cylinder(r=2.25,h=5,$fn=20);
        translate([0,5,20])rotate([0,90,0])
        cylinder(r=2.25,h=5,$fn=20);
        translate([0,25,20])rotate([0,90,0])
        cylinder(r=2.25,h=5,$fn=20);
    }
}fakeStopper();



















