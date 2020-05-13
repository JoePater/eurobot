module claws2D(){
    rotate(142.4)translate([72.7,-56.3])difference(){
        union(){
            translate([-102,33])square([5,5]);
            translate([-75,55])square([5,5]);
            projection()translate([-300,-700,110])scale(1000)import("/home/joe/STLs/gear_claw_main_changed.stl");
            
        }
        translate([-87,22])rotate(55)square([5,5]);
    }
    
}
module clawsTog2D(){
    rotate(142.4)translate([72.7,-56.3])difference(){
        union(){
            //translate([-102,33])square([5,5]);
            //translate([-75,55])square([5,5]);
            projection()translate([-300,-700,110])scale(1000)import("/home/joe/STLs/gear_claw.stl");
            
        }
        //translate([-87,22])rotate(55)square([5,5]);
    }
    
}
module clawSizeTest(){
    circle(r=31.5,$fn=80);
    scale(1.38)translate([-12.3,-36.5])clawsTog2D();
}
module motorHole(x=0.1){
    difference(){
        circle(r=1.45+x,$fn=30);
        translate([0.9+1.5+2*x,0])square([3,3],center=true);
    }
}
module completeClaws(){
    linear_extrude(4)difference(){
        scale(1.38)claws2D();
        translate([46,0])motorHole(0.1);
        circle(r=2.25,$fn=20);
    }
    linear_extrude(8)difference(){
        scale(1.38)claws2D();
        translate([-20,-20])square([90,35]);
    }
}
module motorTest(){
    linear_extrude(2)difference(){
        translate([-2.5,-2.5])square([25,5]);
        for(i=[0:4]){
            translate([i*5,0])motorHole(i*0.025);
        }
    }
}

//motorTest();
completeClaws();
//clawSizeTest();

