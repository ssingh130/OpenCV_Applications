#include<opencv.hpp>
#include<iostream>
#include"firstheader.h"

using namespace cv;
using namespace std;




void SimplestCB(Mat& in, Mat& out, float percent) {
    assert(in.channels() == 3);
    assert(percent > 0 && percent < 100);

    float half_percent = percent / 200.0f;

    vector<Mat> tmpsplit; split(in, tmpsplit);
    for (int i = 0; i < 3; i++) {
        //find the low and high precentile values (based on the input percentile)
        Mat flat; tmpsplit[i].reshape(1, 1).copyTo(flat);
        cv::sort(flat, flat, SORT_EVERY_ROW + SORT_ASCENDING);
        int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
        int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
        cout << lowval << " " << highval << endl;

        //saturate below the low percentile and above the high percentile
        tmpsplit[i].setTo(lowval, tmpsplit[i] < lowval);
        tmpsplit[i].setTo(highval, tmpsplit[i] > highval);

        //scale the channel
        normalize(tmpsplit[i], tmpsplit[i], 0, 255, NORM_MINMAX);
    }
    merge(tmpsplit, out);
}

// Usage example
//void main() {
//    Mat tmp, im = imread("nar2.jpg");
//
//    SimplestCB(im, tmp, 1);
//
//    imshow("orig", im);
//    imshow("balanced", tmp);
//    waitKey(0);
//    return;
//}
 

int main(int argc, char** argv) {
	Mat input = imread("nar2.jpg");
    
    if (!input.data) {
        cout << "Error Loading image" << endl;
        return -1;
    }

    namedWindow("prew", WINDOW_AUTOSIZE);
    //slider global values
    int iSliderValue1 = 0;
    int iSliderValue2 = 0;
    int iSliderValue3 = 0;
    createTrackbar("R", "prew", &iSliderValue1, 215);
    createTrackbar("G", "prew", &iSliderValue2, 215);
    createTrackbar("B", "prew", &iSliderValue3, 215);

    

    Mat img;
    input.copyTo(img);
    for (int x = 0; x < input.cols - 1; x++) {
        for (int y = 0; y < input.rows - 1; y++) {
            img.at<Vec3b>(y, x)[0] = img.at<Vec3b>(y, x)[0] * (iSliderValue1) / 200;
            img.at<Vec3b>(y, x)[1] = img.at<Vec3b>(y, x)[1] * (iSliderValue2) / 200;
            img.at<Vec3b>(y, x)[2] = img.at<Vec3b>(y, x)[2] * (iSliderValue3) / 200;

        }
    }


    SimplestCB(input, img, 50.0);

    imshow("Input image", input);
    imshow("balanced", img);
	waitKey(0);
	return 0;
}
