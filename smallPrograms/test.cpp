#include <iostream>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main() {
	CvCapture *cam1 = cvCaptureFromCAM(0);
	while(1) {
		if(cam1) {
			Mat img = cvQueryFrame(cam1);
			imshow("Window",  img);
			if(waitKey(1) == 27) {
				break;
			}
		}
	}
	return 0;
}
