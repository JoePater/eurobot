//#include "system.h"
//#include "camera.h"

//#include <thread>
#include "region.h"
#include <cmath>


int main(int argc, char** argv){
    /*System sys;
    while(!sys.getRobot().mechanisms.startStringPulled()); //wait for start

    std::thread camThread(camMain,&sys);

    while(!sys.getRobot().mechanisms.stopButtonPressed()){
        sys.update();
    }*/
    Polygon p1;
    for(double i=0;i<6.28;i+=0.2){
        p1.addVertex(Point(std::cos(i),std::sin(i))*20);
    }
    Polygon p2;
    p2.addVertex(Point(0,0));
    p2.addVertex(Point(5,0));
    p2.addVertex(Point(5,30));
    p2.addVertex(Point(0,30));
    std::cout<<"Polygon 1: "<<p1<<"\n";
    std::cout<<"Polygon 2: "<<p2<<"\n";
    std::cout<<"Overlaps?: "<<p1.overlaps(p2)<<"\n";
    std::cout<<"\tTranslating p2 by (30,30)...\n";
    p2.translate(30,30);
    std::cout<<"Polygon 2: "<<p2<<"\n";
    std::cout<<"Overlaps?: "<<p1.overlaps(p2)<<"\n";
    std::cout<<"P1 contains (0,0)?: "<<p1.contains(Point(0,0))<<"\n";
    std::cout<<"P1 contains (30,30)?: "<<p1.contains(Point(30,30))<<"\n";
    std::cout<<"P2 closest point to (35,0): "<<p2.closestPoint(Point(35,0))<<"\n";
    std::cout<<"P2 closest point to (0,0): "<<p2.closestPoint(Point(0,0))<<"\n";
    std::cout<<"P1 top left/bottom right: "<<p1.getTopLeft()<<" / "<<p1.getBottomRight()<<"\n";
    std::cout<<"\n\n";

    Rectangle r1(Point(-4,6),Point(2,-1));
    std::cout<<"Rectangle 1: "<<r1<<"\n";
    r1.setTopLeft(Point(0,5));
    std::cout<<"Rectangle 1: "<<r1<<"\n";
    r1.setBottomRight(Point(5,0));
    std::cout<<"Rectangle 1: "<<r1<<"\n\n";

    Circle c1(5,Point(5,5));
    std::cout<<"Circle 1: "<<c1<<"\n";
    c1.setRad(10);
    std::cout<<"Circle 1: "<<c1<<"\n";
    c1.setCenter(Point(10,10));
    std::cout<<"Circle 1: "<<c1<<"\n";
    return 0;
}
