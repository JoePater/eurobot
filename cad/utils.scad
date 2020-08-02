use <gear.scad>
module intHoles(){
    linear_extrude(5){
        translate([0,0])circle(r=2.25,$fn=20);
        translate([30,0])circle(r=2.25,$fn=20);
        translate([0,30])circle(r=2.25,$fn=20);
        translate([30,30])circle(r=2.25,$fn=20);
    }
}
module cone1(){
    cylinder(r1=8/2,r2=3.5/2,h=3,$fn=20);
}
module motorHole(x=0.15){
    //mini motor
    difference(){
        circle(r=1.45+x,$fn=30);
        translate([0.9+1.5+2*x,0])square([3,3],center=true);
    }
}
module gearSection(a,n,h,r=0){
    intersection(){
        translate([0,0,h/2])rotate([0,0,r])gear(3.13,n,h,0);
        linear_extrude(h)polygon(points=[[0,0],[100,0],
        [100*cos(a/5),100*sin(a/5)],
        [100*cos(2*a/5),100*sin(2*a/5)],[100*cos(3*a/5),100*sin(3*a/5)],
        [100*cos(4*a/5),100*sin(4*a/5)],[100*cos(a),100*sin(a)]]);
    }
}
module semicircle(r1,r2){
    difference(){
        circle(r=r2,$fn=50);
        translate([-r2,-r2])square([2*r2,r2]);
        circle(r=r1,$fn=50);
    }
}
module arc(r1,r2,x){
    intersection(){
        semicircle(r1,r2);
        rotate(180+x)semicircle(r1,r2);
    }
}
module littleMotor(){
    cube([12,25,10]);
    translate([6,25,5])rotate([-90,0,0])linear_extrude(10)motorHole();
}
module bolt(){
    cylinder(r=2.25,h=25,$fn=20);
}
module dmotorHole(x=0.1){
    difference(){
        circle(r=2+x,$fn=25);
        translate([3.4-2+x,-2.5])square([3,5]);
    }
}
module blt(h){
    cylinder(r=2.25,h=h,$fn=25);
}