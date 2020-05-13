module a(){
    circle(r=80,$fn=80);
}

module b(){
    for(i=[0:4]){
        rotate([0,0,i*72])
        translate([70,0,0])
        square([70,30],center=true);            
    }
}
module c(){
    for(i=[0:4]){
        rotate([0,0,i*72]){
            translate([70,-20])
            circle(r=2.25,$fn=20);
            translate([70,20])
            circle(r=2.25,$fn=20);
        }
    }
}

module base(){
difference(){
    linear_extrude(4)a();
    union(){
        translate([0,0,4-2])linear_extrude(2)b();
        linear_extrude(5)c();
        cylinder(r=2.25,h=5,$fn=20);
    }
}
}
module a2(){
    difference(){
        square([50.5,22]);
        translate([15.25,10])square([20,9]);
}}

module b2(){
    difference(){
        a2();
        translate([5.75,11])circle(r=2.25,$fn=20);
}
}

module c2(){
    difference(){
        b2();
        translate([45.75,11])circle(r=2.25,$fn=20);
    }}
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