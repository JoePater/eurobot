#ifndef BUOYSHAPE_H
#define BUOYSHAPE_H

#include <vector>
#include <map>

#include "opencv2/core.hpp"

#include "Ellipse.h"

enum cupRegion{
    L_REGION, //left left
    M_REGION, //left center
    N_REGION, //right center
    O_REGION, //right right
    P_REGION, //bottom center
    Q_REGION //bottom bottom
};

class BuoyShape
{
    Ellipse el;
public:
    std::map<cupRegion,std::vector<cv::Point2i>> getRegionPoints();
    std::map<cupRegion,bool> regionColors(const cv::Mat& img);

    void scale(float x);
    void rotate(float a);
    void scaleA(float x);
    void scaleB(float x);

    bool adjustHorizontal(const cv::Mat& img,std::map<cupRegion,bool>& redRegs,float factor=0.05);
    bool adjustVertical(const cv::Mat& img,std::map<cupRegion,bool>& redRegs,float factor=0.05);

    BuoyShape()=default;
    BuoyShape(cv::Point2i elTop,std::vector<cv::Point2i> pts);
    BuoyShape(cv::Point2i elTop,const cv::Mat& img);

    cv::Point2d bottomCenter();
    bool adjustEllipse(const cv::Mat& img);
    void draw(cv::Mat& img);
};

#endif // BUOYSHAPE_H
