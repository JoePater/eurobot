
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>
#include <vector>
#include "Ellipse.h"
#include "buoyshape.h"
using namespace cv;

void findRedEdges(const Mat& img,Mat& outputImg,std::vector<Point2i>& points){
    Mat mask(3,10,CV_8SC3);

    char* maskPtr1 = mask.ptr<char>(0);
    char* maskPtr2 = mask.ptr<char>(1);
    char* maskPtr3 = mask.ptr<char>(2);
    for(int i=0;i<28;i+=3){maskPtr1[i+2]=-6;maskPtr1[i+1]=0;maskPtr1[i]=0;}
    for(int i=0;i<28;i+=3){maskPtr2[i+2]=0;maskPtr2[i+1]=0;maskPtr2[i]=0;}
    for(int i=0;i<28;i+=3){maskPtr3[i+2]=6;maskPtr3[i+1]=-1;maskPtr3[i]=-4;}

    if(img.empty()) throw 1;

    //std::cout<<mask<<"\n";
    std::vector<Point2i> points1;

    for(int row=1;row<img.rows-2;++row){
        const unsigned char* p1 = img.ptr<unsigned char>(row-1);
        const unsigned char* p2 = img.ptr<unsigned char>(row);
        const unsigned char* p3 = img.ptr<unsigned char>(row+1);
        unsigned char* outPtr = outputImg.ptr<unsigned char>(row);
        for(int col=5*3;col<img.cols*3-15;col+=3){
            int tmp=0;
            for(int x=-15;x<15;++x){
                tmp += p1[col+x]*maskPtr1[x+15]+p2[col+x]*maskPtr2[x+15]+p3[col+x]*maskPtr3[x+15];
            }
            if(tmp>0) std::cout<<tmp<<"\n";
            if(tmp>300){
                Point2i p(col/3,row);
                points1.push_back(p);
            }
            if(tmp<=0) outPtr[col] = 0;
            else if(tmp<=300) outPtr[col] = 0;//tmp;
            else outPtr[col] = 255;
            //outPtr[col] = p2[col+2];
            outPtr[col+1] = outPtr[col];
            outPtr[col+2] = outPtr[col];
        }
    }
    std::vector<Point2i> tmpPoints;
    for(Point2i point:points1){
        bool tooClose = false;
        if(!tmpPoints.empty()){
            if(abs(point.x-tmpPoints[0].x)>30 || point.y!=tmpPoints[0].y){
            std::cout<<"Point: "<<tmpPoints[tmpPoints.size()/2]<<"\n";
                points.push_back(tmpPoints[tmpPoints.size()/2]);
                tmpPoints.clear();
            }
        }
        for(Point2i p:points){
            if(norm(point-p)<31) tooClose=true;
        }
        if(!tooClose){
        std::cout<<"Temp Point: "<<point<<"\n";
            tmpPoints.push_back(point);
        }
    }
}
/*
bool matchElipse(const Mat& img,Mat& outImg,std::vector<Point2i>& tops){

}
*/
inline int findRedness(const unsigned char* pix){
    //std::cout<<"BGR: "<<(int)pix[0]<<","<<(int)pix[1]<<","<<(int)pix[2];
    return 2*pix[2]-pix[0]-pix[1];//(400*pix[2]-100*pix[1]-200*pix[0])/(pix[2]+pix[1]+pix[0]+20);
}
int main(){
    std::string file = "cup-test-1.png";
    while(1){
        Mat img = imread(file,IMREAD_COLOR);
        Mat outputImg = img.clone();

        std::vector<Point2i> points;
        findRedEdges(img,outputImg,points);

        for(Point2i point:points){
            BuoyShape buoy;
            try{
                buoy = BuoyShape(point,img);
                buoy.draw(img);
            }catch(int e){
                continue;
            }
        }
        /*for(int y=0;y<img.rows;++y){
            unsigned char* ir=img.ptr<unsigned char>(y);
            unsigned char* r=outputImg.ptr<unsigned char>(y);
            for(int x=0;x<img.cols*3;x+=3){
                if(findRedness(ir+x)>100){
                    r[x]=255;r[x+1]=255;r[x+2]=255;
                }else{
                    r[x]=0;r[x+1]=0;r[x+2]=0;
                }
            }
        }*/
        imshow("Input",img);
        //imshow("Output",outputImg);
        waitKey(0);
        std::cin>>file;
    }
    /*
    Mat img(500,500,CV_8UC3,Scalar(0,0,0));
    Ellipse el(Point(200,100),120,40,20);
    el.draw(img);
    //ellipse(img,Point2f(50,100),Size2f(20,15),20,0,360,Scalar(0,0,255),5);
    circle(img,Point(200,100),5,Scalar(0,255,0),3);
    for(double an=0;an<360;an+=10){
        circle(img,el.getPtAtAngle(an,img),2,Scalar(255,0,0),2);
        imshow("Ellipse",img);
        waitKey(0);
    }
    */
    return 0;
}



