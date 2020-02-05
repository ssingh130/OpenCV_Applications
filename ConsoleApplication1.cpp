//#include<stdio.h>
//#include<opencv2/opencv.hpp>
//#include<ximgproc/segmentation.hpp>
//#include<opencv2/core.hpp>
//#include <iostream>
//#include <ctime>
//
//using namespace cv;
//using namespace cv::ximgproc::segmentation;
//using namespace std;
//
//static void help() {
//	std::cout << std::endl << 
//	"Usage:" << std::endl << 
//	"./ssearch input_image (f|q)" << std::endl << 
//		"f=fast, q=quality" << std::endl <<
//		"Use l to display less rects, m to display more rects, q to quit" << std::endl;
//}
//
//int main(int argc, char** argv) {
//
//	if (argc < 3) {
//		help();
//		return -1;
//	}
//
//	Mat im = imread(argv[1]);
//	//resizing the image
//
//	int newHeight = 240;
//	int newWidth = im.cols * newHeight / im.rows;
//	resize(im, im, Size(newWidth, newHeight));
//
//	Ptr<SelectiveSearchSegmentation> ss = createSelectiveSearchSegmentation();
//	ss->setBaseImage(im);
//
//	if (argv[2][0] == 'f') {
//		ss->switchToSelectiveSearchFast();
//	}
//	else if (argv[2][0] == 'q') {
//		ss->switchToSelectiveSearchQuality();
//	}
//	else {
//		help();
//		return -2;
//	}
//	vector<Rect> rects;
//	ss->process(rects);
//	cout << "total No of Regional proposals:" << rects.size() << endl;;
//
//	int numShowRects = 100;
//	int increment = 50;
//
//	while (1) {
//		//creating a copy of the original image
//		Mat imOut = im.clone();
//
//		for (int i = 0; i < rects.size(); i++) {
//			if (i < numShowRects) {
//				rectangle(imOut,rects[i], Scalar(0, 255, 0));
//			}
//			else
//				break;
//		}
//		imshow("Output", imOut);
//		int k = waitKey();
//		if (k == 109) {
//			numShowRects += increment;
//		}
//		else if (k == 108 && numShowRects > increment) {
//			numShowRects -= increment;
//		}
//		else if (k == 113)
//			break;
//	}
//	return 0;
//}
//

//////////////////////Homography/////////////////////

//#include<opencv.hpp>
//using namespace cv;
//using namespace std;
//
//struct userdata {
//	Mat im;
//	vector<Point2f> pts;
//
//};
//void mouseHandler(int event, int x, int y, int flags, void* data_ptr) {
//	if (event == EVENT_LBUTTONDBLCLK) {
//		userdata* data = ((userdata*)data_ptr);
//		circle(data->im, Point(x, y), 3, Scalar(0, 155, 255), 5, LINE_AA
//		);
//		imshow("Image", data->im);
//		if (data->pts.size() < 4) {
//			data->pts.push_back(Point2f(x, y));
//		}
//	}
//}
//int main(int argc, char** argv) {
//	Mat im_src = imread("nar2.jpg");
//
//	Size size(300, 400);
//
//
//	Mat im_dst = Mat::zeros(size, CV_8UC3);
//	vector<Point2f>pt_dst;
//	pt_dst.push_back(Point2f(0,0));
//	pt_dst.push_back(Point2f(size.width-1,0));
//	pt_dst.push_back(Point2f(size.width-1,size.height-1));
//	pt_dst.push_back(Point2f(0,size.height-1));
//
//	//saving mouse clicks
//	Mat im_temp = im_src.clone();
//	userdata data;
//	data.im = im_temp;
//
//	cout << "Click for input--one corner at a time rhen hit ENTER"<<endl;
//	imshow("Image", im_temp);
//	setMouseCallback("Image", mouseHandler, &data);
//	waitKey(0);
//
//	Mat h = findHomography(data.pts, pt_dst);
//
//	warpPerspective(im_src, im_dst, h, size);
//
//	imshow("Dst img", im_dst);
//
//	waitKey(0);
//	return 0;
//}




////////Decting Aruco markers//////


//#include<opencv.hpp>
//#include<aruco.hpp>
//#include "aruco/dictionary.hpp"
//#include<sstream>
//#include<iostream>
//#include<fstream>
//#include<calib3d.hpp>
//
//using namespace std;
//using namespace cv;
//
//int main(){
//}
//void createArucoMarkers() {
//	Mat markerImg, markerImgCopy;
//	Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_1000);
//
//	//aruco::drawMarker(dictionary, 23, 200, markerImg, 1); //creating a marker
//	imwrite("marker23.png", markerImg);
//	VideoCapture inputVideo;
//	inputVideo.open(0);
//	
//	while (inputVideo.grab() )
//	{
//		inputVideo.retrieve(markerImgCopy);
//		markerImg.copyTo(markerImgCopy);
//		vector<int>ids;
//		vector<vector<Point2f>>corners;
//		aruco::detectMarkers(markerImg, dictionary, corners,ids);
//
//		//ATLEAST ONE MARKER DETECTED
//
//		if (ids.size() > 0) {
//			aruco::detectMarkers(markerImgCopy, dictionary, corners, ids);
//
//		}
//		imshow("out", markerImgCopy);
//		waitKey(0);
//		if (waitKey > 0)
//			break;
//
//	}
//};

/////////////////// DFT for grayscale /////////////////////////

#include<opencv.hpp>
//#include<aruco.hpp>
//#include "aruco/dictionary.hpp"
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<calib3d.hpp>

using namespace cv;
using namespace std;

void takeDFT(Mat& source, Mat& dest) {
	
	Mat dft_complex[2] = { source, Mat::zeros(source.size(),CV_32F) };
	Mat dft_ready, dft_done;
	merge(dft_complex, 2, dft_ready);
	dft(dft_ready, dft_done, DFT_COMPLEX_OUTPUT);
	dest = dft_done;

}

void showDFT(Mat& source) {
	Mat split_array[2] = { Mat::zeros(source.size(),CV_32F) ,Mat::zeros(source.size(),CV_32F)};
	 
	split(source, split_array);
	Mat dft_magnitude;
	magnitude(split_array[0], split_array[1], dft_magnitude);
	dft_magnitude += Scalar::all(1);
	log(dft_magnitude, dft_magnitude);
	normalize(dft_magnitude, dft_magnitude, 0,1, NORM_MINMAX);
	imshow("Output dft",dft_magnitude);
	waitKey();
}

int main(int argv, char** argc) {
	Mat dft_out_float;
	Mat dft_inp = imread("nar2.jpg",IMREAD_GRAYSCALE);
	dft_inp.convertTo(dft_out_float, CV_32FC1, 1.0 / 255.0);  ///converting to float values
	imshow("Input image", dft_inp);
	Mat dft_done;
	takeDFT(dft_out_float, dft_done);
	showDFT(dft_done);
}