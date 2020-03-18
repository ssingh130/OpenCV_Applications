#include<opencv.hpp>
#include<opencv_modules.hpp>
#include<iostream>

using namespace std;
using namespace cv;


int thresh = 100;
RNG rng(12345);
//void thresh_callback(int, void**);
vector<Point> contoursConvexHull(vector<vector<Point>>contours)
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
	Mat input = imread("shapes.jpg",IMREAD_GRAYSCALE);
	Mat srcblur;
	GaussianBlur(input, srcblur, Size(9, 9),2,2);
	Mat srcCanny;
	Canny(srcblur, srcCanny, 0, 0, 3, true);
	//converting the image to binary form
	Mat thr;
	threshold(input, thr, 150, 255, THRESH_BINARY);
	imshow("Input", thr);
	
	vector<vector<Point>> contours;
	vector<Vec4i> result;
	vector<Vec4i> hierarchy;
	vector<Vec3f>circles;

	//detecting circles using hough transform
	Mat srcHough;
	input.copyTo(srcHough);
	HoughCircles(srcCanny,circles,HOUGH_GRADIENT,1,srcCanny.rows/8,255,100,0,0);
	for (size_t i = 0; i < circles.size(); i++) {
		Point center(circles[i][0], circles[i][1]);
		int radius = circles[i][2];
		//cirle center
		circle(srcHough, center, 3, Scalar(0, 0, 255), -1, 8, 0);
		//circle outline
		circle(srcHough, center, radius, Scalar(10, 155, 255), 3, 8, 0);
	}
	
	imshow("Hough",srcHough);

	//finding contours

	findContours(thr, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat outputContour= cv::Mat::zeros(thr.size(),CV_8UC3);

	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(100, 255, 255);
		drawContours(outputContour, contours, i, color, 2);
	}
	vector<Point>convexHullpoints=contoursConvexHull(contours);
	polylines(outputContour, convexHullpoints, true, Scalar(0, 0, 255), 2);
	polylines(input, convexHullpoints, true, Scalar(0, 0, 255), 2);

	imshow("orig with contours", input);
	imshow("contours", outputContour);
	waitKey(0);
}
