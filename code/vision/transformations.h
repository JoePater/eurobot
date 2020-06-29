
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "opencv2/core.hpp"
#include "opencv2/aruco.hpp"
#include <vector>
#include <map>

//marker 51/71 for opponent
//map of markers defined here
//id, rotation matrix, translation
extern std::map<int,std::pair<cv::Mat,cv::Mat>> markerMap;

//camera transform defined here
extern cv::Mat cam0Rot;
extern cv::Mat cam0Trans;

extern cv::Mat cam1Rot;
extern cv::Mat cam1Trans;

//camera calibration
extern cv::Mat cameraMatrix, distCoeffs;
//void initCalib();

//marker dictionary
extern cv::Ptr<cv::aruco::Dictionary> markerDict;

std::pair<cv::Point2d,double> estimateRobotPosition(std::vector<std::vector<cv::Point2f>> corners,std::vector<int> ids,bool camera=0);
cv::Point2d estimateOpponent(cv::Point2d currPos,std::vector<cv::Point2f> corners,bool camera=0);


#endif
