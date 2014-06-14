#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
using namespace cv;
using namespace std;

string tostr(int num)
{
    stringstream str;
    str << num;
    return str.str();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CvCapture *cam1 = cvCaptureFromCAM(0);
    while(!cam1)
    {

    }
    while(true)
    {
        if(cam1)
        {
            Mat img = cvQueryFrame(cam1);
            namedWindow("Window", CV_WINDOW_AUTOSIZE);
            string number = tostr(rand());
            imshow("Window", img);
            waitKey(1);
            int x = waitKey(1);
            if(x == 27)
            {
                exit(EXIT_SUCCESS);
            } else {
                if(x == 'c')
                {
                    string uri = "/media/storage/programming/saves/image_" + number + ".jpg";
                    imwrite(uri, img);
                    cout << "\nImage saved!\n==>" << uri;
                }
            }
        }
    }
    return a.exec();
}
