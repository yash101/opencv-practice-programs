#ifndef FUNCTIONSTOOLKIT_HPP
#define FUNCTIONSTOOLKIT_HPP
#include <iostream>
#include <cmath>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <libfreenect_sync.h>

class FunctionsToolkit {
public:
    void msg(std::string msg) {
        std::cout << "\n1165@node0~$ " << msg;
    }
    void nl(int iterations) {
        for(int i = 0; i < iterations; i++) {
            std::cout << "\n1154@node0";
        }
    }
    void nl() {
        nl(1);
    }
    std::string toStr(bool num) {
        std::stringstream str;
        str << num;
        return str.str();
    }
    std::string toStr(int num) {
        std::stringstream str;
        str << num;
        return str.str();
    }
    std::string toStr(float num) {
        std::stringstream str;
        str << num;
        return str.str();
    }
    std::string toStr(double num) {
        std::stringstream str;
        str << num;
        return str.str();
    }

    std::string toStr(long num) {
        std::stringstream str;
        str << num;
        return str.str();
    }
    double radToDeg(double num) {
        return num * (3.14159 / 180);
    }
    double tanDeg(double tan) {
        return std::tan(radToDeg(tan));
    }
    double hypotenuse(double leg_a, double leg_b) {
        return std::sqrt((leg_a * leg_a) + (leg_b * leg_b));
    }
    double leg(double leg, double hypotenuse) {
        return std::sqrt((hypotenuse * hypotenuse) - (leg * leg));
    }
    double distance(double CanvasSize, double ActualLength, double MeasuredLength, double CamFOV) {
        return ((CanvasSize * ActualLength) / MeasuredLength) / tanDeg(CamFOV / 2);
    }

    cv::Mat freenect_sync_get_depth_cv(int index)
    {
        static IplImage *image = 0;
        static char *data = 0;
        if (!image)
        {
            image = cvCreateImageHeader(cvSize(640,480), 16, 1);
        }
        unsigned int timestamp;
        freenect_sync_get_depth((void**)&data, &timestamp, index, FREENECT_DEPTH_11BIT);
        cvSetData(image, data, 640*2);
        cv::Mat img = cv::cvarrToMat(image, true, true, 0);
        cvReleaseImage(&image);
        return img;
    }

    cv::Mat freenect_sync_get_rgb_cv(int index)
    {
        static IplImage *image = 0;
        static char *data = 0;
        if(!image)
        {
            image = cvCreateImageHeader(cvSize(640, 480), 16, 1);
        }
        unsigned int timestamp;
        freenect_sync_get_video((void**)&data, &timestamp, index, FREENECT_VIDEO_RGB);
        cvSetData(image, data, 640*2);
        cv::Mat img = cv::cvarrToMat(image, true, true, 0);
        cvReleaseImage(&image);
        return img;
    }

    cv::Mat freenect_sync_get_ir_cv(int index)
    {
        static IplImage *image = 0;
        static char *data = 0;
        if(!image)
        {
            image = cvCreateImageHeader(cvSize(640, 480), 16, 1);
        }
        unsigned int timestamp;
        freenect_sync_get_video((void**)&data, &timestamp, index, FREENECT_VIDEO_IR_10BIT);
        cvSetData(image, data, 640*2);
        cv::Mat img = cv::cvarrToMat(image, true, true, 0);
        cvReleaseImage(&image);
        return img;
    }
};

#endif // FUNCTIONSTOOLKIT_HPP
