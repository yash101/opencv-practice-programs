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
string imageURI = "patts.jpg";
int imageNumber = rand();
int rotation = 0;
bool rot = false;
int rndNum = 0;
int frameNum = 0;
int skipFrames = 0;
int captured = 0;
bool record = false;
FunctionsToolkit kit;

void cvRotate(Mat& src, double angle, Mat& dst)
{
    warpAffine(src, dst, getRotationMatrix2D(Point2f(max(src.cols / 2, src.rows / 2), max(src.cols / 2, src.rows / 2)), angle, 1.0), Size(max(src.cols, src.rows), max(src.cols, src.rows)));
    return;
}

string toStr(int number)
{
    stringstream str;
    str << number;
    return str.str();
}

void * imageGrabber(void *threadID)
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
    if(faces2.size() > 0)
    {
        cout << "Face Found!" << endl;
    }
    else
    {
        cout << "Face not found!" << endl;
    }

    pthread_t threads[0];
    pthread_create(&threads[0], NULL, imageGrabber, (void *)0);
    cout << "Welcome!\n\tPress [ESC] to exit!\n\tPress [S] to switch modes!\n\tPress [C] to capture the image!\n\tPress [T] to toggle rotation :D!" << endl;
    string text = "ACTION: NONE";
    string key_s = " ";
    while(true)
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
        Mat gray, hist, clean;
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
                            //cvtColor(smallCreeper, creeperFaceHSV, CV_BGR2HSV);
                            smallCreeper.copyTo(creeperFaceHSV);
                            inRange(creeperFaceHSV, Scalar(250, 250, 250), Scalar(255, 255, 255), creeperFaceHSV);
                            //inRange(creeperFaceHSV, Scalar(0, 0, 254), Scalar(0, 0, 255), creeperFaceHSV);
                            threshold(creeperFaceHSV, creeperFaceHSV, 64, 255, CV_THRESH_BINARY_INV);
                            smallCreeper.copyTo(destinationROI, creeperFaceHSV);
                        } else {
                            Rect faceROI(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
                            Mat faceColor = x(faceROI);
                            //cvtColor(faceColor, faceColor, CV_BGR2RGB);
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
            x.copyTo(clean);

            namedWindow("Window", CV_WINDOW_KEEPRATIO);

            if(record)
            {
                if(frameNum >= skipFrames)
                {
                    cout << "(PROC) The image was saved at \"/media/storage/programming/saves/facechanger-timedsaver/BDAY2" << imageNumber << "_PROC.jpg" << endl;
                    imwrite("/media/storage/programming/saves/facechanger-timedsaver/BDAY2/" + toStr(imageNumber) + "_PROC.jpg", clean);
                    imageNumber++;
                    cout << "(UNPROC) The image was saved at \"/media/storage/programming/saves/facechanger-timedsaver/BDAY2/" << imageNumber << "_UNPROC.jpg" << endl;
                    imwrite("/media/storage/programming/saves/facechanger-timedsaver/BDAY2" + toStr(imageNumber) + "_UNPROC.jpg", y);
                    imageNumber++;
                    frameNum = 0;
                    rectangle(x, Point(0, 0), Point(x.cols, x.rows), Scalar(255, 255, 255), 128, 8, 0);
                    captured += 2;
                    circle(x, Point(x.cols -32, x.rows - 32), 8, Scalar(0, 0, 255), 16, 8, 0);
                }
                else if(frameNum == skipFrames - 1)
                {
                    rectangle(x, Point(0, 0), Point(x.cols, x.rows), Scalar(16, 255, 16), 16, 8, 0);
                    circle(x, Point(x.cols -32, x.rows - 32), 8, Scalar(255, 0, 0), 16, 8, 0);
                }
                else
                {
                    rectangle(x, Point(0, 0), Point(x.cols, x.rows), Scalar(16, 16, 255), 16, 8, 0);
                    circle(x, Point(x.cols -32, x.rows - 32), 8, Scalar(0, 255, 0), 16, 8, 0);
                }
                frameNum++;
            }

            string tmp = record ? "True!" : "False";

            putText(x, "Press [ S ] to switch modes!", Point(16, 20), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 16, 16), 1, 8, false);
            putText(x, "Press [ESC] to exit!", Point(16, 40), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 16, 16), 1, 8, false);
            putText(x, "Press [ C ] to capture an image!", Point(16, 60), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 16, 16), 1, 8, false);
            putText(x, "Press [ D ] to clear the saves folder!", Point(16, 80), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 16, 16), 1, 8, false);
            putText(x, "Press [ R ] to toggle recording!", Point(16, 100), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 16, 16), 1, 8, false);
            putText(x, "FrameDelay: " + toStr(skipFrames), Point(16, 120), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(16, 16, 255), 1, 8, false);
            putText(x, "FrameCount: " + toStr(frameNum), Point(16, 140), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(16, 16, 255), 1, 8, false);
            putText(x, "Captured: " + toStr(captured), Point(16, 160), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(16, 16, 255), 1, 8, false);
            putText(x, "Record: " + tmp, Point(16, 180), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(16, 16, 255), 1, 8, false);
            putText(x, "Key: " + key_s, Point(x.cols - 192, x.rows - 32), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(16, 16, 255), 1, 8, false);
            putText(x, "Action: " + text, Point(16, x.rows - 32), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(16, 16, 255), 1, 8, false);

            text = "";
            if(flipFace)
            {
                putText(x, "Press [ T ] to toggle rotation!", Point(16, 200), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 16, 16), 1, 8, false);
            }
            imshow("Window", x);
            char key = waitKey(1);
            key_s = toStr(key);
            int keyStroke = (int) key;

            if(keyStroke != -1)
            {
                cout << "Key Event: ASCII_" << (int) keyStroke << " | ASCII_" << key << endl;
            }

            if(keyStroke == 27)
            {
                text = "EXIT";
                exit(EXIT_SUCCESS);
            }
            else
            {
                if(keyStroke == 's')
                {
                    text = "Mode Changed!";
                    flipFace = !flipFace;
                }
                else
                {
                    if(keyStroke == 'c')
                    {
                        cout << "The image was saved at \"/host/share/saves/image" << imageNumber << ".jpg" << endl;
                        imwrite("/media/storage/programming/saves/image" + toStr(imageNumber) + ".jpg", clean);
                        text = "Image Saved! FSAVED";
                        imageNumber++;
                    }
                    else
                    {
                        if(keyStroke == 't')
                        {
                            rot = !rot;
                            text = "Rotation Toggled!";
                        }
                        else
                        {
                            if(keyStroke == '-')
                            {
                                if(skipFrames > 1)
                                {
                                    skipFrames--;
                                    text = "Record Frequency Increased!";
                                }
                            }
                            else
                            {
                                if(keyStroke == '=')
                                {
                                    skipFrames++;
                                    text = "Record Frequency Deceased!";
                                }
                                else
                                {
                                    if(keyStroke == 'd')
                                    {
                                        system("rm /media/storage/programming/saves/facechanger-timedsaver/BDAY2*.jpg");
                                        cout << "Erased all images!" << endl;
                                        text = "Erased all images!";
                                    }
                                    else
                                    {
                                        if(keyStroke == 'r')
                                        {
                                            record = !record;
                                            text = "Recorder Toggled!";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }        
    }
    return 0;
}
