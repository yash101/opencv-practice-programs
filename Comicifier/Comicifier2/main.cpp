//Welcome! This is the OpenCV programming started template, written
//by Devyash Lodha! This template is meant to give newbie programmers
//quite a bit of a head start when learning OpenCV. The truth is that
//when you start writing complex algorithms with OpenCV, as you add
//more processing stages, the processing time will increase too. If
//the rate at which you are processing the images drops below the rate
//at which you are getting the image, the buffers will quickly start
//to fill up, and your application will have a ton of lag. There are
//many ways to get past this problem! One powerful method is to thread
//the camera. Doing so allows the camera image to be downloaded right
//as it is available. When we download the new image, we delee the old
//one to prevent memory leaks! This template has a bootloader, which
//gets the camera started and makes it possible to get started with
//your code! The bootloader turns on the camera, and then launches the
//thread, which connects to the camera. The bootloader then launches
//your code!

//How to get started:
//There is a "run" function in this template. It must return an integer!
//This is the function where the image is copied locally, then supplied
//to you for processing! This is the function you will typically code
//within! There is another function, the grabber! This is the function
//that loads the image from the camera. This is actually a pointer, so it
//points to a memory location, which contains the code. This function is
//run in a separate thread! The grabber has multiple failsafes to attempt
//to recover from errors, to find invalid capture IDs and many other things
//that can go wrong. If any fatal errors are detected, the grabber will
//exit() with the EXIT_FAILURE ID, which will end all threads and end the
//program. The error debug will be sent to STDOUT. The function main() is
//the bootloader. It will call the run() function when it has done launching
//everything. If you have some function or item that you want performed
//before launching the run() function, place it in the main() function,
//in the position you desire!

//these are the main includes that we will use! QCoreApplication is used
//by QT to initialise the program. highgui.hpp is the OpenCV HighGUI
//module, used for the Graphical User Interface, and other basic things.
//It also interfaces us to the camera! The objdetect.hpp file is the
//ObjDetect module, and is used for object detection and processing. The
//header, imgproc.hpp is for processing images. We will use this for
//converting color spaces and for thresholding, typically, however, that
//just scratches the surface of this library! We will use OpenCV's calib3d
//modle to give us access to camera calibration and many of those features
//that will make your program even more accurate! However, this is only to
//be used by experienced OpenCV'ers! pthread.h is used for threading, making
//it possible for us to run multiple tasks parallel to each other, increasing
//efficiency! We use ctime to give us access to some time features. This is
//used by the bootloader to generate the seed for the random generator!
//sys/stat.h is used by the skeleton to check if directories exist!
#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <pthread.h>
#include <ctime>
#include <sys/stat.h>

//add your other includes here:
//for example, to get some advanced trig and math functions,
//#include <cmath>
#include <opencv2/photo/photo.hpp>
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

//This is the path at which this skeleton shall reserve to save folders
//and files! This is where you should store all your configuration files
//and save debug! Debug can be in the form of images, videos, or maybe
//even a text/plain console output log!
const string path = "~/Desktop/CV-SKELETON/PROGRAMS/";
//This is the name of the project! This will be used in the generation
//of the directory structure, and is meant to keep all the files from
//each project saparate from each other!
const string project_name = "FaceChanger";
const string project_path = path + project_name;

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

//This is the image save number we will use! We will start off with a
//random number, generated by the bootloader. Whenever we take a picture,
//we will increment this number so we do not end up overwriting any files
//from the latest run!
long num = 0;

//====================[CAMERA SETUP]=====================
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
//If you use an AVI file, or a cvCaptureFromFile() (MJPEG, etc.), you
//need to provide the location of either the stream or the file to open!
//Otherwise, the skeleton won't be able to resolve the location and will
//crash! Be sure to include http:// if you are using a video stream.
//For Filesystems, feel free to use either relative or absolute filenames!
static string stream = "http://10.11.65.13/mjpg/video.mjpg";

