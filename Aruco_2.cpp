#include<opencv.hpp>
#include<opencv2/highgui.hpp>
#include<aruco.hpp>
#include "aruco/dictionary.hpp";

using namespace std;
using namespace cv;
using namespace aruco;


//function for pose estimation 
int PoseEstimate(Mat& inp, Mat& out) {

}

int main(int argc, char* argv[]) {
	VideoCapture inVid;
	inVid.open(0); //replace '0' with the filename in the project directory

	Ptr<Dictionary> Dict = getPredefinedDictionary(DICT_4X4_100); //defining and creating the dictionary
	while (inVid.grab()) {
		Mat img, imgCopy;
		inVid.retrieve(img);
		img.copyTo(imgCopy);

		vector<int>ids;
		vector<vector<Point2f>> corners;
		detectMarkers(img, Dict, corners, ids);

		if (ids.size() > 0) {
			drawDetectedMarkers(imgCopy, corners, ids);
		}
		imshow("out", imgCopy);
		char c = (char)waitKey(1);
		if (c == 27)
			break;
	}
}