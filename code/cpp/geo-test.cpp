//#include "system.h"
//#include "camera.h"

//#include <thread>
#include "geometry.h"


int main(int argc, char** argv){
    /*System sys;
    while(!sys.getRobot().mechanisms.startStringPulled()); //wait for start

    std::thread camThread(camMain,&sys);

    while(!sys.getRobot().mechanisms.stopButtonPressed()){
        sys.update();
    }*/
    Point pt1(3,5);
    std::cout<<"Point 1: "<<pt1<<"\n";
    Point pt2(4,7);
    std::cout<<"Point 2: "<<pt2<<"\n";
    Point pt3(pt1+pt2);
    std::cout<<"Point 3 (pt1+pt2): "<<pt3<<"\n";
    std::cout<<"Point 1 * 5: "<<pt1*5<<" = "<<5*pt1<<"\n";
    std::cout<<"Point 1 / 5: "<<pt1/5<<"\n";
    std::cout<<"Distance 1 to 2: "<<getDistance(pt1,pt2)<<"\n";
    std::cout<<"Gradient 1 to 2: "<<getGradient(pt1,pt2)<<"\n";
    std::cout<<"Angle 1 to 2: "<<getAngle(pt1,pt2)<<"\n\n";

    Line l1(pt1,pt2);
    std::cout<<"Line 1: "<<l1<<"\n";
    std::cout<<"Length: "<<l1.getLength()<<"\n";
    std::cout<<"Point 1 unit from end: "<<l1.getPoint(1)<<"\n";
    std::cout<<"Gradient: "<<l1.getGradient()<<"\n";
    std::cout<<"Angle: "<<l1.getAngle()<<"\n\n";
    Line l2(Point(4,4),Point(3,8));
    std::cout<<"Line 2: "<<l2<<"\n";
    std::cout<<"Intersects?: "<<intersects(l1,l2)<<"\n";
    std::cout<<"Closest point/distance to:\n";
    std::cout<<"\t(0,0): "<<getLineClosestPoint(Point(0,0),l1);
    std::cout<<" / "<<getDistToLine(Point(0,0),l1)<<"\n";
    std::cout<<"\t(4,5): "<<getLineClosestPoint(Point(4,5),l1);
    std::cout<<" / "<<getDistToLine(Point(4,5),l1)<<"\n";
    std::cout<<"\t(10,10): "<<getLineClosestPoint(Point(10,10),l1);
    std::cout<<" / "<<getDistToLine(Point(10,10),l1)<<"\n";
    l2 = Line(Point(10,4),Point(3,8));
    std::cout<<"Line 2: "<<l2<<"\n";
    std::cout<<"Intersects?: "<<intersects(l1,l2)<<"\n";
    return 0;
}
