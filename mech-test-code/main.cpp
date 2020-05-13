#include "mechanisms.h"
#include <string>
#include <vector>
#include <iostream>

MotorRoutine storoutine(std::string s){
	if(s=="CLAW_DOWN")return CLAW_DOWN;
	if(s=="CLAW_DOWN_EDGE")return CLAW_DOWN_EDGE;
	if(s=="CLAMP")return CLAMP;
	if(s=="RELEASE")return RELEASE;
	if(s=="CLAW_UP")return CLAW_UP;
	if(s=="CLAW_UP_EDGE")return CLAW_UP_EDGE;
	if(s=="STOP_ALL")return STOP_ALL;
	return STOP_ALL;
}
Buoy stobuoy(std::string s){
	if(s=="EMPTY")return EMPTY;
	if(s=="RED")return RED;
	if(s=="GREEN")return GREEN;
	return EMPTY;
}

int main(){
	Mechanisms mech;
	while(1){
		std::vector<std::string> func;
		std::string in;
		std::getline(std::cin,in);
		try{
			std::string::iterator start = in.begin();
			for(auto iter=in.begin();iter<in.end();++iter){
				if(*iter==' '){
					func.push_back(std::string(start,iter));
					start = iter + 1;
				}
			}
			std::string end(start,in.end());
			if(!end.empty())func.push_back(end);
			for(auto i:func){
				std::cout<<"-"<<i<<"-"<<"\n";
			}
			if(func[0]=="setDrive"){
				mech.setDrive(std::stof(func[1]),std::stof(func[2]));
			}else if(func[0]=="setRoutine"){
				mech.setRoutine(storoutine(func[1]),stobuoy(func[2]));
			}else if(func[0]=="setClawClamp"){
				mech.setClawClamp(std::stoi(func[1]),std::stoi(func[2]));
			}else if(func[0]=="setRevolverRotation"){
				mech.setRevolverRotation(std::stoi(func[1]));
			}else if(func[0]=="getClawRoutine"){
				std::cout<<mech.getClawRoutine()<<"\n";
			}else if(func[0]=="getClawProgress"){
				std::cout<<mech.getClawProgress()<<"\n";
			}else if(func[0]=="getRevProgress"){
				std::cout<<mech.getRevProgress()<<"\n";
			}else if(func[0]=="setCupHolderOpen"){
				mech.setCupHolderOpen(std::stoi(func[1]));
			}else if(func[0]=="setCupHolderWaiting"){
				mech.setCupHolderWaiting(std::stoi(func[1]));
			}else if(func[0]=="setCupHolderClosed"){
				mech.setCupHolderClosed(std::stoi(func[1]));
			}else if(func[0]=="getCupColor"){
				std::cout<<"Buoy: "<<mech.getCupColor()<<"\n";
			}else{
				std::cout<<"Error 1\n";
			}
		}catch(...){std::cout<<"Error 2\n";}
	}
	return 0;
}
		
