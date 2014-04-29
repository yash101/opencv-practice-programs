#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
#include <time.h>

using namespace std;
using namespace cv;

//feature selection:
bool trackTargets = 1;
bool trackBalls = 1;
bool trackOtherRobots = 0;
bool useNiftyInterface = 1;
bool calculateDistance = 1;
bool unmodifiedFeed = 1;
int view = 0;
String image_uri = "~/devx/saves/DevVision2014";

//environment variables
bool onboard = 0;
int captype = 0;
int capdev = 0;
int imageWidth = 320;
int imageHeight = 240;
double camFOV = 67;
double pi = 3.14159;
string uri = "http://10.11.65.11/mjpg/video.mjpg";

//arena variables
//--Static Target
double staticTarget_width = 4;
double staticTarget_height = 32;
double staticTarget_distanceFromFloor = 37.5;
//--Dynamic Target
double dynamicTarget_width = 23.5;
double dynamicTarget_height = 4;
double dynamicTarget_distanceFromFloor = 68;
//--Ball Variables
double arena_ballheight = 25;
double arena_ballwidth = 25;

//robot variables
double cam_distanceFromFront = 22;
double cam_distanceFromFloor = 46;

//Game Variables
bool redAlliance = 1;

//Target Tracking Variables
//--TARGETS
int target_hueslidermin = 0;
int target_hueslidermax = 1;
int target_satslidermin = 0;
int target_satslidermax = 1;
int target_valslidermin = 0;
int target_valslidermax = 1;
int target_accuracy = 1;
int target_blurKernelSquare = 2;
int target_minArea = 500;
int target_maxArea = 10000;

//--BALLS
int ball_hueslidermin = 0;
int ball_hueslidermax = 0;
int ball_satslidermin = 0;
int ball_satslidermax = 0;
int ball_valslidermin = 0;
int ball_valslidermax = 0;
int ball_accuracy = 1;
int ball_blurKernelSquare = 2;
int ball_minarea = 500;
int ball_maxarea = 10000;
int ball_minsides = 3;
int ball_maxsides = 100;

//objects
CvCapture *cam1;

//Program Variables
int fframes = 0;
int maxfframes = 32;
int fontface = CV_FONT_HERSHEY_SIMPLEX;
const string confLocation = "~/devx/config/DevVision2014/conf1.devx";

void nl(int repetitions) {
	for (int i = 0; i < repetitions; i++) {
		cout << "\n$";
	}
}

void msg(string message) {
	cout << "\n$ " + message;
}

string bool2str(bool in) {
	stringstream str;
	str << in;
	return str.str();
}

string int2str(int inp) {
	stringstream str;
	str << inp;
	return str.str();
}

string double2str(double in) {
	stringstream str;
	str << in;
	return str.str();
}

string float2str(float in) {
	stringstream str;
	str << in;
	return str.str();
}

string longDouble2str(long double in) {
	stringstream str;
	str << in;
	return str.str();
}

double tanDeg(double input) {
	return tan((input) * pi / 180);
}

double hypotenuse(double legA, double legB) {
	return sqrt(((legA*legA) + (legB*legB)));
}

double leg(double hypotenuse, double leg) {
	return sqrt(((hypotenuse*hypotenuse) - (leg*leg)));
}

double inch2feet(double inches) {
	double feet = inches / 12;
	return feet;
}

double feet2inch(double feet) {
	double inches = feet * 12;
	return inches;
}

double inch2cm(double inch) {
	double cm = inch * 2.544;
	return cm;
}

double cm2inch(double cm) {
	double inch = cm / 2.544;
	return inch;
}

double cm2meter(double cm) {
	double meter = cm / 1000;
	return meter;
}

