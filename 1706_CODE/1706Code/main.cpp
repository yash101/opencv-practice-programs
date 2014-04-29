#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/flann/flann.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <ostream>
#include <ios>
#include <iosfwd>
#include <vector>
#include <time.h>
#include <math.h>
//#include <array>
#include <assert.h>
#include <cfloat>
#include <QtNetwork/qlocalserver.h>
#include <cstdio>

using namespace std;
using namespace cv;

const string stream="http://10.11.65.11/mjpg/video.mjpg";

int add(int a,int b){
    int add;
    __asm__ ( "addl %%ebx, %%eax;" : "=a" (add) : "a" (a) , "b" (b) );
    return add;
}

int mult(int a,int b){
    int prod;
    __asm__ ( "imull %%ebx, %%eax;" : "=a" (prod) : "a" (a) , "b" (b) );
    return prod;
}

int div(int a,int b,int c){
    int quo,rem;
    __asm__ ( "movl $0x0, %%edx;"
              "movl %2, %%eax;"
              "movl %3, %%ebx;"
               "idivl %%ebx;" : "=a" (quo), "=d" (rem) : "g" (a), "g" (b) );
    if(c==1){
        return rem;
    }else{
        return quo;
    }
}

int sub(int a,int b){
    int subl;
    __asm__ ("subl %%ebx, %%eax;":"=a"(subl):"g"(a),"g"(b));
    return subl;
}

Mat frame;
int main(){
    int hueSlider=0;
    int satSlider=0;
    int valSlider=0;
    int hueSlider2=255;
    int satSlider2=255;
    int valSlider2=255;
    int accuracy=1;
    int dilateIter=1;
    int CAPTYPE=3;
    int CAPDEV=0;

    ifstream in("conf01.dff");
    if(in.is_open()){
        //in >> CAPTYPE;
        //in >> "|";
        //in >> CAPDEV;
        //in >> "|"
        in >> hueSlider;
        in >> satSlider;
        in >> valSlider;
        in >> hueSlider2;
        in >> satSlider2;
        in >> valSlider2;
        in >> accuracy;
        in >> dilateIter;
        //in >> stream;
    }

    const int sliderMax=255;
    const int minSliderMax=254;
    int maxAccuracy=100;
    int maxDilateIterations=5;

    CvCapture *cam1;
    if(CAPTYPE==0){
        cam1=cvCaptureFromCAM(CAPDEV);
    }else{
        if(CAPTYPE==1){
            cam1=cvCaptureFromFile(stream.data());
        }else{
            if(CAPTYPE==2){
                cam1=cvCaptureFromAVI(stream.data());
            }else{
                if(CAPTYPE==3){
                    cam1=cvCaptureFromFile("http://10.11.65.11/mjpg/video.mjpg");
                }else{
                    if(CAPTYPE==4){
                        cam1=cvCaptureFromFile("http://10.11.65.13/mjpg/video.mjpg");
                    }else{
                        cout << "$ camera identifier incorrect. Change. Exiting" << endl << "$ Press any key to exit..." << endl;
                        cin.get();
                        return -1;
                    }
                }
            }
        }
    }

    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX_SMALL,0.75,0.75,0,1,CV_AA);

    Mat img,dst,thresh;

    namedWindow("HSV",CV_WINDOW_NORMAL);
    namedWindow("HSVimg",CV_WINDOW_KEEPRATIO);
    namedWindow("Polygons",CV_WINDOW_KEEPRATIO);

    while(1){
        if(cam1){
            img=cvQueryFrame(cam1);

            createTrackbar("Hue_min","HSV",&hueSlider,minSliderMax,0);
            createTrackbar("Hue_max","HSV",&hueSlider2,sliderMax,0);
            createTrackbar("Saturation_min","HSV",&satSlider,minSliderMax,0);
            createTrackbar("Saturation_max","HSV",&satSlider2,sliderMax,0);
            createTrackbar("Value_min","HSV",&valSlider,minSliderMax,0);
            createTrackbar("Value_max","HSV",&valSlider2,sliderMax,0);
            createTrackbar("Polygon Error","HSV",&accuracy,maxAccuracy,0);
            createTrackbar("Dilate Iterations","HSV",&accuracy,maxDilateIterations,0);
            int c=waitKey(1);
            if(c==27){
                cout << "[ESC] Pressed! Now Exiting\nPress any key to continue..." << endl;
                //cin.get();
                break;
            }
            while(hueSlider2<=hueSlider){
                hueSlider2=hueSlider+1;
                //hueSlider2=add(hueSlider,1);
                //hueSlider2=add(hueSlider,1);
            }
            while(satSlider2<=satSlider){
                //satSlider2=satSlider+1;
                //satSlider2=add(satSlider,1);
                satSlider2=add(satSlider,1);
            }
            while(valSlider2<=valSlider){
                //valSlider2=valSlider+1;
                //valSlider2=add(valSlider,1);
                valSlider2=add(valSlider,1);
            }
            if(accuracy<10){
                accuracy=10;
            }

            img.copyTo(dst);

            cvtColor(img,img,CV_BGR2HSV);
            inRange(img,Scalar(hueSlider,satSlider,valSlider),Scalar(hueSlider2,satSlider2,valSlider2),img);
            dilate(img,img,1,Point(0,0),dilateIter,BORDER_CONSTANT,morphologyDefaultBorderValue());
            img.copyTo(thresh);

            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours(img,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point(0,0));

            vector<vector<Point> > contours_poly(contours.size());

            Scalar tri_color=Scalar(0,0,255);
            Scalar rect_color=Scalar(255,0,0);
            Scalar penta_color=Scalar(0,255,0);
            Scalar cir_color=Scalar(255,255,255);

            for(int i=0;i<contours.size();i++){
                if(contourArea(contours[i])>500){
                    approxPolyDP(contours[i],contours_poly[i],accuracy,true);
                    if(contours_poly[i].size()==3){
                        drawContours(dst,contours_poly,i,tri_color,3,8,hierarchy,0,Point());
                    }else{
                        if(contours_poly[i].size()==4){
                            drawContours(dst,contours_poly,i,rect_color,3,8,hierarchy,0,Point());
                        }else{
                            if(contours_poly[i].size()==5){
                                drawContours(dst,contours_poly,1,penta_color,3,8,hierarchy,0,Point());
                            }else{
                                if(contours_poly[i].size()>5){
                                    drawContours(dst,contours_poly,1,cir_color,3,8,hierarchy,0,Point());
                                }
                            }
                        }
                    }
                }
            }
            imshow("HSVimg", thresh);
            imshow("Polygons", dst);
            cout << "$ " << add(1,2) << " " << mult(1442563236363,3562379452043985) << " " << div(52345,423,0) << " " << div(4525523445325943523525242435,2452454,1) << endl;
        }else{
            cout << "$ Failed to get the image!" << endl;
        }
    }

    ofstream out("conf01.dff");
    //out << CAPTYPE;
    //out << "|";
    //out << CAPDEV;
    //out << "|";
    out << hueSlider;
    out << satSlider;
    out << valSlider;
    out << hueSlider2;
    out << satSlider2;
    out << valSlider2;
    out << accuracy;
    out << dilateIter;
    //out >> stream;

    destroyAllWindows();
    return 0;
}













