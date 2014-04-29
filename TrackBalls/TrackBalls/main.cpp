////#include <opencv2/opencv.hpp>
////#include <iostream>
////#include <fstream>
////#include <vector>
////#include <QCoreApplication>
////#include <ostream>
////#include <istream>
////#include <iostream>
////#include <ios>
////#include <iosfwd>

////int capMode=0;
////int capDev=0;
////std::string stream="http://10.11.65.12/video/video.mjpg";

////using namespace std;
////using namespace cv;

////CvCapture *cam1;

////int main(int argc, char *argv[])
////{
////    QCoreApplication a(argc, argv);
////    if(capMode==0){
////        cam1=cvCaptureFromCAM(capDev);
////    }else if(capMode==1){
////        cam1=cvCaptureFromFile(stream.data());
////    }else if(capMode==2){
////        cvCaptureFromAVI(stream.data());
////    }else{
////        cout<<"main.cpp>>Invalid capMode. Change the variable and recompile to fix."<<endl;
////    }
////    namedWindow("unprocessed",CV_WINDOW_KEEPRATIO);
////    namedWindow("grayscale",CV_WINDOW_KEEPRATIO);
////    namedWindow("smooth",CV_WINDOW_KEEPRATIO);
////    namedWindow("circles",CV_WINDOW_KEEPRATIO);
////    while(1){
////        while(cam1){
////            Mat camFrame=cvQueryFrame(cam1);
////            imshow("unprocessed",camFrame);
////            Mat grayscale;
////            cvtColor(camFrame,grayscale,CV_BGR2GRAY);
////            imshow("grayscale",grayscale);
////            equalizeHist(grayscale,grayscale);
////            GaussianBlur(grayscale,grayscale,Size(9,9),2,2);
////            imshow("smooth",grayscale);
////            vector<Vec3f> circles;
////            HoughCircles(grayscale,circles,CV_HOUGH_GRADIENT,1,grayscale.rows/8,200,100,0,0);
////            for(size_t i=0;i<circles.size();i++){
////                Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
////                int radius=cvRound(circles[i][2]);
////                circle(camFrame,center,3,Scalar(0,255,0),-1,8,0);
////                circle(camFrame,center,radius,Scalar(0,0,255),3,8,0);
////            }
////            imshow("circles",camFrame);
////            waitKey(1);
////        }
////        if(!cam1){
////            cout<<"main.cpp>>Could not capture frame."<<endl<<"\tPlease check that the camera is properly plugged in, or change the settings:"<<endl<<"\t\tcapMode, capDev and stream!";
////        }
////    }
////    return a.exec();
////}


//////#include "opencv2/highgui/highgui.hpp"
//////#include "opencv2/imgproc/imgproc.hpp"
//////#include <iostream>
//////#include <stdio.h>

//////using namespace cv;

///////** @function main */
//////int main(int argc, char** argv)
//////{
//////  Mat src, src_gray;
//////  CvCapture *cam=cvCaptureFromCAM(0);
//////  /// Read the imag
//////  while(1){
//////      src=cvQueryFrame(cam);
//////      if( !src.data )
//////        { return -1; }

//////      /// Convert it to gray
//////      cvtColor( src, src_gray, CV_BGR2GRAY );

//////      /// Reduce the noise so we avoid false circle detection
//////      GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

//////      vector<Vec3f> circles;

//////      /// Apply the Hough Transform to find the circles
//////      HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );

//////      /// Draw the circles detected
//////      for( size_t i = 0; i < circles.size(); i++ )
//////      {
//////          Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//////          int radius = cvRound(circles[i][2]);
//////          // circle center
//////          circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
//////          // circle outline
//////          circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
//////       }

//////      /// Show your results
//////      namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
//////      imshow( "Hough Circle Transform Demo", src );

//////      waitKey(1);
//////  }
//////  return 0;
//////}

//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <stdio.h>
//#include <stdlib.h>

//using namespace std;
//using namespace cv;

//Mat img, grayscale;
//Mat output, edges;

//int capType=0;
//int capDev=0;
//string input=NULL;
//int edgeThresh=1;

//int lowThresh;
//const int max_lowThreshold=100;
//int ratio=3;
//int kernel=3;

//void CannyTheshold(int,void*){
//    blur(img,edges,Size(3,3));
//    Canny(edges,edges,lowThresh,lowThresh*ratio,kernel);
//    output=Scalar::all(0);
//    img.copyTo(output,edges);
//}

