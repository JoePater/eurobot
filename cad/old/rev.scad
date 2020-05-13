module a(){
    circle(r=80,$fn=80);//65/cos(54)
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
difference(){
    linear_extrude(4)a();
    union(){
            
        linear_extrude(5)c();
        cylinder(r=2.25,h=5,$fn=20);
    }
}