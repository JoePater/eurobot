module sissorA(l=100,w=40){
    //H shape
    x=8;
    difference(){
        union(){
            cube([x,l+x,x]);
            translate([w-x,0,0])cube([x,l+x,x]);
            translate([0,x/2+l/2-20-x/2,0])cube([w,x,x]);
            translate([0,x/2+l/2+20-x/2,0])cube([w,x,x]);
        }
        translate([0,x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
    }
}
module sissorAx(l=100,w=40){
    //H with raised holes
    x=8;
    difference(){
        union(){
            cube([x,l+x,x]);
            translate([w-x,0,0])cube([x,l+x,x]);
            translate([0,x/2+l/2-20-x/2,0])cube([w,x,x]);
            translate([0,x/2+l/2+20-x/2,0])cube([w,x,x]);
            translate([0,0,8])cube([8,16,8]);
            translate([w-8,0,8])cube([8,16,8]);
        }
        translate([0,x/2,12])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,12,12])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=w,$fn=20);
    }
}
module sissorB(l=100){
    //stick
    x=8;
    difference(){
        cube([x,l+x,x]);
        translate([0,x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
    }
}
module sissorBx(l=100){
    //raised stick
    x=8;
    difference(){
        union(){
            cube([x,l+x,x]);
            translate([0,0,x])cube([x,12,10]);
        }
        translate([0,6,3*x/2])rotate([0,90,0])cylinder(r=4,h=x,$fn=20);
        translate([0,x/2+l/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
        translate([0,l+x/2,x/2])rotate([0,90,0])cylinder(r=2.25,h=x,$fn=20);
    }
}
module partM1a(){
    //axle holder thing for lighthouse
    difference(){
        union(){
            cube([30,76,4]);
            translate([0,0,4])cube([20,8,25]);
            translate([0,76-8,4])cube([20,8,25]);
        }
        translate([10,0,4+15])rotate([-90,0,0])cylinder(r=4,h=80,$fn=30);
        
        translate([25,5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([25,71,0])cylinder(r=2.25,h=4,$fn=25);
        translate([10,15,0])cylinder(r=2.25,h=4,$fn=25);
        translate([10,61,0])cylinder(r=2.25,h=4,$fn=25);
    }
}
module partM1b(d=25){
    //holds motor for LH
    difference(){
        union(){
            translate([0,0,4])cube([16.25+d,25,22]);
            translate([0,-10,0])cube([5+6.25+d+5,45,4]);
        }
        
        translate([5+6.25+d,0,4+22])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=30);
        translate([5+6.25+d,10,4+22])rotate([-90,0,0])cylinder(r=4,h=3.5,$fn=30);
        translate([5,0,4+22-5])cube([12.5,25,5]);
        
        translate([5+6.25+d-10,15,0])cylinder(r=2.25,h=30,$fn=25);
        translate([5+6.25+d-10,15,0])cylinder(r1=5,r2=2.25,h=4,$fn=25);
        
        translate([5,-5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([5+6.25+d,30,0])cylinder(r=2.25,h=4,$fn=25);
        translate([5+6.25+d,-5,0])cylinder(r=2.25,h=4,$fn=25);
        translate([5,30,0])cylinder(r=2.25,h=4,$fn=25);
    }
}
module partM2(d=25){
    //motor holder cap for LH
    difference(){
        cube([16.25+d,25,8]);
        
        translate([5,0,0])cube([12.5,25,5]);
        translate([5+6.25+d-10,15,0])cylinder(r=2.25,h=30,$fn=25);
        //translate([25,15,0])cylinder(r=2.25,h=30,$fn=25);
        
        translate([5+6.25+d,0,0])rotate([-90,0,0])cylinder(r=2.25,h=25,$fn=30);
        translate([5+6.25+d,10,0])rotate([-90,0,0])cylinder(r=4,h=3.5,$fn=30);
    }
}
module partM3(){
    //string gear thing for LH
    difference(){
        union(){
            translate([0,0,2.5])gear(5,25,5,0);
            translate([0,0,5])cylinder(r1=13,r2=5,h=5,$fn=30);
            translate([0,0,10])cylinder(r=5,h=5,$fn=30);
            translate([0,0,15])cylinder(r1=5,r2=10,h=5,$fn=30);
        }
        cylinder(h=25,r=2.25,$fn=25);
        translate([1+5,0,15])cylinder(r=1,h=5,$fn=15);
    }
}
module partM4(r=2.25){
    //string pulley wheel for LH
    difference(){
        union(){
            cylinder(r1=8,r2=6,h=2,$fn=30);
            translate([0,0,1.5])cylinder(r1=6,r2=8,h=2,$fn=30);
        }
        cylinder(r=r,h=4,$fn=25);
    }
}
module partM4b(){
    //washer for LH
    difference(){
        cylinder(r=6,h=12.5,$fn=30);
        cylinder(r=4,h=12.5,$fn=30);
    }
}
module partM5(){
    //axle for LH
    difference(){
        union(){
            intersection(){
                rotate([-90,0,0])cylinder(r=3.75,h=88,$fn=40);
                translate([-20,-10,-3])cube([40,108,6]);
            }
            translate([-8,-3,-3])cube([16,3,6]);
        }
        translate([0,84,0])cylinder(r=2.25,h=10,$fn=20,center=true);
    }
        
}
module partM6(){
    //track thing for LH
    difference(){
        cube([110,10,22]);
        translate([8,0,15-2.25])cube([94,10,4.5]);
        
        translate([106,5,0])cylinder(r=2.25,h=30,$fn=20);
        translate([4,5,0])cylinder(r=2.25,h=30,$fn=20);
    }
}
module partM7(){
    //motor gear for LH
    difference(){
        translate([0,0,2.5])gear(5,6,5,0);
        linear_extrude(5)motorHole();
    }
}
module partN1(){
    //top platform
    difference(){
        union(){
            cube([115,22,3]);
            cube([10,4,3+8]);
            translate([0,18,0])cube([10,4,3+8]);
        }
        
        translate([5,0,3+4])rotate([-90,0,0])cylinder(r=2.25,h=80,$fn=25);
        translate([20,5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([20,22-5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([50,5,0])cylinder(r=2.25,h=3,$fn=25);
        translate([50,22-5,0])cylinder(r=2.25,h=3,$fn=25);
    }
}
module partN2(){
    //motor holders for top of LH
    difference(){
        union(){
            cube([15,30,3]);
            translate([10,0,0])cube([10,30,25]);
        }
        translate([15,15-6.25,0])cube([5,12.5,25]);
        
        translate([5,15-6,0])cylinder(r=2.25,h=3,$fn=25);
        translate([5,15+6,0])cylinder(r=2.25,h=3,$fn=25);
        
        translate([10,4,15])rotate([0,90,0])cylinder(h=10,r=2.25,$fn=25);
        translate([10,26,15])rotate([0,90,0])cylinder(h=10,r=2.25,$fn=25);
    }
}
module partN3(){
    //spinning light platform
    difference(){
        cube([20,20,5]);
        translate([10,10,0])linear_extrude(5)
        motorHole();
    }
}
module boltAlt(){
    //bolts for holding scissor lift together
    translate([0,-1.5,0])cube([6,3,3],center=true);
    intersection(){
        rotate([-90,0,0])cylinder(r=1.8,h=20,$fn=25);
        cube([10,50,3],center=true);
    }
}