//int main(){
//    CvCapture *cam1;
//    if(capType==0){
//        cam1=cvCaptureFromCAM(capDev);
//    }else{
//        if(capType==1){
//            cam1=cvCaptureFromFile(input.data());
//        }else{
//            if(capType==2){
//                cam1=cvCaptureFromAVI(input.data());
//            }else{
//                if(capType==2||capType==3){
//                    cam1=cvCaptureFromFile("http://10.11.65.12/mjpg/video.mjpg");
//                }
//            }
//        }
//    }
//    int failures=0;
//    int totalFail=0;
//    while(1){
//        if(cam1){
//            failures=0;
//        }
//        while(cam1){
//            img=cvQueryFrame(cam1);
//            namedWindow("Camera",CV_WINDOW_KEEPRATIO);
//            namedWindow("SketchBook",CV_WINDOW_KEEPRATIO);
//            namedWindow("App",CV_WINDOW_KEEPRATIO);
//            namedWindow("trackbars",CV_WINDOW_AUTOSIZE);
//            imshow("Camera",img);
//            output.create(img.size(),img.type());
//            cvtColor(img,grayscale,CV_BGR2GRAY);
//            createTrackbar("Threshold:","trackbars",&lowThresh,max_lowThreshold,CannyTheshold);
//            CannyTheshold(0,0);
//            imshow("SketchBook",output);
//            imshow("App",output);
//            waitKey(0);
//        }
//        if(!cam1){
//            failures++;
//            totalFail++;
//        }
//    }
//}
////#include "opencv2/imgproc/imgproc.hpp"
////#include "opencv2/highgui/highgui.hpp"
////#include <stdlib.h>
////#include <stdio.h>

////using namespace cv;

/////// Global variables

////Mat src, src_gray;
////Mat dst, detected_edges;

////int edgeThresh = 1;
////int lowThreshold;
////int const max_lowThreshold = 100;
////int ratio = 3;
////int kernel_size = 3;
////char* window_name = "Edge Map";

/////**
//// * @function CannyThreshold
//// * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
//// */
////void CannyThreshold(int, void*)
////{
////  /// Reduce noise with a kernel 3x3
////  blur( src_gray, detected_edges, Size(3,3) );

////  /// Canny detector
////  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

////  /// Using Canny's output as a mask, we display our result
////  dst = Scalar::all(0);

////  src.copyTo( dst, detected_edges);
////  imshow( window_name, dst );
////  imshow("ScetchBook",dst);
//// }


/////** @function main */
////int main( int argc, char** argv )
////{
////  /// Load an image
////    CvCapture *cam;
////    cam=cvCaptureFromCAM(0);
////    while(1){
////        src=cvQueryFrame(cam);
////      /// Create a matrix of the same type and size as src (for dst)
////      dst.create( src.size(), src.type() );

////      /// Convert the image to grayscale
////      cvtColor( src, src_gray, CV_BGR2GRAY );

////      /// Create a window
////      namedWindow( window_name, CV_WINDOW_AUTOSIZE );
////      namedWindow("ScetchBook",CV_WINDOW_KEEPRATIO);

////      /// Create a Trackbar for user to enter threshold
////      createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

////      /// Show the image
////      CannyThreshold(0, 0);

////      /// Wait until user exit program by pressing a key
////      waitKey(1);}

////  return 0;
////  }


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace cv;
using namespace std;

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 51;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
int capMode=1;
int capDev=0;
const string uri="http://10.11.65.12/mjpg/video.mjpg";

void CannyThreshold(int, void*){
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
    imshow("ScetchBook",dst);
}

int main(){
    CvCapture *cam;
    if(capMode==0){
        cam=cvCaptureFromCAM(capDev);
    }else{
        if(capMode==1){
            cam=cvCaptureFromFile(uri.data());
        }else{
            if(capMode==2){
                cam=cvCaptureFromAVI(uri.data());
            }
        }
    }
    while(1){
        src=cvQueryFrame(cam);
        flip(src,src,1);
        dst.create( src.size(), src.type() );

        cvtColor( src, src_gray, CV_BGR2GRAY );

        namedWindow( window_name, CV_WINDOW_KEEPRATIO);
        namedWindow("ScetchBook",CV_WINDOW_KEEPRATIO);

        createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

        CannyThreshold(0, 0);

        waitKey(1);
    }
    return 0;
}
