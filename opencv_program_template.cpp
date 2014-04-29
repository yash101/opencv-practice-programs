//these are the main includes that we will use! QCoreApplication is used
//by QT to initialise the program. highgui.hpp is the OpenCV HighGUI
//module, used for the Graphical User Interface, and other basic things.
//It also interfaces us to the camera! The objdetect.hpp file is the
//ObjDetect module, and is used for object detection and processing. The
//header, imgproc.hpp is for processing images. We will use this for
//converting color spaces and for thresholding, typically, however, that
//just scratches the surface of this library!
#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <pthread.h>

//add your other includes here:
//for example, to get some advanced trig and math functions,
//#include <cmath>
//Now, it's time to get coding!

//here are the namespaces! We need these, to scope into the libraries
//we want to use. std is the scope of the C++ STL libraries, that you
//probably used for your first application! cv is the namespace for
//the OpenCV library! This is the library we will use for all this
//image processing!
using namespace std;
using namespace cv;
//If we use more different libraries, we can use,
//using namespace [library namespace];
//for example,
//using namespace ocl; //ocl is the OpenCL accelerated portion of the
//OpenCV library!

//this is the object that we capture the image from. This is used for
//Camera, MJPEG, AVI and other miscellaneous video formats. Many people
//use VideoCapture instead. However, that is part of the OpenCV C syntax
//and it is outdated. It is also harder to use than CvCapture!
CvCapture *cam1;
//this stores the image that we will be passing along multiple threads!
Mat img;
//this is a mutex. It's job is to lock the resources. If a person is
//working on something, they are locked. In that case, we would call
//pthread_mutex_lock. Once that person is done with his or her job,
//that person is unlocked so he/she can be used elsewhere. To do this,
//we call pthread_mutex_unlock! Remember that the resource is
//unavailable when locked, so it can halt your program. To prevent this,
//quickly lock the resource, clone it into another local resource and
//use that. That way, the resource will be unlocked as a majority of the
//time.
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

//these are the capture environment variables, used by my grabber and
//bootloader skeleton. here is the table of outcomes:
//      +-----+------------------+---------------+
//      | I.D |     OUTPUT       | PREREQUISITES |
//      +-----+------------------+---------------+
//      |  0  |  Direct camera   |   Device ID   |
//      |  1  |  MJPEG           |   Stream URL  |
//      |  2  |  AVI File        |   Stream URL  |
//      |  3  |  Image [JPG|PNG] |   Stream URL  |
//      +-----+------------------+---------------+
static int capType = 0;
//the Device ID. Typically, this is zero, however, if the system has
//multiple cameras, you may need to play around and find the proper
//id for each camera!
static int capDev = 0;
static string stream = "";

//this is the function you will play with :)
int run()
{
    //this is the loop that runs forever, and holds our main processing loop!
    while(true)
    {
        //this is the matrix that we will store the picture within
        Mat input;
        //first, we need to lock the resource, to prevent corrupt heaps, stacks
        //and other memory locations. Those errors are very ugly and hard to
        //debug!
        pthread_mutex_lock(&m1);
        //now, we need to clone that public resource, the Matrix, into a local
        //one, that we can do whatever in the world we want to do with it!
        img.copyTo(input);
        pthread_mutex_unlock(&m1);
        //now, don't forget to unlock the resource! Otherwise, your program
        //will halt, waiting for the resource to unlock!

        //we need to validate the data of the image! If the image is empty,
        //it will cause serious errors and will crash the program. Instead of
        //going by the dangerous route and allowing the program to crash, we
        //will instead validate the data of the Matrix. All we need to check
        //is whether the image is empty or not!
        if(!input.empty())
        {
            //TODO: Type your code in over here! The camera input is in the
            //"input" matrix. Now, have fun with your code!
            //###############################################################
            
            //###############################################################
            //Your code must end here! All functions you add must go before
            //this one or it won't be visible to the compiler! Have a good
            //day and I hope you enjoy programming with this code template!

            //over here, we will check keystrokes on the main windows! Key
            //code 27 is the ASCII value for the [ESC] key. When that is
            //called, we perform exit(EXIT_SUCCESS); to halt the program send
            //back the code that means that the program executed successfully!
            //We need to get the keystroke and save it in a variable, an int
            //or a char. Then, we validate it!
            int key = waitKey(1);
            if(key == 27)
            {
                exit(EXIT_SUCCESS);
            }
        }
    }
    return 0;
}

//======================[DO NOT ENTER]==========================//
//=====================[BEHIND THE HOOD]========================//
//===================[ADVANCED USERS ONLY]======================//

//this is the thread, in which we grab the image!
void *grabber(void* threadAddr)
{
    long tid = (long) threadAddr;
    cout << "Grabber>> Grabber initialised!" << endl;
    cout << "Grabber>> Thread " << tid << endl;
    cout << "Grabber>> CAPTYPE\t" << capType << endl;
    cout << "Grabber>> CAPDEV\t" << capDev << endl;
    cout << "Grabber>> STREAM\t" << stream << endl;

    int failedFrames = 0;

    while(true)
    {
        if(capType == 0 || capType == 1 || capType == 2)
        {
            if(cam1)
            {
                failedFrames = 0;
                Mat tmp = cvQueryFrame(cam1);
                if(!tmp.empty())
                {
                    pthread_mutex_lock(&m1);
                    tmp.copyTo(img);
                    pthread_mutex_unlock(&m1);
                } else {
                    cout << "[ERROR][FATAL] Grabber>> Something went awefully wrong! I must exit!" << endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                cout << "[ERROR] Grabber>> Unable to pull frame!" << endl;
                failedFrames++;
                if(failedFrames == 128)
                {
                    cout << "[ERROR][FATAL] Grabber>> Camera not sending images for extended period!" << endl;
                    exit(EXIT_FAILURE);
                }
            }
        } else if(capType == 3) {
            Mat tmp = imread(stream.data());
            if(!tmp.empty())
            {
                pthread_mutex_lock(&m1);
                tmp.copyTo(img);
                pthread_mutex_unlock(&m1);
            } else {
                cout << "[ERROR][FATAL] Grabber>> Could not load the image! Will now exit!" << endl;
                exit(EXIT_FAILURE);
            }
        } else {
            cout << "[ERROR][FATAL] Grabber>> Invalid CAPTYPE ID. Please change those variables!" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

//this is the bootloader. It loads the main basic functions, and spawns
//the skeleton
int main()
{
    cout << "Bootloader>> Init!" << endl;
    cout << "Bootloader>> creating threads!" << endl;
    pthread_t threads[0];
    cout << "Bootloader>> setting up the camera!" << endl;
    if(capType == 0)
    {
        cout << "Bootloader>> Choosing internal camera!" << endl;
        cam1 = cvCaptureFromCAM(capDev);
    } else {
        if(capType == 1)
        {
            cout << "Bootloader>> Choosing file capture!" << endl;
            cam1 = cvCaptureFromFile(stream.data());
        } else {
            if(capType == 2)
            {
                cout << "Bootloader>> Choosing AVI file!" << endl;
                cam1 = cvCaptureFromAVI(stream.data());
            } else {
                if(capType == 3)
                {
                    cout << "Bootloader>> Choosing image!" << endl;
                }
            }
        }
    }
    cout << "Bootloader>> starting the image grabber thread!" << endl;
    pthread_create(&threads[0], NULL, grabber, (void *)0);
    cout << "Bootloader>> The grabber should be initialising now!" << endl;
    cout << "Bootloader>> Launching the program!" << endl;
    return run();
}
