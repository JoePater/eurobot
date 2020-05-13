#include "buoyshape.h"
#include <iostream>

const double buoy_top_diameter = 54;
const double buoy_bottom_diameter = 72;
const double diameter_ratio = buoy_bottom_diameter/buoy_top_diameter;
const double buoy_height = 115;

const int buoy_edge_step = 5; //distance either side of edge to test
const double bottom_angle = 30; //angle to avoid either side of horizontal for bottom samples
const double bottom_angle_step = 20; //angle to step for bottom sample
const double side_edge_step = 0.03; //step fraction up side edge for samples
const double side_edge_stop = 0.5; //highest point up side of edge for samples

const int max_edge_search = 45; //distance either side of top point to choose sample for ellipse
const int top_search_depth = 14; //depth to search for points down from top

const int num_buoy_adjusts = 10; //number of adjustments to try on buoy

const unsigned char min_red_pixel = 30; //minimum red value for pixel to be considered red

const double pi = 3.141593;

inline int findRedness(const unsigned char* pix){
    //std::cout<<"BGR: "<<(int)pix[0]<<","<<(int)pix[1]<<","<<(int)pix[2];
    return 2*pix[2]-pix[0]-pix[1];//return (400*pix[2]-100*pix[1]-200*pix[0])/(pix[2]+pix[1]+pix[0]+20);
}

void checkRowRedEdges(const cv::Mat& img,std::vector<cv::Point2i>& pts,int row,int topCol){
    if(topCol<6)return;
    if(topCol>=img.cols*3-6)return;

    const unsigned char* r = img.ptr<unsigned char>(row);
    if(findRedness(r+topCol*3)<70){
        //std::cout<<"Not red. RGB="<<(int)r[topCol*3+2]<<":"<<(int)r[topCol*3+1]<<":"<<(int)r[topCol*3]<<"\n";
        return;
    }

    int prevRedness=0;
    for(int i=0;i<max_edge_search*3;i+=3){
        int p=topCol*3+i;

        //check bounds
        if(p>=img.cols*3-6)break;

        //determine redness of section
        int redness = findRedness(r+p);//findRedness(r+p-6)+findRedness(r+p-3)+findRedness(r+p)+findRedness(r+p+3)+findRedness(r+p+6);

        if(redness<90){pts.push_back(cv::Point2i(p/3,row));break;}
        prevRedness = redness;
    }
    prevRedness = 0;
    for(int i=0;i<max_edge_search*3;i+=3){
        int p=topCol*3-i;

        //check bounds
        if(p<6)break;

        //determine redness of section
        int redness = findRedness(r+p);//(r+p-6)+findRedness(r+p-3)+findRedness(r+p)+findRedness(r+p+3)+findRedness(r+p+6);

        if(redness<90){pts.push_back(cv::Point2i(p/3,row));break;}
        prevRedness = redness;
    }
}

BuoyShape::BuoyShape(cv::Point2i elTop,std::vector<cv::Point2i> pts):
el(elTop,pts){}

BuoyShape::BuoyShape(cv::Point2i elTop,const cv::Mat& img){
    std::vector<cv::Point2i> pts;
    //add edges to vector
    for(int k=1;k<=top_search_depth;k+=1) checkRowRedEdges(img,pts,elTop.y+k,elTop.x);
    //std::cout<<pts<<"\n";
    if(pts.size()>=5)
        el = Ellipse(elTop,pts);
    else throw -1;
    if(!adjustEllipse(img))throw -1;

}



