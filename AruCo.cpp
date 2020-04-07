//This program is about detecting the aruco markers already created in the previous file

#include<opencv.hpp>
#include<aruco.hpp>
#include "aruco/dictionary.hpp";
#include<sstream>
#include<iostream>
#include<fstream>
#include<calib3d.hpp>


using namespace std;
using namespace cv;
using namespace aruco;

namespace {
	const char* about = "Basic marker detection";
	const char* keys =
		"{d        |       | dictionary: DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,"
		"DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6, DICT_5X5_1000=7, "
		"DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10, DICT_6X6_1000=11, DICT_7X7_50=12,"
		"DICT_7X7_100=13, DICT_7X7_250=14, DICT_7X7_1000=15, DICT_ARUCO_ORIGINAL = 16,"
		"DICT_APRILTAG_16h5=17, DICT_APRILTAG_25h9=18, DICT_APRILTAG_36h10=19, DICT_APRILTAG_36h11=20}"
		"{v        |       | Input from video file, if ommited, input comes from camera }"
		"{ci       | 0     | Camera id if input doesnt come from video (-v) }"
		"{c        |       | Camera intrinsic parameters. Needed for camera pose }"
		"{l        | 0.1   | Marker side length (in meters). Needed for correct scale in camera pose }"
		"{dp       |       | File of marker detector parameters }"
		"{r        |       | show rejected candidates too }"
		"{refine   |       | Corner refinement: CORNER_REFINE_NONE=0, CORNER_REFINE_SUBPIX=1,"
		"CORNER_REFINE_CONTOUR=2, CORNER_REFINE_APRILTAG=3}";
}

static bool readCameraParameters(string filename, Mat& camMat, Mat& distCoeffs) {
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["camera_matrix"] >> camMat;
	fs["distortion_coefficients"] >> distCoeffs;
	return true;
}
static bool readDetectorParam(string filename, Ptr<DetectorParameters>& params) {
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
	fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
	fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
	fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
	fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
	fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
	fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
	fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
	fs["minDistanceToBorder"] >> params->minDistanceToBorder;
	fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
	fs["cornerRefinementMethod"] >> params->cornerRefinementMethod;
	fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
	fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
	fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
	fs["markerBorderBits"] >> params->markerBorderBits;
	fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
	fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
	fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
	fs["minOtsuStdDev"] >> params->minOtsuStdDev;
	fs["errorCorrectionRate"] >> params->errorCorrectionRate;
	return true;
}

int main(int argc, char* argv[] ) {
	CommandLineParser parser(argc, argv, keys);
	parser.about(about);

	if (argc < 2) {
		parser.printMessage();
		return 0;
	}

	//parser arguments 
	int dictionaryId = parser.get<int>("d");
	bool showRejected = parser.has("r");
	bool estimatePose = parser.has("c");
	float markerLength = parser.get<float>("l");


	VideoCapture cap(0);
	Mat cameraMatrix, distCoeffs;

	Mat markerImg, markerImgCopy;
	Ptr<DetectorParameters> detectorParams = DetectorParameters::create(); 
	if (parser.has("dp")) {
		bool readOk = readDetectorParam(parser.get<string>("dp"), detectorParams);
		if (!readOk) {
			cerr << "Invalid Parameters file" << endl;
			return 0;
		}
	}

	if (parser.has("refine")) {
		detectorParams->cornerRefinementMethod = parser.get<int>("refine");
	}
	int camId = parser.get<int>("ci");
	//video input
	String video;
	if (parser.has("v")) {
		video = parser.get<String>("v");
	}
	if (!parser.check()) {
		parser.printErrors();
		return 0;
	}

	Ptr<Dictionary> dictionary = getPredefinedDictionary(PREDEFINED_DICTIONARY_NAME(dictionaryId));

	Mat camMat;
	if (estimatePose) {
		bool readOk = readCameraParameters(parser.get<string>("c"), camMat, distCoeffs);
		if (!readOk) {
			cerr << "Invalid camera file" << endl;
			return 0;
		}
	}

	VideoCapture inputVideo;
	int waitTime;
	if (!video.empty()) {
		inputVideo.open(video);
		waitTime = 0;
	}
	else {
		inputVideo.open(camId);
		waitTime = 10;
	}
	double totalTime = 0;
	int totalIterations = 0;

	while (inputVideo.grab()) {
		Mat img, imgCopy;
		inputVideo.retrieve(img);

		double tick = (double)getTickCount();

		vector<int>ids;
		vector<vector<Point2f>>corners, rejected;
		vector<Vec3d>rvecs, tvecs;
		// detect markers and estimate pose
		aruco::detectMarkers(img, dictionary, corners, ids, detectorParams, rejected);
		if (estimatePose && ids.size() > 0)
			aruco::estimatePoseSingleMarkers(corners, markerLength, camMat, distCoeffs, rvecs,
				tvecs);

		double currentTime = ((double)getTickCount() - tick) / getTickFrequency();
		totalTime += currentTime;
		totalIterations++;
		if (totalIterations % 30 == 0) {
			cout << "Detection Time = " << currentTime * 1000 << " ms "
				<< "(Mean = " << 1000 * totalTime / double(totalIterations) << " ms)" << endl;
		}

		// draw results
		img.copyTo(imgCopy);
		if (ids.size() > 0) {
			aruco::drawDetectedMarkers(imgCopy, corners, ids);

			if (estimatePose) {
				for (unsigned int i = 0; i < ids.size(); i++)
					aruco::drawAxis(imgCopy, camMat, distCoeffs, rvecs[i], tvecs[i],
						markerLength * 0.5f);
			}
		}

		if (showRejected && rejected.size() > 0)
			aruco::drawDetectedMarkers(imgCopy, rejected, noArray(), Scalar(100, 0, 255));

		imshow("out", imgCopy);
		char key = (char)waitKey(waitTime);
		if (key == 27) break;
	}

	return 0;
}

