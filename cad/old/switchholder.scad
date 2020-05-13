module bolt(){
    cylinder(r=2.25,h=25,$fn=20);
}

bolt1 = 20;
difference(){
    cube([35,10,8]);
    translate([15,0,3])cube([20,10,5]);
    translate([bolt1,5,0])bolt();
    translate([(bolt1+10),5,0])bolt();
}