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
}
