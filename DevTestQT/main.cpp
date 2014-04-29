#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CvCapture *cam1 = cvCaptureFromCAM(0);
    while(1) {
        Mat img = cvQueryFrame(cam1);
        imshow("Window",img);
        waitKey(1);
    }
    return a.exec();
}
