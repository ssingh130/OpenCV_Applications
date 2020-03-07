#include<opencv.hpp>
#include<opencv_modules.hpp>
#include<iostream>
#include <opencv2/core/types_c.h>


using namespace std;
using namespace cv;


int thresh = 100;
RNG rng(12345);
//void thresh_callback(int, void**);
vector<Point> contoursConvexHull(vector<vector<Point> > contours)
{
	vector<Point> result;
	vector<Point> pts;
	for (size_t i = 0; i < contours.size(); i++)
		for (size_t j = 0; j < contours[i].size(); j++)
			pts.push_back(contours[i][j]);
	convexHull(pts, result);
	return result;
}
void main(int argc, char** argv) {

	//reading an image in color
	Mat input = imread("nar2.jpg",IMREAD_GRAYSCALE);
	Mat srcblur;
	blur(input, srcblur, Size(3, 3));
	Mat srcCanny;
	Canny(srcblur, srcCanny, 0, 100, 3, true);
	//converting the image to binary form
	Mat thr;
	threshold(input, thr, 100, 255, THRESH_BINARY);
	imshow("Input", thr);
	
	vector<vector<Point>> contours;
	vector<Vec4i> result;
	vector<Vec4i> hierarchy;


	//finding contours


	findContours(thr, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat outputContour= cv::Mat::zeros(thr.size(),CV_8UC3);

	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(255, 255, 255);
		drawContours(outputContour, contours, i, color, 2);
	}
	vector<Point>convexHullpoints=contoursConvexHull(contours);
	polylines(outputContour, convexHullpoints, true, Scalar(0, 0, 255), 2);

	imshow("contours", outputContour);
	waitKey(0);

}