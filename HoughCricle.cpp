#include<opencv.hpp>
#include<opencv_modules.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main() {
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
		//imshow("frame", frame);

		Mat gray_frame;
		cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
		//imshow("gray", gray_frame);

		GaussianBlur(gray_frame, gray_frame, Size(9, 9), 2, 2);
		vector<Vec3f>circles;
		int p1 = 75;
		HoughCircles(gray_frame, circles, HOUGH_GRADIENT, 1, gray_frame.rows / 64, p1, p1*0.4, 25, 200);

		//Drawing Circles
		for (size_t i = 0; i < circles.size(); i++) {
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			//circle center
			circle(frame, center, 2, Scalar(0, 255,0),-1,3,0);
			//outer circle
			circle(frame, center, radius, Scalar(0, 0, 255), 3);
		}
		imshow("HCT", frame);
		// Press  ESC on keyboard to exit
		char c = (char)waitKey(25);
		if (c == 27)
			break;
	}


	cap.release();
	destroyAllWindows();
	return 0;
}