//Now, let's get coding! Before we can start, we need to get some things
//covered! You must set up the capType, capDev and stream variables above!
//They tell the bootloader and the grabber what input device to use! There
//is a table of the possible outcomes of these variables above! After you
//have set up your camera, you can get started coding. Your processing
//goes in the space, labeled, "TODO: Type your code in over here!", after
//the line of equal signs. Now, you may wonder, how to get the image to
//process?! Well, that's the main point of this template. All the threading
//has been done for you already! There is a cv::Mat, called "input". This
//is the camera data! Feel free to use up this Matrix. For example, if you
//wanted to change it's color space, you may attempt using:
//cvtColor(input, input, CV_BGR2GRAY|HSV|ETC.);! Also, feel free to make
//your own Matrices, etc. e.g. Mat gray!

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
            //===============================================================//


            //create the window in which we will display the image!
            namedWindow("Image", CV_WINDOW_OPENGL);
            cv::setOpenGlContext("Image");

            fastNlMeansDenoisingColored(input, input, 3, 3, 7, 21);
            cvtColor(input, input, CV_BGR2Lab);
            cvtColor(input, input, CV_Lab2BGR);
            //the size of the camera input
            Size regSize(640, 480);

            //create all the matrices we will use throughout our processing
            //loop! Gray = grayscale. edges is a picture with all the edges.
            //mask is the mask we will apply. small is a rescaled image, of
            //half the dimensions. big is the image of the same size of the camera
            Mat gray, edges, mask, small, big;

            //let's convert the input image to grayscale and save it into gray
            cvtColor(input, gray, CV_BGR2GRAY);
            //let's do a medianBlur to calculate the noise and get rid of it!
            medianBlur(gray, gray, 7);
            //this is an edge detector. It uses the variant between multiple
            //pixels and draws a dot if it great enough!
            Laplacian(gray, edges, CV_8U, 5);
            //this converts the edges into a binary image, and inverts it, to
            //have a white background!
            threshold(edges, mask, 80, 255, CV_THRESH_BINARY_INV);

            //this is half the size of the camera!
            Size half(regSize.width / 2, regSize.height / 2);
            //we resize the original image to the size we defined before!
            resize(input, small, half, 0, 0, INTER_LINEAR);

            //we blow it up back to the camera size!
            resize(small, big, regSize, 0, 0, INTER_LINEAR);
            //this is the destination matrix, the picture we display on the screen!
            Mat dst;
            //We clear the destination image
            dst.setTo(0);
            //We copy the input image into the output image, but only where
            //the mask is white!
            big.copyTo(dst, mask);

            //let's show the image on the screen now! Hooray!!! We are done!
            imshow("Image", dst);


            //===============================================================//
            //Your code must end here! All functions you add must go before
            //this one or it won't be visible to the compiler! Have a good
            //day and I hope you enjoy programming with this code template!

            //over here, we will check keystrokes on the main windows! Key
            //code 27 is the ASCII value for the [ESC] key. When that is
            //called, we perform exit(EXIT_SUCCESS); to halt the program send
            //back the code that means that the program executed successfully!
            //We need to get the keystroke and save it in a variable, an int
            //or a char. Then, we validate it!
            int key = 0;
            key = waitKey(1);
            if(key != -1)
            {
                cout << "Key code: " << key << " pressed!" << endl;
            }
            //ESC key
            if(key == 27 || key == 131099)
            {
                exit(EXIT_SUCCESS);
            } else {
                //F1 key (Without SHIFT)
                if(key == 65470) {

                }
            }
        }
    }
    return 0;
}

//======================[DO NOT ENTER]==========================//
//=====================[BEHIND THE HOOD]========================//

