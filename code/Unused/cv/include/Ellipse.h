#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class Ellipse
{
    cv::Point2i point;
    double a;
    double b;
    double angle;
public:
    Ellipse()=default;
    Ellipse(cv::Point2i p,double a,double b,double ang);
    Ellipse(cv::Point2i top,std::vector<cv::Point2i> pts);
    cv::Point2i getPoint();
    double getA();
    double getB();
    double getAngle();
    void updatePoint(cv::Point2i point);
    void updateA(double a);
    void updateB(double b);
    void updateAngle(double angle);

    cv::Point2d getCenter();
    void draw(cv::Mat& img);
    cv::Mat transformPt(const cv::Mat& p,double ang,double k);
    bool contains(cv::Point2i p);
    cv::Point2d getPtAtAngle(double ang);

    long checkRedEdges(const cv::Mat& img);
    //cv::Point2d getTopLeft();
    //cv::Point2d getBottomRight();
    //long checkRedFill(const cv::Mat& img);
};

cv::Point2d getEllipsePtAtAngle(double ang,double a,double b,double elAng,cv::Point2d center);

#endif // ELLIPSE_H