std::map<cupRegion,std::vector<cv::Point2i>> BuoyShape::getRegionPoints(){
    std::map<cupRegion,std::vector<cv::Point2i>> res;

    //start with l,m,n,o
    //step for either side of edge
    cv::Point2i step;
    step.x = buoy_edge_step*std::cos(el.getAngle()*pi/180);
    step.y = buoy_edge_step*std::sin(el.getAngle()*pi/180);

    //l+m: get edges
    cv::Point2d edgeStart;
    edgeStart.x = bottomCenter().x - diameter_ratio*el.getA()*std::cos(el.getAngle()*pi/180);
    edgeStart.y = bottomCenter().y + diameter_ratio*el.getA()*std::sin(el.getAngle()*pi/180);
    cv::Point2d edgeEnd;
    edgeEnd.x = el.getCenter().x - el.getA()*std::cos(el.getAngle()*pi/180);
    edgeEnd.y = el.getCenter().y + el.getA()*std::sin(el.getAngle()*pi/180);

    //get points either side
    std::vector<cv::Point2i> v1,v2;
    for(double w=side_edge_step;w<side_edge_stop;w+=side_edge_step){
        cv::Point2i pt=w*edgeEnd+(1-w)*edgeStart;
        v1.push_back(pt+step);
        v2.push_back(pt-step);
    }
    res[L_REGION] = v2;
    std::cout<<"LREGION: "<<v2<<"\n";
    res[M_REGION] = v1;

    //n+o: get edges
    edgeStart.x = bottomCenter().x + diameter_ratio*el.getA()*std::cos(el.getAngle()*pi/180);
    edgeStart.y = bottomCenter().y - diameter_ratio*el.getA()*std::sin(el.getAngle()*pi/180);
    edgeEnd.x = el.getCenter().x + el.getA()*std::cos(el.getAngle()*pi/180);
    edgeEnd.y = el.getCenter().y - el.getA()*std::sin(el.getAngle()*pi/180);

    //get points either side
    v1.clear();
    v2.clear();
    for(double w=side_edge_step;w<side_edge_stop;w+=side_edge_step){
        cv::Point2i pt=w*edgeEnd+(1-w)*edgeStart;
        v1.push_back(pt+step);
        v2.push_back(pt-step);
    }
    res[N_REGION] = v2;
    res[O_REGION] = v1;

    step.x = buoy_edge_step*std::cos(el.getAngle()*pi/180);
    step.y = buoy_edge_step*std::sin(el.getAngle()*pi/180);

    v1.clear();
    v2.clear();
    for(double ang=180+bottom_angle;ang<=360-bottom_angle;ang+=bottom_angle_step){
        cv::Point2i pt{getEllipsePtAtAngle(ang,el.getA(),el.getB(),el.getAngle(),bottomCenter())};
        v1.push_back(pt+step);
        v2.push_back(pt-step);
    }
    res[P_REGION] = v2;
    res[Q_REGION] = v1;

    return res;
}
std::map<cupRegion,bool> BuoyShape::regionColors(const cv::Mat& img){
    std::map<cupRegion,std::vector<cv::Point2i>> regs = getRegionPoints();
    std::map<cupRegion,bool> res;
    //iterate through regions
    for(auto r:regs){
        int n=0;
        int total=0;
        //iterate through points in region
        for(cv::Point2i p:r.second){
            if(p.x>=img.cols || p.x<0 || p.y>img.rows || p.y<0){
                throw 12;
            }
            ++n;
            int k=findRedness(img.ptr<unsigned char>(p.y)+p.x*3);
            total += k;
            std::cout<<"Region: "<<r.first<<" Point: "<<p<<" Redness: "<<k<<"\n";
        }
        res[r.first] = false;
        if(n>0){
            if(total/n > 60){
                res[r.first]=true;
            }
        }
    }
    return res;
}

cv::Point2d BuoyShape::bottomCenter(){
    cv::Point2d res;
    cv::Point2d c = el.getCenter();
    double a = el.getA();
    double b = el.getB();
    double dist = buoy_height/buoy_top_diameter*2*a*std::sqrt(std::abs(1.0-std::pow(b/a,2)));
    cv::Point2d diff;
    diff.x = dist*std::sin(el.getAngle()/180*pi);
    diff.y = std::abs(dist*std::cos(el.getAngle()/180*pi));
    return c+diff;
}

