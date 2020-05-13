h=8;
l=55;
w=30;
d=31;
module a(){
    difference(){
        cube([l,w,h]);
        union(){
            translate([10-0.1,0,h-5])cube([12+0.2,w,5]);
            translate([d+16,0,h])rotate([-90,0,0])cylinder(h=w,r=2.1,$fn=30);
            translate([d+16,5,h])rotate([-90,0,0])cylinder(h=3.2,r=4.1,$fn=30);
            translate([d+16,25,h])rotate([-90,0,0])cylinder(h=3.2,r=4.1,$fn=30);
            translate([5,5,0])cylinder(h=h,r=2.25,$fn=20);
            translate([5,25,0])cylinder(h=h,r=2.25,$fn=20);
            translate([35,5,0])cylinder(h=h,r=2.25,$fn=20);
            translate([35,25,0])cylinder(h=h,r=2.25,$fn=20);
        }
    }
}
module parts(){
    translate([10+0.2,6,h-5+0.2])cube([12-0.4,24,10]);
    translate([16,w,h])rotate([-90,0,0])cylinder(h=5,r=1,$fn=30);
    translate([d+16,0,h])rotate([-90,0,0])cylinder(h=w+5,r=1.8,$fn=30);
}
//parts();
a();
translate([-5,0,0])mirror([1,0,0])a();