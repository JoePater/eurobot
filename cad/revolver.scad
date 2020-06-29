include <utils.scad>

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


module bigGear(){
    difference(){
        union(){
            translate([0,0,2.5])gear(5,50,5,0);
            cylinder(r=47/2+5,h=15,$fn=80);
        }
        translate([0,0,2])cylinder(r=47/2+0.1,h=13,$fn=80);
        cylinder(r=2.25,h=2,$fn=30);
        intersection(){
            translate([3,-6,0])cube([5,12,2]);
            cylinder(r=7.5,h=5,$fn=50);
        }
    }
}//bigGear();

module baseA(){
    difference(){
        union(){
            cube([40,32,6]);
            translate([5,16+3,6])rotate([90,0,0])
            linear_extrude(6)polygon(points=[
            [0,0],[0,40],[35,0]]);
            translate([0,0,6])cube([5,32,40]);
        }
        translate([5,5,0])cube([30,4.5,6]);
        translate([5,27-4.5,0])cube([30,4.5,6]);
        translate([0,16-2,0])cube([2.5,4,46]);
        
        translate([0,6,6])cube([5,4.5,12]);
        translate([0,26-4.5,6])cube([5,4.5,12]);
        translate([0,6,30])cube([5,4.5,12]);
        translate([0,26-4.5,30])cube([5,4.5,12]);
    }
}//baseA();
    
module baseB(){
   difference(){
      union(){
         cylinder(r=25/2-0.1,h=16,$fn=60);
         translate([0,-16,0])cube([30,32,4]);
      }
      translate([0,0,4])cylinder(r=8,h=12,$fn=50);
      cylinder(r=5,h=4,$fn=40);
      
      translate([-0.5,15.5/2,4])cylinder(r=4,h=16,$fn=30);
      translate([-0.5,15.5/2,0])cylinder(r=2.25,h=4,$fn=30);
      translate([-0.5,-15.5/2,4])cylinder(r=4,h=16,$fn=30);
      translate([-0.5,-15.5/2,0])cylinder(r=2.25,h=4,$fn=30);
      
      translate([-0.5+24,15.5/2,0])cylinder(r=2.25,h=4,$fn=30);
      translate([-0.5+24,-15.5/2,0])cylinder(r=2.25,h=4,$fn=30);
   }
}//baseB();

module baseAB(){
    baseA();
    translate([-0.2,16,12.5])rotate([0,-90,0])
    baseB();
}//baseAB();

module baseABG(){
    baseAB();
    translate([-5-14-0.4,16,12.5])rotate([0,90,0])
    bigGear();
}//baseABG();

module littleGear(){
    difference(){
        translate([0,0,2.5])gear(5,9,5,0);
        difference(){
            cylinder(r=3,h=5,$fn=40);
            translate([3-0.7,-5,0])cube([5,10,5]);
        }
    }
}//littleGear();

module baseC(){
    //holds motor
    difference(){
        union(){
            cube([24,68,9]);
            translate([0,34-23,0])cube([24,46,46]);
        }
        translate([0,34,23])rotate([0,90,0])
        cylinder(r=37/2+0.2,h=24,$fn=80);
        
        translate([0,34,4.5])cube([24,34,1.5]);
        
        translate([5,63,0])cylinder(r=2.25,h=9,$fn=25);
        translate([19,63,0])cylinder(r=2.25,h=9,$fn=25);
        translate([5,5,0])cylinder(r=2.25,h=9,$fn=25);
        translate([19,5,0])cylinder(r=2.25,h=9,$fn=25);
    }
}//baseC();

module baseCG(){
    baseC();
    a=270;
    translate([-14,34+8*sin(a),23+8*cos(a)])rotate([0,-90,0])
    littleGear();
}//baseCG();

module baseAll(){
    translate([0,35,0])baseCG();
    baseABG();
}//baseAll();

module photoGuide(){
    difference(){
        cube([30,9,6]);
        translate([15-2.5,4.5-3.5,0])cube([5,7,6]);
        translate([0,0,1.7])linear_extrude(4.3){
            polygon(points=[
            [0,0],[12.5,4.5-1.6],[17.5,4.5-1.6],[30,0],
            [30,9],[17.5,4.5+1.6],[12.5,4.5+1.6],[0,9]]);
        }
    }
}//photoGuide();

