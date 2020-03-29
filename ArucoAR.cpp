#include<aruco.hpp>
#include<opencv.hpp>
#include<iostream>
#include"aruco/dictionary.hpp"

using namespace std;
using namespace cv;
using namespace aruco;

int DetectMarker(Mat& source) {


	Ptr <Dictionary> dictionary = getPredefinedDictionary(DICT_6X6_250);
	Ptr<DetectorParameters>parameters = DetectorParameters::create();

	//vectors that will contain the detected and the rejected markers
	vector<vector<Point2f>> markerCorners, rejectedCandidates;

	vector<int>markerIds;
	detectMarkers(source,dictionary,markerCorners,markerIds,parameters,rejectedCandidates);
	Mat h= findHomography()

	return 0;
}

int main(int argv, char** argc) {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	while (1) {
		Mat frame;
		//capture frame by frame
		cap >> frame;
		//if frame is empty break immediately
		if (frame.empty())
			break;
		imshow("frame", frame);

		Mat markerImg;
		Ptr <Dictionary> dictionary = getPredefinedDictionary(DICT_6X6_250);

		aruco::drawMarker(dictionary, 33, 200, markerImg, 1);
		// Press  ESC on keyboard to exit
		char c = (char)waitKey(25);
		if (c == 27)
			break;
	}
	Mat frame;
	DetectMarker(frame);
	cap.release();
	destroyAllWindows();
}