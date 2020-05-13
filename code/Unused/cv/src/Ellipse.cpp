#include "Ellipse.h"
#include <cmath>
#include <iostream>

const double pi = 3.1415927;

Ellipse::Ellipse(cv::Point2i p,double a,double b,double ang):point(p),a(a),b(b),angle(ang){}

Ellipse::Ellipse(cv::Point2i top,std::vector<cv::Point2i> pts):point(top){
    std::vector<cv::Point2f> npts;
    for(auto p:pts){
        npts.push_back(p);
    }
    cv::RotatedRect rect = cv::fitEllipse(pts);
    a = rect.size.width/2;
    b = rect.size.height/2;
    angle = rect.angle;

    if(a < b){double tmp=a;a=b;b=tmp;angle=angle+90;}
    //std::cout<<"a: "<<a<<" b: "<<b<<" angle: "<<angle<<"\n";
    while(angle>=360)angle-=360;
}


cv::Point2i Ellipse::getPoint(){return point;};
double Ellipse::getA(){return a;};
double Ellipse::getB(){return b;};
double Ellipse::getAngle(){return angle;};
void Ellipse::updatePoint(cv::Point2i p){point = p;};
void Ellipse::updateA(double newa){a=newa;};
void Ellipse::updateB(double newb){b=newb;};
void Ellipse::updateAngle(double ang){angle=ang;};


cv::Point2d Ellipse::getCenter(){
    using std::pow;
    double tmpX = a/std::sqrt(1+pow(b/std::tan(angle/180.0*pi)/a,2));
    if(angle < 0) tmpX *= -1;
    double tmpY = b*std::sqrt(1-pow(tmpX/a,2));
    double dx = tmpX*std::cos(angle*pi/180.0)-tmpY*std::sin(angle/180.0*pi);
    double dy = tmpY*std::cos(angle*pi/180.0)+tmpX*std::sin(angle/180.0*pi);
    //std::cout<<"tmpX: "<<tmpX<<"   tmpY: "<<tmpY<<"   dx: "<<dx<<"   dy: "<<dy<<"   angle: "<<angle<<"\n";
    return cv::Point2d(point.x+dx,point.y+std::abs(dy));
}
void Ellipse::draw(cv::Mat& img){
    cv::ellipse(img,getCenter(),cv::Size2f(a,b),angle,0,360,cv::Scalar(0,0,255),3);
}
bool Ellipse::contains(cv::Point2i p){
    cv::Point2d trPt;
    cv::Point2d center = getCenter();
    trPt.x = (p.x-center.x)*std::cos(angle/180.0*pi)-(p.y-center.y)*std::sin(angle/180.0*pi);
    trPt.y = (p.y-center.y)*std::cos(angle/180.0*pi)+(p.x-center.x)*std::sin(angle/180.0*pi);
    trPt += center;
    return (std::pow(trPt.x/a,2)+std::pow(trPt.y/b,2)) <= 1;
}
cv::Point2d scalePt(cv::Point2d pt,cv::Point2d c,double xscale,double yscale){
    return pt+cv::Point2d((pt.x-c.x)*xscale,(pt.y-c.y)*yscale);
}
cv::Point2d rotatePt(cv::Point2d pt,cv::Point2d about,double a){ //radians
    cv::Point2d out;
    out.x = ((pt.x-about.x)*std::cos(a)-(pt.y-about.y)*std::sin(a))+about.x;
    out.y = ((pt.x-about.x)*std::sin(a)+(pt.y-about.y)*std::cos(a))+about.y;
    return out;
}
cv::Point2d Ellipse::getPtAtAngle(double ang){
    ang = (90-angle-ang);
    //std::cout<<"ANG: "<<ang<<"\n";
    while(ang>=360)ang -= 360;
    while(ang<0)ang += 360;
    cv::Point2d c = getCenter();
    cv::Point2d tmp;
    tmp.x = 1/std::sqrt(std::pow(a,-2)+std::pow(std::tan(ang/180*pi)*b,-2));
    tmp.y = b*std::sqrt(1-std::pow(tmp.x/a,2));
    if(!(ang > 90 && ang < 270))tmp.y *= -1;
    if(ang>180)tmp.x *= -1;
    return rotatePt(tmp+c,c,angle*pi/180);
}
long Ellipse::checkRedEdges(const cv::Mat& img){
    long total;
    //cv::Point2d center = getCenter();
    for(double ptAng=0;ptAng<180;ptAng+=5){
        cv::Point2d edgePt = getPtAtAngle(ptAng);
        for(char i=-1;i<=5;i+=3){
            for(char j=-1;j<=1;++j){
                cv::Point2d pt = edgePt+cv::Point2d(i,j);
                if(contains(pt))total += img.at<unsigned char>(pt);
                else total -= img.at<unsigned char>(pt);
            }
        }

    }
    return total;
}


cv::Point2d getEllipsePtAtAngle(double ang,double a,double b,double elAng,cv::Point2d center){
    ang = (90-elAng-ang);
    //std::cout<<"ANG: "<<ang<<"\n";
    while(ang>=360)ang -= 360;
    while(ang<0)ang += 360;
    cv::Point2d tmp;
    tmp.x = 1/std::sqrt(std::pow(a,-2)+std::pow(std::tan(ang/180*pi)*b,-2));
    tmp.y = b*std::sqrt(1-std::pow(tmp.x/a,2));
    if(!(ang > 90 && ang < 270))tmp.y *= -1;
    if(ang>180)tmp.x *= -1;
    return rotatePt(tmp+center,center,ang*pi/180);
}


















