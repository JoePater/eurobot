include <utils.scad>;
//include <gear.scad>;

module partK1(h=97-5,mh=47-5){
    //big base for claw
    difference(){
        union(){
            cube([40+7,50,15]);
            translate([0,25-6,0])cube([40,12,h-10]);
            translate([0,6,h-10])cube([40,25,18]);
            translate([0,6,mh-20])cube([15-3,13,40]);
        }
        translate([5+7,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([35+7,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([5+7,45,0])cylinder(r=2.25,h=15,$fn=20);
        translate([35+7,45,0])cylinder(r=2.25,h=15,$fn=20);
        
        translate([25,36,h-10])cylinder(r=2.25,h=20,$fn=20);
        translate([5,36,h-10])cylinder(r=2.25,h=20,$fn=20);
        
        translate([20-3,6,h])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=20);
        
        translate([0,6+5,mh-15])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=20);
        translate([0,6+5,mh+15])rotate([0,90,0])
        cylinder(r=2.25,h=15,$fn=20);
        
        translate([15-3,19,mh-6.3])cube([28,12,12.6]);
    }
}
module partK2(){
    //holds motor in place for lifter
    difference(){
        cube([40,13,15]);
        translate([5,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([35,5,0])cylinder(r=2.25,h=15,$fn=20);
        translate([20-6.3,0,5])cube([12.6,13,10]);
    }
}
module partK3(){
    //little gear for lifter
    difference(){
        translate([0,0,3])gear(5,8,6,0);
        linear_extrude(6)motorHole(0.1);
    }
}
module partU(){
    //front wheel holder
    difference(){
        union(){
            cube([54,12,8]);
            cube([16,38,8]);
            translate([8,30,8])cylinder(r=8,h=19,$fn=40);
        }
        translate([8,30,0])cylinder(r=3.9,h=8+19,$fn=40);
        translate([48,6,0])cylinder(r=2.25,h=18,$fn=25);
        translate([18,6,0])cylinder(r=2.25,h=18,$fn=25);
    }
}




module partI1(a=15){
    //big gear for claw
    difference(){
        translate([0,0,-4])cylinder(r=10,h=12,$fn=40);
        translate([0,0,-4])cylinder(r=2.15,h=12,$fn=20);
    }
    difference(){
        union(){
            
            translate([0,0,4])gear(5,55,8,4.5);
        }
        cylinder(r=2.15,h=8,$fn=20);
        translate([-150,-150,0])cube([300,140,8]);
        translate([-150,-150,0])cube([140,300,8]);
        //translate([-1,a+5])circle(r=2.25,$fn=20);
        translate([-1,a+15,0])cylinder(r=2.25,h=8,$fn=20);
        translate([19,a+5,0])cylinder(r=2.25,h=8,$fn=20);
        translate([19,a+15,0])cylinder(r=2.25,h=8,$fn=20);
    }
}
module partI2(b=35+12,c=25.5){
    //ataches claw to lifting gear
    linear_extrude(8)difference(){
        translate([0,0])square([25+12,20]);
            
        //translate([5,5])circle(r=2.25,$fn=20);
        translate([12,15])circle(r=2.25,$fn=20);
        translate([20+12,5])circle(r=2.25,$fn=20);
        translate([20+12,15])circle(r=2.25,$fn=20);
    }
    cube([12,10,b-6-8]);
    difference(){
        translate([0,0,b-6-8])cube([36,25,6]);
        
        translate([5+c,0,b-8])rotate([-90,0,0])
        cylinder(r=2.15,h=25,$fn=25);
        translate([5+c,8,b-8])rotate([-90,0,0])
        cylinder(r=4,h=3.3,$fn=25);
        
        translate([22,5,b-8-6])cylinder(r=2.25,h=6,$fn=20);
        translate([22,20,b-8-6])cylinder(r=2.25,h=6,$fn=20);
       
    }
}
module partI3(c=25.5){
    //cap, holds motor and pivot for claw
    d = 19;
    difference(){
        cube([36,25,13]);
        
        translate([31-c,0,13])rotate([-90,0,0])
        cylinder(r=2.15,h=25,$fn=25);
        translate([31-c,8,13])rotate([-90,0,0])
        cylinder(r=4,h=3.3,$fn=25);
        
        translate([31-c+d-6.2,0,13-10])cube([12.4,25,20]);
        
        translate([36-22,5,0])cylinder(r=2.25,h=13,$fn=20);
        translate([36-22,20,0])cylinder(r=2.25,h=13,$fn=20);
    }
}
module partJ(){
    //little gear for cup rotater
    difference(){
        translate([0,0,2])gear(4.95,6,4,0);
        translate([0,0,1])linear_extrude(3)motorHole(0.1);
    }
}
module partH(){
    //nut-gear
    difference(){
        translate([0,0,2.5])gear(4.95,19,5,0);
        
        translate([0,0,1.8])cylinder(r=3.95,h=3.2,$fn=6);
        cylinder(r=2.25,h=5,$fn=20);
    }
}

//hello

module partB1(h=7,l=32,w=28,d=20){
    //claw pivot holder thing
    difference(){
        cube([l,w,h]);
        translate([6,0,h])rotate([-90,0,0])cylinder(h=w,r=2.1,$fn=30);
        translate([6,5,h])rotate([-90,0,0])cylinder(h=3.2,r=5,$fn=30);
        
        translate([d+6,0,h])rotate([-90,0,0])cylinder(h=w,r=2.1,$fn=30);
        translate([d+6,5,h])rotate([-90,0,0])cylinder(h=3.2,r=5,$fn=30);
        
        translate([16,22,0])cylinder(h=h,r=2.25,$fn=20);        
        translate([16,22,0])cone1();
    }
}
module clawR(a=50,b=25){
    mirror([1,0,0])difference(){
        union(){
            rotate([0,0,-30])gearSection(65,20,6);
            translate([-4,-4,0])cube([8,a+4-15,6]);
            translate([-0.7,26,0])rotate([0,0,35]){
                cube([6,45,6]);
                translate([-5,25,0])cube([5,10,6]);
            }
            linear_extrude(6)translate([10,35+a,0])rotate(155)arc(36.8,39.8,65);
            translate([10,85,0])linear_extrude(11.5,scale=1+(18/72/10)){
                rotate([0,0,-27])translate([-33-5.5,-5,0])square([5.5,5]);
                rotate([0,0,-10])translate([-36-2.5,-2.5,0])square([2.5,5]);
                rotate([0,0,55])translate([-36-6,-2.5,0])square([6,5]);
            }
            translate([-21,-6,0])rotate([0,0,15])cube([25,25,6]);
            translate([4,14,1.5])rotate([0,90,0])cylinder(r=2.1,h=4,$fn=30);
        }
        cylinder(r=2.25,h=6,$fn=20);
        translate([0,b,0])cylinder(r=2.15,h=6,$fn=20);
        translate([-19,50,0])cylinder(r=1.75,h=6,$fn=20);
        translate([0,-8,0])cube([15,4,6]);
        translate([-16,-5,3])rotate([0,0,15])cube([12.3,26,3]);
        translate([0,0,-5])cube([50,50,5]);
    }
}
module clawL(a=50,b=25){
    difference(){
        union(){
            rotate([0,0,-30-180/20])gearSection(80,20,6);
            translate([-4,-4,0])cube([8,a+4-15,6]);
            translate([-0.7,26,0])rotate([0,0,35]){
                cube([6,45,6]);
                translate([-5,20,0])cube([5,10,6]);
            }
            linear_extrude(6)translate([10,35+a,0])rotate(155)arc(36.8,39.8,65);
            translate([10,85,0])linear_extrude(11.5,scale=1+(18/72/10)){
                rotate([0,0,-27])translate([-33-5.5,-5,0])square([5.5,5]);
                rotate([0,0,-10])translate([-36-2.5,-2.5,0])square([2.5,5]);
                rotate([0,0,55])translate([-36-6,-2.5,0])square([6,5]);
            }
            translate([4,14,1.5])rotate([0,90,0])cylinder(r=2.1,h=4,$fn=30);
        }
        cylinder(r=2.25,h=6,$fn=20);
        translate([0,b,0])cylinder(r=2.15,h=6,$fn=20);
        translate([-16,46,0])cylinder(r=1.75,h=6,$fn=20);
        translate([0,-8,0])cube([15,4,6]);
        translate([0,0,-5])cube([50,50,5]);
    }
}
module clawRa(){
    //pulley for claw
    difference(){
        union(){
            cylinder(r=2.5,h=8,$fn=30);
            translate([0,0,8])cylinder(r=6,h=2,$fn=30);
        }
        cylinder(r=1.75,h=10,$fn=30);
    }
}
module clawLa(){
    //winch thing
    mirror([1,0,0])difference(){
        union(){
            rotate_extrude($fn=50)difference(){
                union(){square([10,4.5]);
                square([12.5,2]);}
                translate([10.2,3])circle(r=1,$fn=20);
            }
            cylinder(r=3,h=6,$fn=30);
            translate([0,0,6])cylinder(r1=3,r2=7,h=3,$fn=30);
            translate([0,0,9])cylinder(r=7,h=0.5,$fn=30);
            translate([11,-5,0])rotate([0,-60,0])cube([3,5,4.5]);
            translate([8,2.5,3.8])rotate([0,45,0])cylinder(r=1,h=3,$fn=20);
        }
        linear_extrude(15)motorHole();
        translate([8,-7.5,0])cylinder(r=0.8,h=2,$fn=20);
        translate([8.8,0,3])cube([2,1.5,3]);
        translate([10.9,-5,3.5])rotate([0,-60,0])cube([2,5,2]);
    }
}
module partX(){
    //spring raiser for claw
    difference(){
        cylinder(r=4,h=1,$fn=30);
        cylinder(r=2.2,h=1,$fn=30);
    }
}