module holderA(){
    //sliding section
    a=atan((36-27)/115);
    translate([6,-10,0])difference(){
        union(){
            translate([8,0,0])cube([30-8,20,3]);
            translate([3,3.72+10,11.5/2])rack(4,7,11.5,5);
            translate([0,10,0])mirror([0,1,0])
            translate([3-4/2,3.72,11.5/2])rack(4,8,11.5,5);
            translate([27+3-6,7.5,11.5])cube([6,5,8]);
            translate([24,7.5,11.5+8])rotate([0,a,0])
            cube([6,5,10]);
        }
        translate([-3,0,0])cube([3,20,20]);
        translate([30,0,0])cube([3,20,20]);
        translate([30,7.5,11.5+8])cube([6,5,10]);
    }
}//holderA();

module holderB(){
    //gears
    n=15;
    difference(){
        union(){
            //cylinder(r=2.5,h=3+4.5,$fn=30);
            translate([0,0,3+4.5+2])gear(4,n,4,0);
        }
        cylinder(r=1.75,h=3+4.5+4,$fn=25);
    }
}//holderB();

module holderB2(){
    //gear with lever
    holderB();
    rotate([0,0,10])difference(){
        union(){
            translate([4,-4,7.5])cube([30,8,4]);
        }
        translate([4+30-5,0,7.5])cylinder(r=1.75,h=4,$fn=25);
    }
}//holderB2();

module holderC(){
    //slider cover
    dy=4;
    dx=19;
    ds=10.2;
    a=atan((dy+10)/dx);
    d=sqrt(pow(dy+10,2)+pow(dx,2));
    x=30;
    difference(){
            translate([ds/tan(60),ds,0])union(){
            linear_extrude(3.2){
                polygon(points=[[0,0],[x,0],
                [24*cos(60),24*sin(60)],
                [x*cos(120),x*sin(120)]]);
            }
            o=4;
            translate([0,0,3.2])linear_extrude(3.7){
                polygon(points=[[0,-o],[x,-o],[x,0],
                [24*cos(60),24*sin(60)],
                [x*cos(120),x*sin(120)],
                [x*cos(120)-o*sin(120),x*sin(120)+o*cos(120)],
                [-o*sin(120),o*cos(120)]]);
            }
        }
        rotate([0,0,a])translate([d,0,0])
        cylinder(r=1.75,h=7,$fn=25);
        rotate([0,0,120-a])translate([d,0,0])
        cylinder(r=1.75,h=7,$fn=25);
        rotate([0,0,60])translate([32,0,0])
        cylinder(r=1.75,h=7,$fn=25);
    }
}//holderC();

module holderD1(){
    //link for lever
    difference(){
        cube([20,8,3]);
        translate([4,4,0])cylinder(r=1.75,h=4,$fn=25);
        translate([16,4,0])cylinder(r=1.75,h=4,$fn=25);
    }
}

module holderD2(){
    //servo cap for lever
    difference(){
        union(){
            cube([10,20,3]);
            translate([5,5,3])cylinder(r=5,h=6,$fn=30);
        }
        translate([5,15,0])cylinder(h=3,r=1.75,$fn=20);
        translate([5,5,6])cylinder(h=6,r=2.4,$fn=20);
    }
}

module holderABC(){
    holderA();
    dy=4;
    dx=19;
    ds=10.2;
    translate([dx,-10-dy,0])rotate([0,0,180/15])
    holderB();
    translate([dx,dy+10,0])rotate([0,0,180/15])
    holderB2();
    holderC();
}//holderABC();

module holderABC2(){
    rotate([0,0,0])holderABC();
    rotate([0,0,120])holderABC();
    rotate([0,0,240])holderABC();
}//holderABC2();

module holderABCD(){
    holderABC2();
    translate([-44.7,27.4,4])rotate([0,0,100])
    translate([-16,-4,0])holderD1();
    translate([-42.5,15.6,1])rotate([0,0,-30])
    translate([-5,-15,0])holderD2();
}

module holderABCD2(){
    for(i=[0,72,144,216,288]){
        rotate([0,0,i])
        translate([70,0,0])holderABCD();
    }
}holderABCD2();






