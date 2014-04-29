#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QCoreApplication>
#include <iostream>
#include <pthread.h>
using namespace cv;
using namespace std;

CvCapture *cam1 = cvCaptureFromCAM(0);
Mat i, whiteboard;
pthread_mutex_t m1;
int hmin, smin, vmin, hmax, smax, vmax, kernel, epsilon;

void *camera(void *tid)
{
    long threadID = (long) tid;
    cout << "The image grabber has initiated in Thread " << threadID << "!" << endl;
    while(true)
    {
        //check if the camera is available and wait if not
        if(cam1)
        {
            //store the camera image inside a temporary matrix
            Mat x = cvQueryFrame(cam1);
            //check to make sure the data in x is valid
            if(!x.empty())
            {
                //lock the camera image
                pthread_mutex_lock(&m1);
                //write the new image to the matrix
                x.copyTo(i);
                //unlock the camera image
                pthread_mutex_unlock(&m1);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //create the threads
    pthread_t threads[0];

    //spawn the thread to read from the image stream (camera)
    long grabbertid = pthread_create(&threads[0], NULL, camera, (void *)0);

    //infinite loop, exited by exit()
    while(true)
    {
        //declare the Matrix to store the image from the camera
        Mat img;
        //lock the camera image
        pthread_mutex_lock(&m1);
        //check to make sure the camera image isn't empty
        if(!i.empty())
        {
            //copy over the data from the camera stream into a local Matrix to free that data
            i.copyTo(img);
        }
        //unlock the camera image
        pthread_mutex_unlock(&m1);

        //check to make sure that the grabbed image isn't empty.
        //Remember that the failsafes were above so they don't help here.
        //We need to redo to cause anything
        if(!img.empty())
        {
            //create the hsv threshold matrix
            Mat hsv;

            //convert the image to HSV for better color processing
            cvtColor(img, hsv, CV_BGR2HSV);

            //create the main output window
            namedWindow("HSV", CV_WINDOW_FREERATIO);

            //create trackbars for the HSV settings to make program operation easier
            createTrackbar("HMIN", "HSV", &hmin, 254, 0);
            createTrackbar("HMAX", "HSV", &hmax, 255, 0);
            createTrackbar("SMIN", "HSV", &smin, 254, 0);
            createTrackbar("SMAX", "HSV", &smax, 255, 0);
            createTrackbar("VMIN", "HSV", &vmin, 254, 0);
            createTrackbar("VMAX", "HSV", &vmax, 255, 0);
            createTrackbar("BLUR", "HSV", &kernel, 16, 0);
            createTrackbar("EPSILON", "HSV", &epsilon, 16, 0);

            //START: Prevent dumb crashes by making sure the inputs are not really wierd, e.x. min greater than max
            if(hmin > hmax)
            {
                hmin = hmax - 1;
            }
            if(smin > smax)
            {
                smin = smax - 1;
            }
            if(vmin > vmax)
            {
                vmin = vmax - 1;
            }
            if(kernel < 1)
            {
                kernel = 1;
            }
            if(kernel > 128)
            {
                kernel = 128;
            }
            if(epsilon < 1)
            {
                epsilon = 1;
            }
            if(epsilon > 16)
            {
                epsilon = 16;
            }
            //END: Prevent dumb crashes

            //threshold the image to the hue, saturation and value variables
            inRange(hsv, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), hsv);
            //blur the image a bit to rid it of noise
            blur(hsv, hsv, Size(kernel, kernel), Point(-1, -1), BORDER_DEFAULT);
            //convert the image to a binary image for higher efficiency
            threshold(hsv, hsv, 128, 255, CV_THRESH_BINARY);
            //create the vectors to store the contour data
            vector<vector<Point> > contours;
            vector<Rect> hierarchy;
            //find the contours in the image
            findContours(hsv, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point());
            //more vectors for contour data
            vector<vector<Point> > poly(contours.size());
            vector<RotatedRect> boundRect(contours.size());

            //here, we will iterate through every particle (contour) that we detect and process it
            for(int i = 0; i < contours.size(); i++)
            {
                //estimate the contours and set up the variables to make further processing easier
                approxPolyDP(contours[i], poly[i], epsilon, true);
                //check the convexity of the contour
                if(!isContourConvex(poly[i]))
                {
                    continue;
                }
                //calculate the bounding rectangle for the contour
                boundRect[i] = minAreaRect(Mat(poly[i]));
                //draw the detected contours to the screen
                drawContours(hsv, poly, i, Scalar(0, 39, 250), 1, 8, hierarchy, 0, Point());

            }


            imshow("HSV", hsv);
            int keyStroke = waitKey(1);
            if(keyStroke == 27)
            {
                exit(EXIT_SUCCESS);
            }
        }
    }
    return a.exec();
}
