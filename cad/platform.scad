module bolt(){
    cylinder(r=2.25,h=42,$fn=20);
}

module poleConnector(){
    difference(){
        union(){
            cube([20,10,25]);
            translate([0,0,25])cube([20,8,8]);
            translate([0,11,0])cube([20,10,25]);
        }
        translate([10,10.5,0])cylinder(r=3.35,h=50,$fn=30);
        rotate([90,0,0])translate([3.5,20,-42])bolt();
        rotate([90,0,0])translate([16.5,20,-42])bolt();
        rotate([90,0,0])translate([3.5,5,-42])bolt();
        rotate([90,0,0])translate([16.5,5,-42])bolt();
        rotate([0,90,0])translate([-29,4,0])bolt();
        }
}

module cameraConnector(){
    difference(){
        union(){
            cube([26,3,30]);
            translate([0,0,22])cube([3,20,8]);
            translate([23,0,22])cube([3,20,8]);
        }
        rotate([0,90,0])translate([-26,15,0])bolt();
        rotate([90,0,0])translate([5,5,-3])bolt();
        rotate([90,0,0])translate([21,5,-3])bolt();
    }
}
module partP1(){
    //main platform
    l=85;
    difference(){
        union(){
            cube([l,l,3]);
            translate([l/2,l/2,3])cylinder(r=8,h=12,$fn=40);
            translate([l/2-3,l/2-18,3])cube([3,36,12]);
        }
        translate([l/2,l/2-8,3])cube([8,16,12]);
        translate([l/2,l/2,3])cylinder(r=6.4/2,h=12,$fn=40);
        translate([l/2-3,l/2-13,8])rotate([0,90,0])
        cylinder(r=2.25,h=3,$fn=25);
        translate([l/2-3,l/2+13,8])rotate([0,90,0])
        cylinder(r=2.25,h=3,$fn=25);
    }
}partP1();
module partP2(){
    //platform clamp
    l=85;
    difference(){
        union(){
            translate([l/2,l/2,3])cylinder(r=8,h=12,$fn=40);
            translate([l/2,l/2-18,3])cube([3,36,12]);
        }
        translate([l/2-8,l/2-8,3])cube([8,16,12]);
        translate([l/2,l/2,3])cylinder(r=6.4/2,h=12,$fn=40);
        translate([l/2,l/2-13,8])rotate([0,90,0])
        cylinder(r=2.25,h=3,$fn=25);
        translate([l/2,l/2+13,8])rotate([0,90,0])
        cylinder(r=2.25,h=3,$fn=25);
    }
}//partP2();