//this is the thread, in which we grab the image!
void *grabber(void* threadAddr)
{
    //We want to have the thread ID, for debug. This is optional, but is nice
    //to have!
    long tid = (long) threadAddr;
    //debug the capture variables defined
    cout << "Grabber>> Grabber initialised!" << endl;
    cout << "Grabber>> Thread " << tid << endl;
    cout << "Grabber>> CAPTYPE\t" << capType << endl;
    cout << "Grabber>> CAPDEV\t" << capDev << endl;
    cout << "Grabber>> STREAM\t" << stream << endl;

    //the number of frames that failed to download. This is used for error detection
    //and prevention, and helps safely exit the program instead of crashing it.
    int failedFrames = 0;

    //the main infinite loop. This is what happens constantly. We will grab the image
    //and do the basic things with it in here
    while(true)
    {
        //check the camera type. These use CvCapture, and thus we need
        //to use cvQueryFrame()! Those are the Capture Types, CAPTYPE = 0, 1, or 2.
        if(capType == 0 || capType == 1 || capType == 2)
        {
            //check to see if the camera is available!
            if(cam1)
            {
                //reset the fail ticker because we know that the camera is working now!
                failedFrames = 0;
                //this is a temporary image in which we store the image. We don't want
                //to create a bottleneck, so we first get the image then we save it.
                //This allows us to prevalidate the image data before holding back the
                //program and validating while the image is locked!
                Mat tmp = cvQueryFrame(cam1);
                //check to make sure the image is valid
                if(!tmp.empty())
                {
                    //lock the resources so we do not end up corrupting memory and thus
                    //crashing the program
                    pthread_mutex_lock(&m1);
                    //copy the temporary matrix data into the global one so when we save
                    //it into the global matrix, we can prevalidate, and then quickly
                    tmp.copyTo(img);
                    //unlock the resources so we can allow other threads to continue with
                    //their execution.
                    pthread_mutex_unlock(&m1);
                } else {
                    //this happens if something wierd happens. This almost never
                    //triggers!
                    cout << "[ERROR][FATAL] Grabber>> Something went awefully wrong! I must exit!" << endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                //if the camera failed or was unavailable atm.
                cout << "[ERROR] Grabber>> Unable to pull frame!" << endl;

                //increment the failure counter
                failedFrames++;

                //if enough failures happened, just give up and quit to prevent the program
                //from crashing!
                if(failedFrames == 1280)
                {
                    cout << "[ERROR][FATAL] Grabber>> Camera not sending images for extended period!" << endl;
                    exit(EXIT_FAILURE);
                }
            }
            //we can also process jpegs, pngs, tiffs, gifs and much more! :D. That allows
            //us to debug our processing loop when we don't have a camera on hand!
        } else if(capType == 3) {
            //open the image file and copy it's data
            Mat tmp = imread(stream.data());
            //if the image was not empty, we're golden! Just need to send the image to
            //the global image!
            if(!tmp.empty())
            {
                //lock the resources
                pthread_mutex_lock(&m1);
                //copy the image
                tmp.copyTo(img);
                //unlock the resources
                pthread_mutex_unlock(&m1);
            } else {
                //otherwise, the image was invalid and we couldn't open/read it! We will
                //then exit, sadly :(
                cout << "[ERROR][FATAL] Grabber>> Could not load the image! Will now exit!" << endl;
                exit(EXIT_FAILURE);
            }
        } else {
            //If the capType was wrong, e.g. 6, you get this error and the program quits!
            //This is a programmer errorn and must be fixed immediately
            cout << "[ERROR][FATAL] Grabber>> Invalid CAPTYPE ID. Please change those variables!" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

//This function checks the status of a directory. Does it exist or
//not! It returns a true if the directory was found, and false if
//the directory is missing
bool checkDir(string path)
{
    //This creates a struct for stat. stat is a way to check if a
    //directory exists
    struct stat st;
    //We check the status of the directory here.
    if(!stat(path.data(), &st))
    {
        if(st.st_mode & S_IFDIR)
        {
            //If the directory exists, we return a true.
            return true;
        }
    } else {
        //If the directory does not exist, we return a false!
        return false;
    }

    return false;
}

//This uses system() to make a directory. This uses mkdir -p, so the
//entire directory structure. This way, the direcory you were looking
//for is going to be there unless deleted by the user or another program
//while this program was running
bool makedir(string path)
{
    //We use the check the directory, if it exists
    if(checkDir(path))
    {
        //if the directory exists, we are golden and we do not need to
        //worry. We just send back a true to tell that the operation
        //completed successfully!
        return true;
    } else {
        //if the file path is nothing, we know that it is invalid. We
        //return false because we know the outcome will be negative!
        if(path.length() == 0)
        {
            return false;
        }

        //We need to formulate the command to send to the system. We will
        //use mkdir -p because mkdir is used to create directories in
        //POSIX. The -p switch tells mkdir to create the entire directory
        //path so we do not have to run mkdir for each hierarchical level
        //in the path!
        string command = "mkdir -p " + path;
        //We need to make sure the command executes successfully! If mkdir
        //returns false, something has gone wrong and we need to send back
        //false to let the program know something went wrong. Otherwise,
        //return a true!
        if(!system(command.data()))
        {
            //we return true and exit from the function!
            return true;
        } else {
            //We return false and exit from the function!
            return false;
        }
    }
}

//this is the bootloader. It loads the main basic functions, and spawns
//the skeleton
int main()
{
    //The bootloader has just started
    cout << "Bootloader>> init!" << endl;
    //Message the user what the bootloader is performing
    cout << "Bootloader>> Checking saves directory and creating it if nonexistent!" << endl;
    cout << "Bootloader>> Checking directories and making missing ones!" << endl;

    //Create the project configuration folder
    if(makedir(project_path + "/config"))
    {
        cout << "Bootloader>> checked directory: " << project_path << "/config" << endl;
    }

    //Create project save folder
    if(makedir(project_path + "/saves"))
    {
        cout << "Bootloader>> checked directory: " << project_path << "/config" << endl;
    }

    //Create the project debug folder
    if(makedir(project_path + "/debug"))
    {
        cout << "Bootloader>> checked directory: " << project_path << "/debug" << endl;
    }

    //Start up by creating a random image starting number
    cout << "Bootloader>> Generating seed for the random number generator!" << endl;
    srand(time(0));
    cout << "Bootloader>> Getting a random number from the seed!" << endl;
    cout << "Bootloader>> Using 0 as minimum number, and 65535 as maximum number!" << endl;
    //get the random number. 65535 is the max number, and and 0 is the minimum!
    num = rand() % 65535 + 0;
    cout << "Bootloader>> Done generating random number! Number is: " << num << "!" << endl;
    cout << "Bootloader>> creating threads!" << endl;
    //Define the number of threads!
    pthread_t threads[0];
    cout << "Bootloader>> setting up the camera!" << endl;
    //set up the camera based on the variables defined above
    if(capType == 0)
    {
        cout << "Bootloader>> Choosing internal camera!" << endl;
        cam1 = cvCaptureFromCAM(capDev);
    } else {
        if(capType == 1)
        {
            cout << "Bootloader>> Choosing file capture!" << endl;
            //cam1 = cvCaptureFromFile(stream.data());
            cam1 = cvCreateFileCapture("http://10.11.65.13/mjpg/video.mjpg");
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
    //create the grabber thread!
    pthread_create(&threads[0], NULL, grabber, (void *)0);
    cout << "Bootloader>> The grabber should be initialising now!" << endl;
    cout << "Bootloader>> Launching the program!" << endl;
    //run the main loop. This is the function that holds your code
    return run();
}
