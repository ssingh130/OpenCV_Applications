#include<stdio.h>
#include<opencv2/opencv.hpp>
#include<ximgproc/segmentation.hpp>
#include<opencv2/core.hpp>
#include <iostream>
#include <ctime>

using namespace cv;
using namespace cv::ximgproc::segmentation;
using namespace std;

static void help() {
	std::cout << std::endl << 
	"Usage:" << std::endl << 
	"./ssearch input_image (f|q)" << std::endl << 
		"f=fast, q=quality" << std::endl <<
		"Use l to display less rects, m to display more rects, q to quit" << std::endl;
}

int main(int argc, char** argv) {

	if (argc < 3) {
		help();
		return -1;
	}

	Mat im = imread(argv[1]);
	//resizing the image

	int newHeight = 240;
	int newWidth = im.cols * newHeight / im.rows;
	resize(im, im, Size(newWidth, newHeight));

	Ptr<SelectiveSearchSegmentation> ss = createSelectiveSearchSegmentation();
	ss->setBaseImage(im);

	if (argv[2][0] == 'f') {
		ss->switchToSelectiveSearchFast();
	}
	else if (argv[2][0] == 'q') {
		ss->switchToSelectiveSearchQuality();
	}
	else {
		help();
		return -2;
	}
	vector<Rect> rects;
	ss->process(rects);
	cout << "total No of Regional proposals:" << rects.size() << endl;;

	int numShowRects = 100;
	int increment = 50;

	while (1) {
		//creating a copy of the original image
		Mat imOut = im.clone();

		for (int i = 0; i < rects.size(); i++) {
			if (i < numShowRects) {
				rectangle(imOut,rects[i], Scalar(0, 255, 0));
			}
			else
				break;
		}
		imshow("Output", imOut);
		int k = waitKey();
		if (k == 109) {
			numShowRects += increment;
		}
		else if (k == 108 && numShowRects > increment) {
			numShowRects -= increment;
		}
		else if (k == 113)
			break;
	}
	return 0;
}