void BuoyShape::scale(float x){
    el.updateA(x*el.getA());
    el.updateB(x*el.getB());
}
void BuoyShape::rotate(float a){
    el.updateAngle(el.getAngle()+a);
}
void BuoyShape::scaleA(float x){el.updateA(x*el.getA());}
void BuoyShape::scaleB(float x){el.updateB(x*el.getB());}
bool BuoyShape::adjustHorizontal(const cv::Mat& img,std::map<cupRegion,bool>& redRegs,float factor){
    int flags=0;
    if(redRegs[L_REGION])flags|=0b1;
    if(redRegs[M_REGION])flags|=0b10;
    if(redRegs[N_REGION])flags|=0b100;
    if(redRegs[O_REGION])flags|=0b1000;

    std::cout<<"Flags: "<<flags<<"\n";

    //float closeness = 0.2/(n+1);
    //scale down,rotate,scale up
    switch(flags){
    case 0://none red
        scale(0.8);
        break;
    case 1:
        rotate(-10);
    case 2://M red
        scale(1-factor);
        break;
    case 3://left red
        rotate(-60*factor);
        break;
    case 4://N red
        scale(1-factor);
        break;
    case 5://N,L red
        return 0;
    case 6://M+N red
        break;
    case 7://LMN red
        scale(0.95);
        break;
    case 8: //O red
        rotate(10);
        break;
    case 9: //LO red
        return 0;
    case 10:
        return 0;
    case 11:
        return 0;
    case 12:
        rotate(60*factor);
        break;
    case 13:
        return 0;
    case 14:
        scale(0.95);
        break;
    case 15:
        scale(1.05);
        break;
    }
    return 1;
}
bool BuoyShape::adjustVertical(const cv::Mat& img,std::map<cupRegion,bool>& redRegs,float factor){
    if(redRegs[P_REGION]){
        if(redRegs[Q_REGION]){ //too short
            std::cout<<"P,Q\n";
            if(el.getB()<el.getA()/20)//already as long as can be
                scale(1+factor);
            else
                scaleB(1-factor);
        }
    }else{
        if(redRegs[Q_REGION]){
            std::cout<<"nP,Q\n";
            return 0;
        }else{ //too long
            std::cout<<"nP,nQ\n";
            if(el.getB()>0.9*el.getA())//already short as can be
                scale(1-factor);
            else
                scaleB(1-factor);
        }
    }
    return 1;
}
bool BuoyShape::adjustEllipse(const cv::Mat& img){
    for(int n=0;n<num_buoy_adjusts;++n){
        //get region points
        std::map<cupRegion,bool> redRegs;
        for(int i=0;i<20;++i){
            try{
                redRegs=regionColors(img);
                break;
            }catch(int){
                std::cout<<"BIG\n";
                scale(0.6);
            }
        }

        if(!adjustHorizontal(img,redRegs,0.05))return 0;

        if(!adjustVertical(img,redRegs,0.05))return 0;

        cv::Mat a = img.clone();
        draw(a);
        cv::imshow("Adjusted",a);
        cv::waitKey(0);
    }
    return 1;
}

void BuoyShape::draw(cv::Mat& img){
    std::cout<<"A: "<<el.getA()<<" B: "<<el.getB()<<" Ang: "<<el.getAngle()<<"\n";
    cv::Point2d edgeStart;
    edgeStart.x = bottomCenter().x - diameter_ratio*el.getA()*std::cos(el.getAngle()*pi/180);
    edgeStart.y = bottomCenter().y + diameter_ratio*el.getA()*std::sin(el.getAngle()*pi/180);
    cv::Point2d edgeEnd;
    edgeEnd.x = el.getCenter().x - el.getA()*std::cos(el.getAngle()*pi/180);
    edgeEnd.y = el.getCenter().y + el.getA()*std::sin(el.getAngle()*pi/180);
    cv::line(img,edgeStart,edgeEnd,cv::Scalar(255,0,0),3);

    edgeStart.x = bottomCenter().x + diameter_ratio*el.getA()*std::cos(el.getAngle()*pi/180);
    edgeStart.y = bottomCenter().y - diameter_ratio*el.getA()*std::sin(el.getAngle()*pi/180);
    edgeEnd.x = el.getCenter().x + el.getA()*std::cos(el.getAngle()*pi/180);
    edgeEnd.y = el.getCenter().y - el.getA()*std::sin(el.getAngle()*pi/180);
    cv::line(img,edgeStart,edgeEnd,cv::Scalar(255,0,0),3);

    //std::cout<<"Bottom center: "<<bottomCenter()<<"\n";
    cv::circle(img,bottomCenter(),3,cv::Scalar(0,255,0),2);
    cv::circle(img,el.getCenter(),3,cv::Scalar(0,255,0),2);

    cv::ellipse(img,el.getCenter(),cv::Size2d(el.getA(),
        el.getB()),-el.getAngle(),0,360,cv::Scalar(255,0,0),3);
    cv::ellipse(img,bottomCenter(),cv::Size2d(diameter_ratio*el.getA(),
        diameter_ratio*el.getB()),-el.getAngle(),0,360,cv::Scalar(255,0,0),3);
}






