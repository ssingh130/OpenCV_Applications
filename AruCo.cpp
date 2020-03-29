#include<opencv.hpp>
#include<aruco.hpp>
#include "aruco/dictionary.hpp"
#include<sstream>
#include<iostream>
#include<fstream>
#include<calib3d.hpp>

using namespace std;
using namespace cv;


int main() {
	
	
	VideoCapture cap(0);
	Mat cameraMatrix, distCoeffs;
	Mat markerImg, markerImgCopy;
	Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_1000);
	//aruco::drawMarker(dictionary, 23, 200, markerImg, 1); //creating a marker
	imwrite("marker23.png", markerImg);
	while (1)
	{
		cap.retrieve(markerImg);
		cap >> markerImg;
		markerImg.copyTo(markerImgCopy);
		vector<int>ids;
		vector<vector<Point2f>>corners;
		aruco::detectMarkers(markerImg, dictionary, corners,ids);

		//ATLEAST ONE MARKER DETECTED

		if (ids.size() > 0) {
			aruco::detectMarkers(markerImgCopy, dictionary, corners, ids);
			vector<Vec3d> rvecs, tvecs;
			aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
			//drawing the axis
			for (int i = 0; i < ids.size(); i++)
				aruco::drawAxis(markerImgCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);

		}
		imshow("out", markerImgCopy);
		char key = (char)cv::waitKey(25);
		if (key == 27)
			break;

	}
	cap.release();
	destroyAllWindows();
	return -1;
};