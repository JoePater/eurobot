use <utils.scad>

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
        translate([0,5,12])rotate([0,90,0])blt(15);
        translate([0,5,42])rotate([0,90,0])blt(15);
        translate([0,45,12])rotate([0,90,0])blt(15);
        translate([0,45,42])rotate([0,90,0])blt(15);
    }
}//fakeClaw();
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
    translate([40,40,0])__fakeDrive2();
    translate([40,-68,0])__fakeDrive();
    translate([40,-40,0])__fakeDrive2();
}//fakeDrive();

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
}//fakeRev();

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
}//fakeStopper();

module skelA1(){
    //right side of drive connector
    difference(){
        union(){
            translate([40,0,10])cube([95,77,14]);
            translate([40,0,10])cube([95,10,50]);
            translate([40,0,10])cube([10,25,50]);
        }
        translate([40,25,10])cube([40,77-25,14]);
        
        translate([35,59,10])linear_extrude(14)baseHoles();
        translate([35,31,10])linear_extrude(14)baseHoles();
        
        translate([95,0,24+5])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=25);
        translate([135-5,0,24+5])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=25);
        translate([95,0,60-5])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=25);
        translate([135-5,0,60-5])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=25);
        
        translate([40,20,24+5])rotate([0,90,0])
        cylinder(r=2.25,h=10,$fn=25);
        translate([40,20,60-5])rotate([0,90,0])
        cylinder(r=2.25,h=10,$fn=25);
    }
}//skelA1();

module skelB1(){
    mirror([0,1,0])skelA1();
}//skelB1();

module skelC(){
    //connects top parts
    translate([0,0,193-5.2])difference(){
        union(){
            translate([8.5,-93,0])cube([40,138,5]);
            //translate([35.5+5,-20,-40])cube([8,40,40]);
            translate([22,45-10,5])cube([40-22+8.5,10,20]);
           
        }
        //rev holder
        translate([22-6.5,8.75,0])cylinder(r=2.25,h=5,$fn=20);
        translate([22-6.5,-8.75,0])cylinder(r=2.25,h=5,$fn=20);
        translate([42-6.5,8.75,0])cylinder(r=2.25,h=5,$fn=20);
        translate([42-6.5,-8.75,0])cylinder(r=2.25,h=5,$fn=20);
        //motor holder
        translate([13.5,-30,0])cylinder(r=2.25,h=5,$fn=20);
        translate([13.5,-88,0])cylinder(r=2.25,h=5,$fn=20);
        translate([27.5,-30,0])cylinder(r=2.25,h=5,$fn=20);
        translate([27.5,-88,0])cylinder(r=2.25,h=5,$fn=20);
        //connector holes
        //translate([
        //photointerrupter holes
        translate([15.5,32+7.5,0])cylinder(r=2.25,h=5,$fn=20);
        translate([15.5,32-7.5,0])cylinder(r=2.25,h=5,$fn=20);
        //platform
        translate([22+26.5/2,45,0])cylinder(r=3.15,h=25,$fn=40);
        translate([40+8.5-5,35,10])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=30);
        translate([22+5,35,10])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=30);
    }
}//skelC();

module skelD(){
    //clamps platform
    difference(){
        cube([26.5,10,25]);
        translate([26.5/2,0,0])cylinder(r=3.15,h=25,$fn=40);
        translate([26.5-5,0,10])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=25);
        translate([5,0,10])rotate([-90,0,0])
        cylinder(r=2.25,h=10,$fn=25);
    }
}//translate([22,35+10.2,193-5.2])skelD();