//Crap:
/*


            ofstream out("~/DevCode/saves/1706Code/conf01.dff");
            out >> CAPTYPE;
            out >> CAPDEV;
            out >> hueSlider;
            out >> satSlider;
            out >> valSlider;
            out >> hueSlider2;
            out >> satSlider2;
            out >> valSlider2;
            out >> accuracy;
            out >> dilateIter;
            out >> stream;



//VideoWriter("~/desktop/record.avi",)

int arg1,arg2,add,sub,mul,quo,rem;
arg1=3;
arg2=11;
__asm__ ( "addl %%ebx, %%eax;" : "=a" (add) : "a" (arg1) , "b" (arg2) );
__asm__ ( "subl %%ebx, %%eax;" : "=a" (sub) : "a" (arg1) , "b" (arg2) );
__asm__ ( "imull %%ebx, %%eax;" : "=a" (mul) : "a" (arg1) , "b" (arg2) );
__asm__ ( "movl $0x0, %%edx;"
          "movl %2, %%eax;"
          "movl %3, %%ebx;"
           "idivl %%ebx;" : "=a" (quo), "=d" (rem) : "g" (arg1), "g" (arg2) );

printf( "%d + %d = %d\n", arg1, arg2, add );
printf( "%d - %d = %d\n", arg1, arg2, sub );
printf( "%d * %d = %d\n", arg1, arg2, mul );
printf( "%d / %d = %d\n", arg1, arg2, quo );
printf( "%d %% %d = %d\n", arg1, arg2, rem );


*/


