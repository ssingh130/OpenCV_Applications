#include<opencv.hpp>
#include<aruco.hpp>
#include "aruco/dictionary.hpp"
#include<sstream>
#include<iostream>
#include<fstream>
#include<calib3d.hpp>

using namespace std;
using namespace cv;

int main(){
}
void createArucoMarkers() {
	Mat markerImg, markerImgCopy;
	Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_1000);

	//aruco::drawMarker(dictionary, 23, 200, markerImg, 1); //creating a marker
	imwrite("marker23.png", markerImg);
	VideoCapture inputVideo;
	inputVideo.open(0);
	
	while (inputVideo.grab() )
	{
		inputVideo.retrieve(markerImgCopy);
		markerImg.copyTo(markerImgCopy);
		vector<int>ids;
		vector<vector<Point2f>>corners;
		aruco::detectMarkers(markerImg, dictionary, corners,ids);

		//ATLEAST ONE MARKER DETECTED

		if (ids.size() > 0) {
			aruco::detectMarkers(markerImgCopy, dictionary, corners, ids);

		}
		imshow("out", markerImgCopy);
		waitKey(0);
		if (waitKey > 0)
			break;

	}
};