bool readConfig() {
	msg("===Config File Read Started===");
	ifstream fin(confLocation);
	if (fin.is_open()) {
		fin >> trackTargets;
		fin >> trackBalls;
		fin >> trackOtherRobots;
		fin >> useNiftyInterface;
		fin >> calculateDistance;
		fin >> onboard;
		fin >> captype;
		fin >> capdev;
		fin >> imageWidth;
		fin >> imageHeight;
		fin >> camFOV;
		fin >> pi;
		fin >> uri;
		fin >> staticTarget_width;
		fin >> staticTarget_height;
		fin >> staticTarget_distanceFromFloor;
		fin >> dynamicTarget_width;
		fin >> dynamicTarget_height;
		fin >> dynamicTarget_distanceFromFloor;
		fin >> arena_ballheight;
		fin >> arena_ballwidth;
		fin >> cam_distanceFromFront;
		fin >> cam_distanceFromFloor;
		fin >> redAlliance;
		fin >> target_hueslidermin;
		fin >> target_hueslidermax;
		fin >> target_satslidermin;
		fin >> target_satslidermax;
		fin >> target_valslidermin;
		fin >> target_valslidermax;
		fin >> target_accuracy;
		fin >> target_blurKernelSquare;
		fin >> target_minArea;
		fin >> target_maxArea;
		fin >> ball_hueslidermin;
		fin >> ball_hueslidermax;
		fin >> ball_satslidermin;
		fin >> ball_satslidermax;
		fin >> ball_valslidermin;
		fin >> ball_valslidermax;
		fin >> ball_accuracy;
		fin >> ball_blurKernelSquare;
		fin >> ball_minarea;
		fin >> ball_maxarea;
		fin >> ball_minsides;
		fin >> ball_maxsides;
		fin >> maxfframes;
		fin.close();
		msg("Configuration Read Successfully!");
		return 1;
	} else {
		msg("Read Failure!");
		return 0;
	}
}

bool writeConfig() {
	msg("===Configuration being written!===");
	ofstream fout(confLocation);
	fout << trackTargets;
	fout << endl;
	fout << trackBalls;
	fout << endl;
	fout << trackOtherRobots;
	fout << endl;
	fout << useNiftyInterface;
	fout << endl;
	fout << calculateDistance;
	fout << endl;
	fout << onboard;
	fout << endl;
	fout << captype;
	fout << endl;
	fout << capdev;
	fout << endl;
	fout << imageWidth;
	fout << endl;
	fout << imageHeight;
	fout << endl;
	fout << camFOV;
	fout << endl;
	fout << pi;
	fout << endl;
	fout << uri;
	fout << endl;
	fout << staticTarget_width;
	fout << endl;
	fout << staticTarget_height;
	fout << endl;
	fout << staticTarget_distanceFromFloor;
	fout << endl;
	fout << dynamicTarget_width;
	fout << endl;
	fout << dynamicTarget_height;
	fout << endl;
	fout << dynamicTarget_distanceFromFloor;
	fout << endl;
	fout << cam_distanceFromFront;
	fout << endl;
	fout << cam_distanceFromFloor;
	fout << endl;
	fout << arena_ballheight;
	fout << endl;
	fout << arena_ballwidth;
	fout << endl;
	fout << redAlliance;
	fout << endl;
	fout << target_hueslidermin;
	fout << endl;
	fout << target_hueslidermax;
	fout << endl;
	fout << target_satslidermin;
	fout << endl;
	fout << target_satslidermax;
	fout << endl;
	fout << target_valslidermin;
	fout << endl;
	fout << target_valslidermax;
	fout << endl;
	fout << target_accuracy;
	fout << endl;
	fout << target_blurKernelSquare;
	fout << endl;
	fout << target_minArea;
	fout << endl;
	fout << target_maxArea;
	fout << endl;
	fout << ball_hueslidermin;
	fout << endl;
	fout << ball_hueslidermax;
	fout << endl;
	fout << ball_satslidermin;
	fout << endl;
	fout << ball_satslidermax;
	fout << endl;
	fout << ball_valslidermin;
	fout << endl;
	fout << ball_valslidermax;
	fout << endl;
	fout << ball_accuracy;
	fout << endl;
	fout << ball_blurKernelSquare;
	fout << endl;
	fout << ball_minarea;
	fout << endl;
	fout << ball_maxarea;
	fout << endl;
	fout << ball_minsides;
	fout << endl;
	fout << ball_maxsides;
	fout << endl;
	fout << maxfframes;
	fout << endl;
	fout.close();
	msg("Write Successful!");
	return 1;
}