module skelE(){
    //connects top layer to skelF
    difference(){
        union(){
            translate([8,-93,193-5.2-5.2])cube([24,68,5]);
            translate([8,-59-10,42])cube([24,20,193-5.2-5.2-42]);
            translate([8,-81,42])cube([8,50,88-42]);
        }
        //rev holder
        translate([13.5,-30,193-5.2-5.2])cylinder(r=2.25,h=5,$fn=20);
        translate([13.5,-88,193-5.2-5.2])cylinder(r=2.25,h=5,$fn=20);
        translate([27.5,-30,193-5.2-5.2])cylinder(r=2.25,h=5,$fn=20);
        translate([27.5,-88,193-5.2-5.2])cylinder(r=2.25,h=5,$fn=20);
        //holes for claw
        translate([8,-76,53])rotate([0,90,0])blt(8);
        translate([8,-76,83])rotate([0,90,0])blt(8);
        translate([8,-36,53])rotate([0,90,0])blt(8);
        translate([8,-36,83])rotate([0,90,0])blt(8);
    }
}//skelE();

module skelF(){
    //connects drive to front
    difference(){
        union(){
            translate([40-10,-25,24])cube([10,50,36]);
            translate([0,-81,24])cube([8,81+25,64]);
            translate([8,-10,24])cube([22,20,30]);
        }
        //holes for drive
        translate([40-10.2,20,24+5])rotate([0,90,0])
        cylinder(r=2.25,h=10.2,$fn=25);
        translate([40-10.2,20,60-5])rotate([0,90,0])
        cylinder(r=2.25,h=10.2,$fn=25);
        translate([40-10.2,-20,24+5])rotate([0,90,0])
        cylinder(r=2.25,h=10.2,$fn=25);
        translate([40-10.2,-20,60-5])rotate([0,90,0])
        cylinder(r=2.25,h=10.2,$fn=25);
        //holes for claw
        translate([0,-76,53])rotate([0,90,0])blt(8);
        translate([0,-76,83])rotate([0,90,0])blt(8);
        translate([0,-36,53])rotate([0,90,0])blt(8);
        translate([0,-36,83])rotate([0,90,0])blt(8);
        //stopper
        /*translate([0,-10,63])rotate([0,90,0])
        cylinder(r=2.25,h=8,$fn=20);
        translate([0,10,63])rotate([0,90,0])
        cylinder(r=2.25,h=8,$fn=20);*/
        translate([0,-10,78])rotate([0,90,0])
        cylinder(r=2.25,h=8,$fn=20);
        translate([0,10,78])rotate([0,90,0])
        cylinder(r=2.25,h=8,$fn=20);
    }
}//skelF();

module skelG1(){
    //holds servo for flag
    d=40;
    difference(){
        union(){
            cylinder(r=5,h=10,$fn=40);
            translate([-3.5,0,0])cube([3,15,10]);
            cube([5,d,10]);
            translate([0,40,0])cube([20,45,3]);
        }
        //rod
        cylinder(r=6.3/2,h=10,$fn=40);
        //gap
        translate([-0.5,0,0])cube([0.5,15,10]);
        translate([-3.5,10,5])rotate([0,90,0])
        cylinder(r=2.25,h=3.5+5,$fn=25);
        //servo
        translate([5,d+5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([5,d+45-5,0])cylinder(r=2.25,h=3,$fn=25);
    }
}//skelG1();

module skelG2(){
    difference(){
        cube([20,45,3]);
        translate([5,5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([5,40,0])cylinder(r=2.25,h=3,$fn=25);
    }
}skelG2();

module flagA(){
    //flags
    w=22;
    l=3200/w;
    h=150;
    difference(){
        union(){
            translate([h,0,0])cube([w+2+w,5,3]);
            translate([h,-l,0])cube([w,l,1]);
            translate([h+w+2,-l,0])cube([w,l,1]);
        }
        translate([h,0,3/2])cube([10,5,3/2]);
    }
}//flagA();

module flagB(){
    //flag pole
    w=22;
    l=3200/w;
    h=150;
    difference(){
        union(){
            translate([0,5/2,0])cylinder(r=5,h=3,$fn=30);
            cube([h,5,3]);
            translate([h,0,3/2])cube([10,5,3/2]);
        }
        //TODO: hole for servo
    }
}//flagB();
