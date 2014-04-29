#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/flann/flann.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <float.h>
#include <QCoreApplication>

//using namespace std;
//using namespace cv;

//#define CamType 1

//int main(int argc, char *argv[]){
//    QCoreApplication a(argc, argv);
//    Mat image;
//    CvCapture *cam1;
//    if(CamType==0){
//        cam1=cvCaptureFromCAM(0);
//        cout << "I'm using the Internal Camera" << endl;
//    }else if(CamType==1){
//        cam1=cvCaptureFromFile("http://10.11.65.12/mjpg/video.mjpg");
//    }
//    while(cam1){
//        while(1){
//            image=cvQueryFrame(cam1);
//            imshow("stream",image);
//            cvtColor(image,image,CV_BGR2HSV);
//            imshow("HSV",image);
//            waitKey(0);
//        }
//    }
//    return a.exec();
//}


int x_val = 0;
int y_val = 0;

using namespace cv;
using namespace std;

int main(){
    namedWindow("m1013",CV_WINDOW_KEEPRATIO);
    namedWindow("m1011",CV_WINDOW_KEEPRATIO);
    namedWindow("HSV-m1013",CV_WINDOW_KEEPRATIO);
    namedWindow("HSV-m1011",CV_WINDOW_KEEPRATIO);
    CvCapture *cam1,*cam2,*cam3;
    cam1 = cvCaptureFromFile("http://10.11.65.12/mjpg/video.mjpg");
    //cam2 = cvCaptureFromCAM(0);
    cam3 = cvCaptureFromFile("http://10.11.65.11/mjpg/video.mjpg");
    cout << "capture opened" << endl;
    while(/*cam1&&cam3*/1==1){
        cout << "capture successful" << endl;
        while (1){
            //cout << "entered infinite loop" << endl;
            Mat frame1 = cvQueryFrame(cam1);
            //Mat frame2 = cvQueryFrame(cam2);
            Mat frame3 = cvQueryFrame(cam3);
            //resize(frame2, frame2, Size(), .5, .5, INTER_CUBIC);				//resizes the input image to make operations faster
            imshow("m1013", frame1);
            //imshow("internal", frame2);
            imshow("m1011", frame3);
            cvtColor(frame1, frame1, CV_BGR2HSV);
            //cvtColor(frame2, frame2, CV_BGR2HSV);
            cvtColor(frame3, frame3, CV_BGR2HSV);
            imshow("HSV-m1013", frame1);
            //imshow("HSV-internal", frame2);
            imshow("HSV-m1011", frame3);
            waitKey(1);
        }
    }
    //cin;
    cout<<"something just failed"<<endl;
    return 0;
}
