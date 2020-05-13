//#include "system.h"
//#include "camera.h"

//#include <thread>
#include "mechanisms.h"
#include <cmath>


int main(int argc, char** argv){
    Mechanisms m;
    /*
    std::cout<<"Mechanisms: "<<m<<"\n";
    m.setDrive(100,100);
    m.setDrive(-100,-100);
    m.setDrive(-50,50);
    m.setDrive(-50,100);

    m.setRoutine(CLAW_DOWN);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAW_DOWN_EDGE);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAMP,RED);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(RELEASE);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAW_UP);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAW_UP_EDGE);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAW_RIGHT_WAY_UP);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAW_UPSIDE_DOWN);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(STOP_ALL);
    std::cout<<"Mechanisms: "<<m<<"\n\n";
    */
    m.setRevolverRotation(3);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAMP,RED);

    m.setCupHolderClosed(-1);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    m.setRoutine(CLAMP,GREEN);

    m.setCupHolderClosed(1);
    std::cout<<"Mechanisms: "<<m<<"\n\n";

    const Revolver& r = m.getRevolver();
    std::cout<<"Next free: "<<r.nextFree()<<" Next red: "<<r.nextRed();
    std::cout<<" Next green: "<<r.nextGreen()<<" Next buoy: "<<r.nextBuoy()<<"\n";

    m.setRevolverRotation(r.nextFree());
    std::cout<<m<<"\n\n";

    m.setRevolverRotation(r.nextGreen());
    m.setCupHolderOpen(-1);
    std::cout<<m<<"\n\n";
    return 0;
}





























