#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>
#include <iostream>
#include <functionstoolkit.h>
using namespace cv;
using namespace std;

//string faceCascade = "haarcascade_frontalface_alt.xml";
string faceCascade = "lbpcascade_frontalface.xml";
CascadeClassifier face;
//The Camera object. CvCapture is the new caputure object. cvCaptureFromCAM(int dev) startes the camera and sets it as input
CvCapture *cam1 = cvCaptureFromCAM(0);
//the main image to use
Mat img;
//some mutexes
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
bool flipFace = false;
string imageURI = "bird.png";
int imageNumber = rand();
int rotation = 0;
bool rot = false;
int rndNum = 0;
FunctionsToolkit kit;

void cvRotate(Mat& src, double angle, Mat& dst)
{
    int len = max(src.cols, src.rows);
    Point2f pt(len/2., len/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(len, len));
}

string toStr(int number)
{
    stringstream str;
    str << number;
    return str.str();
}

void *imageGrabber(void *threadID)
{
    long tid = (long) threadID;
    cout << "\nGrabber in thread ==>" << tid << endl;
    while(true)
    {
        if(cam1)
        {
            Mat x = cvQueryFrame(cam1);
            flip(x, x, 1);
            if(!x.empty())
            {
                pthread_mutex_lock(&m1);
                x.copyTo(img);
                pthread_mutex_unlock(&m1);
            }
        }
        /*
        Mat x = kit.freenect_sync_get_rgb_cv(0);
        if(!x.empty())
        {
            pthread_mutex_lock(&m1);
            x.copyTo(img);
            pthread_mutex_unlock(&m1);
        }*/
    }
}

void *randomizer(void *threadID)
{
    cout << "\nRandomizer in thread" << (long) threadID << endl;
    while(true)
    {
        pthread_mutex_lock(&m2);
        rndNum++;
        if(rndNum >= 1000)
        {
            rndNum = 0;
        }
        pthread_mutex_unlock(&m2);
    }
}

int main()
{
    if(!face.load(faceCascade))
    {
        cout << "Could not load the face cascade!" << endl;
        exit(EXIT_FAILURE);
    }

    Mat creeperFace, gray0, hist2;
    Mat unchanged = imread(imageURI.data());

    if(unchanged.empty())
    {
        cout << "Could not load the creeper face!" << endl;
        system("nautilus ~/programming/repos/OpenCV-progs/FaceChanger/build-FaceChanger-Desktop-Debug");
        exit(EXIT_FAILURE);
    }

    cvtColor(unchanged, hist2, CV_BGR2GRAY);
    equalizeHist(hist2, gray0);
    vector<Rect> faces2;
    face.detectMultiScale(gray0, faces2, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));
    unchanged.copyTo(creeperFace);
    for(long j = 0; j < faces2.size(); j++)
    {
        Rect faceROI4(faces2[j].x, faces2[j].y, faces2[j].width, faces2[j].height);
        Mat faceColor4 = unchanged(faceROI4);
        faceColor4.copyTo(creeperFace);
    }

    pthread_t threads[0];
    pthread_create(&threads[0], NULL, imageGrabber, (void *)0);
    cout << "Welcome!\n\tPress [ESC] to exit!\n\tPress [S] to switch modes!\n\tPress [C] to capture the image!\n\tPress [T] to toggle rotation :D!" << endl;
    while(1)
    {
        Mat x, y;
        pthread_mutex_lock(&m1);
        {
            if(!img.empty())
            {
                img.copyTo(x);
            }
        }
        pthread_mutex_unlock(&m1);
        Mat gray, hist;
        if(!x.empty())
        {
            x.copyTo(y);
            cvtColor(x, hist, CV_BGR2GRAY);
            equalizeHist(hist, gray);
            vector<Rect> faces;
            face.detectMultiScale(gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));
            for(long i = 0; i < faces.size(); i++)
            {
                if(faces[i].x > 0 && faces[i].y > 0)
                {
                    if(faces[i].x + faces[i].width < x.cols && faces[i].y + faces[i].height < x.rows)
                    {
                        if(!flipFace)
                        {
                            Mat smallCreeper;
                            resize(creeperFace, smallCreeper, Size(faces[i].width, faces[i].height));
                            Rect roi(Point(faces[i].x, faces[i].y), Size(faces[i].width, faces[i].height));
                            Mat destinationROI = x(roi);
                            Mat creeperFaceHSV;
                            cvtColor(smallCreeper, creeperFaceHSV, CV_BGR2HSV);
                            inRange(creeperFaceHSV, Scalar(0, 0, 254), Scalar(0, 0, 255), creeperFaceHSV);
                            imshow("Win2", creeperFaceHSV);
                            threshold(creeperFaceHSV, creeperFaceHSV, 64, 255, CV_THRESH_BINARY_INV);
                            smallCreeper.copyTo(destinationROI, creeperFaceHSV);
                        } else {
                            Rect faceROI(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
                            Mat faceColor = x(faceROI);
                            cvtColor(faceColor, faceColor, CV_BGR2RGB);
                            flip(faceColor, faceColor, 0);
                            if(rot)
                            {
                                if(rotation >= 360)
                                {
                                    rotation = 0;
                                }
                                cvRotate(faceColor, rotation, faceColor);
                                rotation+=4;
                            }
                            resize(faceColor, faceColor, Size(faces[i].width, faces[i].height));
                            Rect faceROI2(Point(faces[i].x, faces[i].y), Size(faces[i].width, faces[i].height));
                            Mat dstROI = x(faceROI2);
                            faceColor.copyTo(dstROI);
                        }
                    }
                }
            }
            namedWindow("Window", CV_WINDOW_KEEPRATIO);
            imshow("Window", x);
            int keyStroke = waitKey(1);
            if(keyStroke == 27)
            {
                exit(EXIT_SUCCESS);
            } else {
                if(keyStroke == 's')
                {
                    flipFace = !flipFace;
                } else {
                    if(keyStroke == 'c')
                    {
                        cout << "The image was saved at \"/host/share/saves/image" << imageNumber << ".jpg" << endl;
                        imwrite("/host/share/saves/image" + toStr(imageNumber) + ".jpg", x);
                        imageNumber++;
                    } else {
                        if(keyStroke == 't')
                        {
                            rot = !rot;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