bool confCam() {
	msg("Image Width\n$\t=");
	cin >> imageWidth;
	msg("Image Height\n$\t=");
	cin >> imageHeight;
	msg("Camera Field of View\n$\t=");
	cin >> camFOV;
	msg("Please enter the camera type, then [RETURN]");
	msg("\t0=Internal\n$\t1=MJPEG\n$\t2=AVI\n$\t3=Image\n$\t=");
	cin >> captype;
	if (captype == 0 || captype == 1 || captype == 2 || captype == 3) {
		if (captype == 0) {
			msg("You have selected an internal camera! Device ID\n$\t=");
			cin >> capdev;
		} else if (captype == 1) {
			msg("You have selected an MJPEG stream! Stream URL\n$\t=");
			cin >> uri;
		} else if (captype == 2) {
			msg("You have selected an AVI File! Path\n$\t=");
			cin >> uri;
		} else if (captype == 3) {
			msg("You have selected an image! Path\n$\t=");
			cin >> uri;
		}
	} else {
		msg("Invalid code!\n$\t");
		cin.get();
		confCam();
		return 0;
	}
	writeConfig();
	return 0;
}

void featureList() {
	nl(32);
	msg("====================================");
	msg("===========Feature List=============");
	msg("Name           ||   ID   || Enabled ");
	msg("===============++========++=========");
	msg("Track Targets? ||    1   ||    " + bool2str(trackTargets));
	msg("------------------------------------");
	msg("Track Balls?   ||    2   ||    " + bool2str(trackBalls));
	msg("------------------------------------");
	msg("track Robots?  ||    3   ||    " + bool2str(trackOtherRobots));
	msg("------------------------------------");
	msg("calc Distance? ||    4   ||    " + bool2str(calculateDistance));
	msg("------------------------------------");
	msg("Interface?     ||    5   ||    " + bool2str(useNiftyInterface));
	msg("------------------------------------");
	msg("LiveFeed       ||    6   ||    " + bool2str(unmodifiedFeed));
	msg("------------------------------------");
	msg("saves directory||    7   ||    1");
	msg("====================================");
	msg("8 = done; press the ID with [RETURN]");
	msg("         to toggle value!           ");
	msg("====================================");
	msg("==========Saves Directory===========");
	msg(image_uri);
	msg("====================================");
}
void featureSelect() {
	msg("===Feature Selection===");
	int id = 1000;
	while (id != 8) {
		featureList();
		msg("====\n$ || ID=");
		cin >> id;
		if (id == 1) {
			if (trackTargets) {
				trackTargets = 0;
			} else {
				trackTargets = 1;
			}
		} else if (id == 2) {
			if (trackBalls) {
				trackBalls = 0;
			} else {
				trackBalls = 1;
			}
		} else if (id == 3) {
			if (trackOtherRobots) {
				trackOtherRobots = 0;
			} else {
				trackOtherRobots = 1;
			}
		} else if (id == 4) {
			if (calculateDistance) {
				calculateDistance = 0;
			} else {
				calculateDistance = 1;
			}
		} else if (id == 7) {
			msg("Please enter the new URL\n$\t=");
			cin >> image_uri;
		} else if (id == 5) {
			if (useNiftyInterface) {
				useNiftyInterface = 0;
			} else {
				useNiftyInterface = 1;
			}
		} else if (id == 6) {
			if (unmodifiedFeed) {
				unmodifiedFeed = 0;
			} else {
				unmodifiedFeed = 1;
			}
		}

	}
}
void math() {
	msg("===Math Setup===");
	msg("Set up Pi? 1=true/0=false\n$\t=");
	bool y = 0;
	cin >> y;
	if (y) {
		msg("Pi=");
		cin >> pi;
	} else {
		msg("Pi not set!");
	}
}

void configure() {
	msg("===Configuration Utility Started===");
	confCam();
	writeConfig();
	featureSelect();
	writeConfig();
	math();
	msg("===Configuration Complete===");
	cin.get();
	writeConfig();
}

