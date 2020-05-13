r1 = 31;
w = 3;
h = 20;

module servo(){
    translate([0,-11.75-w,-10])difference(){
        cube([12.5+2*w,23.5+2*w,30]);
        translate([w,w,0])cube([12.5,23.5,30]);
        translate([3+w,23.5+w,0])cube([12.5-2*3,w,10]);
        translate([3,23.5+w-1,0])rotate([0,-45,0])cylinder(r=1,h=5,$fn=15);
        translate([12.5+w-1,3,0])rotate([45,0,0])cylinder(r=1,h=5,$fn=15);
    }
}

module holder(){
    difference(){
        union(){
            cylinder(h=h,r=r1+w,$fn=80);
            translate([4+r1+w,0,h-5])cylinder(r=4,h=5,$fn=20);
            translate([r1+w/2+0.875,0,2.5+h-5])cube([w+1.75,4,5],center=true);
        }
        cylinder(h=h,r=r1,$fn=80);
        translate([4+r1+w,0,h-5])cylinder(r=2.25,h=5,$fn=20);
        translate([0,-4-15,0])cube([r1+w,15,h]);
    }
    rotate([0,0,-60])translate([r1,0,0])servo();
    translate([-3,r1,-10])cube([6,w,10]);
}

module bar1(){
    difference(){
        union(){
            cube([10,20,3]);
            translate([5,5,3])cylinder(r=5,h=6,$fn=30);
        }
        translate([5,15,0])cylinder(h=3,r=2.25,$fn=20);
        translate([5,5,6])cylinder(h=6,r=2.4,$fn=20);
    }
}

module bar2(){
    difference(){
        union(){
            cube([8,44,3]);
            translate([0,25,3])cube([8,19,6]);
        }
        translate([4,4,0])cylinder(h=3,r=2.25,$fn=20);
        translate([4,10,0])cylinder(h=3,r=2.25,$fn=20);
        translate([4,30,0])cylinder(h=9,r=2.25,$fn=20);
        translate([4,39,0])cylinder(h=9,r=2.25,$fn=20);
    }
}
module servoTest(){
    linear_extrude(2)difference(){
        translate([-4,-4])square([65,8]);
        for(i=[0:7]){
            translate([i*8,0])circle(r=2.3+i*0.03,$fn=30);
        }
    }
}



//servo();
bar1();
//translate([12,0,0])bar1();
//holder();



