
/////////////////// DFT for grayscale /////////////////////////

#include<opencv.hpp>
//#include<aruco.hpp>
//#include "aruco/dictionary.hpp"
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<calib3d.hpp>
#include"firstheader.h"

using namespace cv;
using namespace std;

void takeDFT(Mat& source, Mat& destination) {
	
	Mat dft_complex[2] = { source, Mat::zeros(source.size(),CV_32F) };
	Mat dft_ready, dft_done;
	merge(dft_complex, 2, dft_ready);
	dft(dft_ready, dft_done, DFT_COMPLEX_OUTPUT);
	destination = dft_done;
	//cout << "DFT Complex" << dft_done;
}
void recenterDFT(Mat& source) {
	int CenterX = source.cols / 2;
	int CenterY = source.rows / 2;
	Mat q1(source, Rect(0, 0, CenterX, CenterY));
	Mat q2(source, Rect(CenterX, 0, CenterX, CenterY));
	Mat q3(source, Rect(0, CenterY, CenterX, CenterY));
	Mat q4(source, Rect(CenterX, CenterY, CenterX, CenterY));
	Mat swapMap;
	q1.copyTo(swapMap);
	q4.copyTo(q1);
	swapMap.copyTo(q1);

	q2.copyTo(swapMap);
	q3.copyTo(q2);
	swapMap.copyTo(q3);
};

void showDFT(Mat& source) {
	Mat split_array[2] = { Mat::zeros(source.size(), CV_32F) , Mat::zeros(source.size(),CV_32F) };
	split(source, split_array);
	Mat dft_magnitude;
	magnitude(split_array[0], split_array[1], dft_magnitude);
	dft_magnitude += Scalar::all(1);
	log(dft_magnitude, dft_magnitude);
	normalize(dft_magnitude, dft_magnitude, 0, 1, NORM_MINMAX, CV_8UC1);
	//Mat dft_split;
	//dft_magnitude.convertTo(dft_split, CV_32F,1.0/255.0);
	//split(dft_magnitude, dft_split[0]);
	imshow("Output dft", dft_magnitude);
	//cout << "DFT" << dft_magnitude;
	recenterDFT(dft_magnitude);
	waitKey(0);
};

void invertingDFT(Mat& source, Mat& dest) {
	Mat dft_inv;
	dft(source, dft_inv, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	dest = dft_inv;
}

int main(int argv, char** argc) {
	Mat dft_out_float;
	Mat dft_inp = imread("nar2.jpg", IMREAD_GRAYSCALE);
	dft_inp.convertTo(dft_out_float, CV_32FC1, 1.0 / 255.0);  ///converting to float values
	imshow("Input image", dft_inp);
	Mat dft_done;
	takeDFT(dft_out_float, dft_done);
	showDFT(dft_done);
	Mat invertedDFT;
	invertingDFT(dft_done, invertedDFT);
	//imshow("INverted DFT", invertedDFT);
	waitKey(0);
};