void minMaxFix() {
	//TARGET
	if (target_hueslidermax <= target_hueslidermin) {
		target_hueslidermax = target_hueslidermin + 1;
	}
	if (target_satslidermax <= target_satslidermin) {
		target_satslidermax = target_satslidermin + 1;
	}
	if (target_valslidermax <= target_valslidermin) {
		target_valslidermax = target_valslidermin + 1;
	}
	if (target_accuracy < 1) {
		target_accuracy = 1;
	}
	if (target_blurKernelSquare < 1) {
		target_blurKernelSquare = 1;
	}
	//BALL
	if (ball_hueslidermax <= ball_hueslidermin) {
		ball_hueslidermax = ball_hueslidermin + 1;
	}
	if (ball_satslidermax <= ball_satslidermin) {
		ball_satslidermax = ball_satslidermin + 1;
	}
	if (ball_valslidermax <= ball_valslidermin) {
		ball_valslidermax = ball_valslidermin + 1;
	}
	if (ball_accuracy < 1) {
		ball_accuracy = 1;
	}
	if (ball_blurKernelSquare < 1) {
		ball_blurKernelSquare = 1;
	}
}

int main() {
	if (readConfig()) {
		msg("Configuration Read Successful!");
	} else {
		msg("Configuration Missing or corrupt!");
		configure();
	}

	if (captype == 0) {
		cam1 = cvCaptureFromCAM(capdev);
		waitKey(1000);
	} else if (captype == 1) {
		cam1 = cvCaptureFromFile(uri.data());
		waitKey(5000);
	} else if (captype == 2) {
		cam1 = cvCaptureFromAVI(uri.data());
		waitKey(100);
	} else if (captype == 3) {
		waitKey(100);
	}

	while (1) {
		Mat in, gui;
		bool ok = 0;
		if (captype == 3) {
			in = imread(uri);
			if (in.data) {
				ok = 1;
			} else {
				ok = 0;
			}
		} else {
			if (cam1) {
				in = cvQueryFrame(cam1);
				ok = 1;
			} else {
				ok = 0;
			}
		}

		if (ok) {
			namedWindow("DevVision2014", CV_WINDOW_FREERATIO);
			fframes = 0;
			//get keyclick!
			int k = 0;
			k = waitKey(1);

			//=======LiveFeed!========
			if (unmodifiedFeed) {
				namedWindow("LiveFeed", CV_WINDOW_FREERATIO);
				imshow("LiveFeed", in);
			}

			//create GUI
			in.copyTo(gui);

			//=======target detection code section========
			if (trackTargets) {
				//create matrices and copy the input image into the processed image
				Mat img, dst, thresh;
				in.copyTo(img);

				//create sliders if that feature is enabled otherwise use config file
				if (useNiftyInterface) {
					//set up windows then sliders
					//==Polygons = targets processed
					namedWindow("Targets", CV_WINDOW_KEEPRATIO);
					namedWindow("Targets HSV Image", CV_WINDOW_KEEPRATIO);
					namedWindow("HSV Targets", CV_WINDOW_KEEPRATIO);
					createTrackbar("Hue Min", "HSV Targets", &target_hueslidermin, 254, 0);
					createTrackbar("Hue Max", "HSV Targets", &target_hueslidermax, 255, 0);
					createTrackbar("Sat Min", "HSV Targets", &target_satslidermin, 254, 0);
					createTrackbar("Sat Max", "HSV Targets", &target_satslidermax, 255, 0);
					createTrackbar("Val Min", "HSV Targets", &target_valslidermin, 254, 0);
					createTrackbar("Val Max", "HSV Targets", &target_valslidermax, 255, 0);
					createTrackbar("Error", "HSV Targets", &target_accuracy, 25, 0);
					createTrackbar("Kernel", "HSV Targets", &target_blurKernelSquare, 100, 0);
				}
				//just fix min and max values to prevent a crash
				minMaxFix();

				//create the output window for the Polygons

				//image conversion and preprocessing
				img.copyTo(dst);
				cvtColor(img, img, CV_BGR2HSV);
				inRange(img, Scalar(target_hueslidermin, target_satslidermin, target_valslidermin), Scalar(target_hueslidermax, target_satslidermax, target_valslidermax), img);
				threshold(img, img, 128, 255, THRESH_BINARY);
				blur(img, img, Size(target_blurKernelSquare, target_blurKernelSquare), Point(-1, -1), 4);
				threshold(img, img, 128, 255, THRESH_BINARY);
				img.copyTo(thresh);

				//start processing
				vector<vector<Point> > target_contours;
				vector<Vec4i> target_hierarchy;
				findContours(img, target_contours, target_hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));
				vector<vector<Point> > target_contour_poly(target_contours.size());
				vector<Rect> target_boundRect(target_contours.size());

				int contour_x = 0;
				int contour_y = 0;
				int contour_width = 0;
				int contour_height = 0;
				int hotX = 0;
				int hotY = 0;
				int statX = 0;
				int statY = 0;
				double staticDist_hyp = 0;
				double staticDist_plane = 0;
				double dynamicDist_hyp = 0;
				double dynamicDist_plane = 0;
				bool dynamic = 0;
				bool dynamicF = 0;

				for (int i = 0; i < target_contours.size(); i++) {
					if (contourArea(target_contours[i]) > target_minArea && contourArea(target_contours[i]) < target_maxArea) {
						approxPolyDP(target_contours[i], target_contour_poly[i], target_accuracy, 1);
						if (!isContourConvex(target_contour_poly[i])) {
							continue;
						}
						target_boundRect[i] = boundingRect(Mat(target_contour_poly[i]));
						contour_x = target_boundRect[i].x;
						contour_y = target_boundRect[i].y;
						contour_width = target_boundRect[i].width;
						contour_height = target_boundRect[i].height;

						if (contour_width > contour_height * 4 && contour_width < contour_height * 9) {
							dynamic = 1;
							dynamicF = 1;
							hotX = contour_x + (contour_width / 2);
							hotY = contour_y + (contour_height / 2);
							double height = (imageHeight * dynamicTarget_height) / contour_height;
							dynamicDist_hyp = height / tanDeg(camFOV / 2);
							double dff = (dynamicTarget_distanceFromFloor + (dynamicTarget_height / 2)) - cam_distanceFromFloor;
							dynamicDist_plane = (leg(dynamicDist_hyp, dff)) - cam_distanceFromFront;
							rectangle(gui, Point(contour_x, contour_y), Point(contour_x + contour_width, contour_y + contour_height), Scalar(64, 128, 255), 1, 8, 0);
						}
						if (contour_height > contour_width * 4 && contour_height < contour_width * 9) {
							dynamic = 0;
							double height = (imageHeight * staticTarget_height) / contour_height;
							staticDist_hyp = height / tanDeg(camFOV / 2);
							double dff = (staticTarget_distanceFromFloor + (staticTarget_height / 2)) - cam_distanceFromFloor;
							staticDist_plane = (leg(staticDist_hyp, dff)) - cam_distanceFromFront;
							statX = contour_x + (contour_width / 2);
							statY = contour_y + (contour_height / 2);
							rectangle(gui, Point(contour_x, contour_y), Point(contour_x + contour_width, contour_y + contour_height), Scalar(255, 128, 64), 1, 8, 0);
							string text = "Static: " + double2str(staticDist_plane / 12) + "ft.";
							putText(gui, text, Point(5, 15), fontface, 0.5, Scalar(255, 0, 0), 1, 8);
						}
					}
				}

				if (dynamicF) {
					msg("Hot Target!!! (" + int2str(hotX) + "," + int2str(hotY) + ")");
				}

				if (useNiftyInterface) {
					Scalar tcolor = Scalar(0, 0, 255);
					Scalar rcolor = Scalar(255, 0, 0);
					Scalar pcolor = Scalar(0, 255, 0);
					Scalar ccolor = Scalar(255, 255, 255);
					bool draw = 0;

					for (int i = 0; i < target_contours.size(); i++) {
						if (contourArea(target_contours[i]) > target_minArea && contourArea(target_contours[i]) < target_maxArea) {
							approxPolyDP(target_contours[i], target_contour_poly[i], target_accuracy, 1);
							if (!isContourConvex(target_contour_poly[i])) {
								continue;
							}
							target_boundRect[i] = boundingRect(Mat(target_contour_poly[i]));
							contour_x = target_boundRect[i].x;
							contour_y = target_boundRect[i].y;
							contour_width = target_boundRect[i].width;
							contour_height = target_boundRect[i].height;

							if (contour_width > contour_height * 4 && contour_width < contour_height * 8) {
								dynamic = 1;
								dynamicF = 1;
								draw = 1;
								hotX = contour_x + (contour_width / 2);
								hotY = contour_y + (contour_height / 2);
							}
							if (contour_height > contour_width * 4 && contour_height < contour_width * 9) {
								dynamic = 0;
								draw = 1;
							}
							if (draw) {
								if (target_contour_poly[i].size() == 3) {
									drawContours(dst, target_contour_poly, i, tcolor, 3, 8, target_hierarchy, 0, Point());
								} else {
									if (target_contour_poly[i].size() == 4) {
										drawContours(dst, target_contour_poly, i, rcolor, 2, 8, target_hierarchy, 0, Point());
									} else {
										if (target_contour_poly[i].size() == 5) {
											drawContours(dst, target_contour_poly, i, pcolor, 3, 8, target_hierarchy, 0, Point());
										}
									}
								}
								if (dynamic) {
									rectangle(dst, Point(contour_x, contour_y), Point(contour_x + contour_width, contour_y + contour_height), Scalar(64, 128, 255), 1, 8, 0);
								} else {
									rectangle(dst, Point(contour_x, contour_y), Point(contour_x + contour_width, contour_y + contour_height), Scalar(255, 128, 64), 1, 8, 0);
								}
							}
						}
					}
					string text = "Static: " + double2str(staticDist_plane / 12) + "ft.";
					putText(dst, text, Point(5, 15), fontface, 0.5, Scalar(255, 0, 0), 1, 8);
					putText(gui, text, Point(5, 15), fontface, 0.5, Scalar(255, 0, 0), 1, 8);
					Point x = getTextSize(text, fontface, 0.5, 1, 0);
					int xpos = x.x / 2;
					circle(dst, Point(statX,statY), 3, Scalar(255, 0, 255), 1, 8, 0);
					line(dst, Point(xpos, 15), Point(statX, statY), Scalar(255, 0, 255), 1, 8, 0);
					line(gui, Point(xpos, 15), Point(statX, statY), Scalar(255, 0, 255), 1, 8, 0);
					if (dynamicF) {
						putText(dst, "HOT!", Point(5, imageHeight - 5), fontface, 0.5, Scalar(0, 255, 0), 1, 8);
						putText(gui, "HOT!", Point(5, imageHeight - 5), fontface, 0.5, Scalar(0, 255, 0), 1, 8);
						Point dsf = getTextSize("HOT!", fontface, 0.5, 1, 0);
						xpos = dsf.x / 2;
						circle(dst, Point(hotX, hotY), 3, Scalar(0, 255, 0), 2, 8, 0);
						line(dst, Point(xpos, imageHeight - 20), Point(hotX, hotY), Scalar(32, 128, 32), 1, 8, 0);
					} else {
						putText(dst, "NOT!", Point(5, 15), fontface, 05, Scalar(0, 0, 255), 1, 8);
						putText(gui, "NOT!", Point(5, 15), fontface, 05, Scalar(0, 0, 255), 1, 8);
					}

					imshow("Targets HSV Image", thresh);
					imshow("Targets", dst);
				}

				//hotkeys for special functions like record!
				if (k == 'a') {
					string location_poly = image_uri + int2str(rand()) + "_targetDST.jpg";
					imwrite(location_poly, dst);
					msg("Wrote to " + location_poly);
				} else if (k == 'b') {
					string location_poly = image_uri + int2str(rand()) + "_targetThresh.jpg";
					imwrite(location_poly, thresh);
					msg("Wrote to " + location_poly);
				}
			}
			//ball detection code
			if (trackBalls) {
				Mat thresh, dst, img;
				if (useNiftyInterface) {
					//Main output window
					namedWindow("Balls", CV_WINDOW_KEEPRATIO);
					//Thresholded image
					//Sliders
					namedWindow("HSV Balls", CV_WINDOW_KEEPRATIO);
					createTrackbar("Hue Min", "HSV Balls", &ball_hueslidermin, 254, 0);	//hue min
					createTrackbar("Hue Max", "HSV Balls", &ball_hueslidermax, 255, 0);	//hue max
					createTrackbar("Sat Min", "HSV Balls", &ball_satslidermin, 254, 0);	//sat min
					createTrackbar("Sat Max", "HSV Balls", &ball_satslidermax, 255, 0);	//sat max
					createTrackbar("Val Min", "HSV Balls", &ball_valslidermin, 254, 0);	//val min
					createTrackbar("Val Max", "HSV Balls", &ball_valslidermax, 255, 0);	//val max
					createTrackbar("Accuracy", "HSV Balls", &ball_accuracy, 25, 0);		//accuracy
					createTrackbar("Kernel", "HSV Balls", &ball_blurKernelSquare, 100, 0);	//blur kernel square radius
				}
				//create ball only windows and sliders
				//copy input image to the ball processing matrix
				in.copyTo(img);
				//copy the ball processing matrix to the output image before any processing happens
				img.copyTo(dst);
				//convert the color space of img to HSV so it is easier to process the image
				cvtColor(img, img, CV_BGR2HSV);
				//threshold the image with a lower and upper bound
				inRange(img, Scalar(ball_hueslidermin, ball_satslidermin, ball_valslidermin), Scalar(ball_hueslidermax, ball_satslidermax, ball_valslidermax), img);
				//convert to binary image
				threshold(img, img, 128, 256, THRESH_BINARY);
				//blur to eradicate noise
				blur(img, img, Size(ball_blurKernelSquare, ball_blurKernelSquare), Point(-1, -1), 4);
				//convert to binary again to remove weighting
				threshold(img, img, 128, 255, THRESH_BINARY);
				//copy the main image to the thresh matrix to be displayed as the HSV thresholded image
				img.copyTo(thresh);

				//start processing
				//create vectors
				//a vector of vectors of cv::Point for the curve data
				vector<vector<Point> > ball_contours;
				//a vector for the hierarchy
				vector<Vec4i> ball_hierarchy;
				//find the contours (curves). Save the contours to ball_contours, hierarchy to ball_hierarchy
				//use mode CV_RETR LIST, method CV_CHAIN_APPROX_NONE
				findContours(img, ball_contours, ball_hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));
				//a vector for the polygons
				vector<vector<Point> > ball_contours_poly(ball_contours.size());
				//a vector for the bonding rectangles
				vector<Rect> ball_boundRect(ball_contours.size());

				//filter and calculate every variable and find the distance, one at a time for each contour. Display a ton of information inside many windows if opted for!
				for (int i = 0; i < ball_contours.size(); i++) {
					// 1st. filter. Uses a minimum and maximum size to make sure the shape is neither too big nor too small!
					if (contourArea(ball_contours[i]) > ball_minarea && contourArea(ball_contours[i]) < ball_maxarea) {
						//2nd. filter. check convexity
						/*if (!isContourConvex(ball_contours_poly[i])) {
							continue;
						}*/
						//figure out how many sides the contour has!
						approxPolyDP(ball_contours[i], ball_contours_poly[i], ball_accuracy, true);
						//3nd. filter, make minimum sides!
						if (ball_contours_poly[i].size() > ball_minsides && ball_contours_poly[i].size() < ball_maxsides) {
							//calculate the bounding rectangle
							ball_boundRect[i] = boundingRect(Mat(ball_contours_poly[i]));
							//now, the bounding rectangle is stored in the vector, ball_boundRect, at the index of i
							//4th. filter, make sure the ratio of width to height is acceptable.
							//define boolean for continue or not
							bool goodAspectTest = 0;
							//check if the width is greater than 0.8 times the height
							if (ball_boundRect[i].width > ball_boundRect[i].height * 1.2) {
								//set flag to 1
								goodAspectTest = 1;
							}
							//check if the height is greater than 0.8 of the height
							if (ball_boundRect[i].height > ball_boundRect[i].width * 1.2) {
								//set flag to 1
								goodAspectTest = 1;
							}
							//check if the height is equal to the width (1:1 ratio)
							if (ball_boundRect[i].width == ball_boundRect[i].height) {
								//set flag to 1
								goodAspectTest = 1;
							}
							//if the ratio test passed, perform the actual math, etc.
							if (goodAspectTest) {
								//radius of the detected ball
								int radius = ball_boundRect[i].width / 2;
								//diameter of the detected ball
								int diam = radius * 2;
								//center of the ball
								Point center(ball_boundRect[i].x + (ball_boundRect[i].width / 2), ball_boundRect[i].y + (ball_boundRect[i].height / 2));
								//height of the target being seen
								double height = 0;
								//distance to the ball
								double dist = 0;
								//opt whether to calculate the distance or not
								if (calculateDistance) {
									//the height of the ball as the camera is seeing
									height = (imageHeight * arena_ballheight) / ball_boundRect[i].width;
									//the distance, in inches
									dist = height / tanDeg(camFOV / 2);
								}

								//circle on the GUI
								circle(gui, center, radius, Scalar(255, 255, 0), 1, 8, 0);
								//draw the distance and a line to the center point
								//define text
								string message = "Dist: " + double2str(inch2feet(dist)) + "ft.";
								//get the text size
								Size textsize = getTextSize(message, fontface, 0.5, 1, 0);
								//where the text will be written
								Point textlocation(imageWidth - textsize.width, imageHeight - 15);
								//write the text
								putText(gui, message, textlocation, fontface, 0, 5, 1, 8);

								//check if the user wants a million windows
								if (useNiftyInterface) {
									//draw a circle on the UI if true
									circle(dst, center, radius, Scalar(255, 255, 0), 2, 8, 0);
									//write the distance to the circle on the screen
									putText(dst, message, textlocation, fontface, 0, 5, 1, 8);
								}
							}
						}
					}
				}

				//opt wheter to use a fancy display or not
				if (useNiftyInterface) {
					//show the thresholded image
					imshow("Balls HSV Image", thresh);
					//show the processed image
					imshow("Balls", dst);
				}
			}
			//Other robot tracking
			if (trackOtherRobots) {

			}
			//Display
			if (useNiftyInterface) {

			}
			imshow("DevVision2014", gui);
			waitKey(1);
			if (k == 27) {
				writeConfig();
				destroyAllWindows();
				return 0;
			} else if (k == 'c') {
				destroyAllWindows();
				readConfig();
				configure();
				main();
				return 0;
			} else if (k == 's') {
				writeConfig();
			} else if (k == 'r') {
				readConfig();
			} else if (k == 'f') {
				destroyAllWindows();
				featureSelect();
				writeConfig();
				main();
				return 0;
			} else if (k == 'd') {
				string location_poly = image_uri + int2str(rand()) + "_liveFeed.jpg";
				imwrite(location_poly, in);
				msg("Image Written to " + location_poly);
			} else if (k == 'e') {
				string location_poly = image_uri + int2str(rand()) + "_GUI.jpg";
				imwrite(location_poly, gui);
				msg("Image Written to " + location_poly);
			}
		} else {
			fframes++;
			waitKey(1);
			if (fframes > maxfframes) {
				fframes = 0;
				msg("!!!!!!!!!!!" + int2str(maxfframes) + " Failed Frames!!!!!!!!!!!\n$\t\tRECONFIGURING!");
				destroyAllWindows();
				confCam();
				main();
				return 0;
			}
		}
	}
	return 0;
}
