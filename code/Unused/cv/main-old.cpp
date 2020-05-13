
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>
using namespace cv;

int main(){
    std::string file = "cup-test-1.png";
    while(1){
        Mat img = imread(file,IMREAD_COLOR);
        Point2f redStart(0,0);
        Point2f redStop(0,0);
        if(img.empty()) return 1;
        long int redTotalPrev = -1;
        for(int row=0;row<img.rows;++row){
            long int redTotal = 0;
            unsigned char* p = img.ptr<unsigned char>(row); //pointer to row
            for(int col=2;col<img.cols*3;col+=3){
                if(p[col]>(p[col-1]+p[col-2])){
                    redTotal += p[col]>40?p[col]:0;
                }
            }

            if(redTotalPrev != -1 && redTotalPrev+20<redTotal){
                bool started = 0;
                for(int col=2;col<img.cols;col+=3){
                    if(p[col]>(p[col-1]+p[col-2])){
                        if(!started){redStart=Point2f(col,row);started=1;}
                    }else if(started){redStop=Point2f(col,row);}
                }
                circle(img,Point((redStart.x+redStop.x)/2,(redStart.y+redStop.y)/2),2,Scalar(255,0,0),FILLED);
            }
            //line(img,Point(img.cols-20,row),Point(img.cols-1,row),Scalar(0,0,(500*redTotal)/img.cols),1);
            redTotalPrev = redTotal;
        }
        imshow("Image",img);
        waitKey(0);
        std::cin>>file;
    }
    return 0;
}



