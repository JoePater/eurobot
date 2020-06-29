#include "region.h"
#include "bitmap.h"

#include <ctime>

void testLine(const Line& l1,const Bitmap& bm){
    std::cout<<"Line L = "<<l1<<"\n";
    std::clock_t start = std::clock();
    std::optional<Position> p1 = bm.closestSetBit(l1);
    if(p1){
        std::cout<<"Line L meets BM at pt "<<*p1;
    }else{
        std::cout<<"Line L doesn't meet BM.";
    }
    std::cout<<" Took "<<std::clock()-start<<"\n\n";
}

int main(int argc, char** argv){
    Bitmap b1(2000,3000);
    Rectangle r(Point(500,2000),Point(1500,1000));

    std::clock_t start = std::clock();
    Circle c1(1000,Point(300,200));
    b1.setRegion(&c1);
    std::clock_t mid = std::clock();
    Circle c2(200,Point(300,200));
    b1.clearRegion(&c2);
    std::clock_t stop = std::clock();

    std::cout<<"Annulus (B1). Time to set: "<<(mid-start);
    std::cout<<" Time to clear: "<<(stop-mid)<<"\n";
    b1.print();
    std::cout<<" Time to print: "<<(std::clock()-stop)<<"\n";

    Bitmap b2(2000,3000);
    start = std::clock();
    Rectangle r1(Point(200,2000),Point(400,200));
    b2.setRegion(&r1);
    stop = std::clock();
    std::cout<<"\nRectangle (B2). Time was "<<(stop-start)<<":\n";
    b2.print();

    start = std::clock();
    Bitmap b3 = b2 & b1;
    mid = std::clock();
    Bitmap b4 = b2 | b1;
    stop = std::clock();
    std::cout<<"\nB3 = B1&B2. Time was "<<mid-start<<":\n";
    b3.print();
    std::cout<<"\nB4 = B1|B2. Time was "<<stop-mid<<"\n";
    b4.print();

    start = std::clock();
    std::cout<<"Region C2 of B2 is clear? "<<b2.areaIsClear(&c2)<<" took "<<std::clock()-start<<"\n";

    start = std::clock();
    std::cout<<"Region C2 of B2 is set? "<<b2.areaIsSet(&c2)<<" took "<<std::clock()-start<<"\n";

    testLine(Line(Point(0,0),Point(500,2000)),b2);
    testLine(Line(Point(0,200),Point(350,210)),b2);
    testLine(Line(Point(400,0),Point(300,2000)),b2);
    testLine(Line(Point(500,1000),Point(300,500)),b2);
    testLine(Line(Point(300,2100),Point(500,2000)),b2);
    testLine(Line(Point(300,2100),Point(500,500)),b2);

    return 0;
}
