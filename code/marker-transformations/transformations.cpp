#include "transformations.h"
#include "opencv2/calib3d.hpp"
#include <cmath>

#include "../constants.h"

//marker 51/71 for opponent
//map of markers defined here
//edge markers at height of 70+50
//id, rotation matrix, translation
std::map<int,std::pair<cv::Mat,cv::Mat>> markerMap{
    std::make_pair(42,std::make_pair((cv::Mat_<double>(3,3)<<0,-1,0, 1,0,0, 0,0,1),(cv::Mat_<double>(1,3)<<1250,1500,0))),
    std::make_pair(52,std::make_pair((cv::Mat_<double>(3,3)<<0,1,0, 0,0,1, 1,0,0),(cv::Mat_<double>(1,3)<<-22,1350,1000))),
    std::make_pair(72,std::make_pair((cv::Mat_<double>(3,3)<<0,1,0, 0,0,1, 1,0,0),(cv::Mat_<double>(1,3)<<-22,1650,1000))),
    std::make_pair(56,std::make_pair((cv::Mat_<double>(3,3)<<0,1,0, 0,0,1, 1,0,0),(cv::Mat_<double>(1,3)<<-22,225,120))),
    std::make_pair(76,std::make_pair((cv::Mat_<double>(3,3)<<0,1,0, 0,0,1, 1,0,0),(cv::Mat_<double>(1,3)<<-22,2775,120))),
    std::make_pair(53,std::make_pair((cv::Mat_<double>(3,3)<<1,0,0, 0,0,1, 0,-1,0),(cv::Mat_<double>(1,3)<<50,3044,120))),
    std::make_pair(73,std::make_pair((cv::Mat_<double>(3,3)<<-1,0,0, 0,0,1, 0,1,0),(cv::Mat_<double>(1,3)<<50,-44,120))),
    std::make_pair(75,std::make_pair((cv::Mat_<double>(3,3)<<1,0,0, 0,0,1, 0,-1,0),(cv::Mat_<double>(1,3)<<1000,3044,120))),
    std::make_pair(55,std::make_pair((cv::Mat_<double>(3,3)<<-1,0,0, 0,0,1, 0,1,0),(cv::Mat_<double>(1,3)<<1000,-44,120))),
    std::make_pair(54,std::make_pair((cv::Mat_<double>(3,3)<<1,0,0, 0,0,1, 0,-1,0),(cv::Mat_<double>(1,3)<<1950,3044,120))),
    std::make_pair(74,std::make_pair((cv::Mat_<double>(3,3)<<-1,0,0, 0,0,1, 0,1,0),(cv::Mat_<double>(1,3)<<1950,-44,120)))
};

//camera transform defined here
cv::Mat cam0Rot{(cv::Mat_<double>(3,3)<<0,0,0, 0,0,0, 0,0,0)};
cv::Mat cam0Trans{(cv::Mat_<double>(1,3)<<0,0,0)};

cv::Mat cam1Rot{(cv::Mat_<double>(3,3)<<0,0,0, 0,0,0, 0,0,0)};
cv::Mat cam1Trans{(cv::Mat_<double>(1,3)<<0,0,0)};

//camera calibration
cv::Mat cameraMatrix, distCoeffs;
//void initCalib();

//marker dictionary
cv::Ptr<cv::aruco::Dictionary> markerDict{cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000)};

std::pair<cv::Point2d,double> estimateRobotPosition(std::vector<std::vector<cv::Point2f>> corners,std::vector<int> ids,bool camera){
    //remove weathervane and opposition markers
    for(int i=0;i<ids.size();++i){
        if(ids[i]==17 || (ids[i]==51 && team==YELLOW) || (ids[i]==71 && team==BLUE)){
            corners.erase(corners.begin()+i);
            ids.erase(ids.begin()+i);
        }
    }
    if(corners.empty()) throw 1;
    //estimate pose
    std::vector<cv::Vec3d> rvecs,tvecs;
    cv::aruco::estimatePoseSingleMarkers(corners,0.07,cameraMatrix,distCoeffs,rvecs,tvecs);
    cv::Point2d ptsTotal;
    double bearingsTotal;
    //find average
    for(int i=0;i<ids.size();++i){
        //get marker transformation
        auto iter = markerMap.find(ids[i]);
        if(iter==markerMap.end())continue;
        cv::Mat rot1 = iter->second.first;
        cv::Mat trans1 = iter->second.second;
        //get marker-camera transformation
        cv::Mat rot2;
        cv::Rodrigues(rvecs[i],rot2);
        cv::Mat trans2 = (cv::Mat_<double>(1,3)<<tvecs[i][0],tvecs[i][1],tvecs[i][2]);
        //get robot-camera transformation
        cv::Mat rot3 = camera ? cam1Rot : cam0Rot;
        cv::Mat trans3 = camera ? cam1Trans: cam0Trans;

        //do calculations
        cv::Mat totalRot = rot3.inv()*rot2*rot1;
        cv::Mat totalTrans = trans1 + rot1*trans2 - rot1*rot2*rot3.inv()*trans3;

        //get point, add to total
        double x = totalTrans.at<double>(0,0);
        double y = totalTrans.at<double>(0,1);
        ptsTotal += (cv::Point2d(x,y));

        //get bearing, add to total
        cv::Mat unitX{(cv::Mat_<double>(1,3)<<1,0,0)};
        cv::Mat k = totalRot*unitX;
        double ang = -180/3.141593*std::atan(k.at<double>(0,0)/k.at<double>(0,1));
        bearingsTotal += (ang);
    }
    return std::make_pair(ptsTotal/static_cast<double>(ids.size()),bearingsTotal/static_cast<double>(ids.size()));
}

cv::Point2d estimateOpponent(cv::Point2d currPos,double bearing,std::vector<cv::Point2f> corners,bool camera=0){
    //get robot-camera transformations
    cv::Mat rot3 = camera ? cam1Rot : cam0Rot;
    cv::Mat trans3 = camera ? cam1Trans: cam0Trans;

    //get pose
    if(corners.empty()) throw 1;
    std::vector<cv::Vec3d> rvecs,tvecs;
    cv::aruco::estimatePoseSingleMarkers(std::vector<std::vector<Point2f>>{corners},0.07,cameraMatrix,distCoeffs,rvecs,tvecs);

    //translation relative to camera as matrix
    cv::Mat transOpp = (cv::Mat_(1,3)<<tvecs[0],tvecs[1],tvecs[2]);

    //translation relative to robot as 2d point
    cv::Point2d oppVec((rot3*transOpp)[0],(rot3*transOpp)[1]);

    //rotate translation by bearing
    cv::Point2d ret;
    ret.x = (oppVec.x*std::cos(bearing*pi/180)+oppVec.y*std::sin(bearing*pi/180));
    ret.y = (-oppVec.x*std::sin(bearing*pi/180)+oppVec.y*std::cos(bearing*pi/180));
    //add to current position
    ret += currPos;
    return ret;
}










