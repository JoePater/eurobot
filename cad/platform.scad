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