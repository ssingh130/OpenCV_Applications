

#include<opencv.hpp>
using namespace cv;
using namespace std;

struct userdata {
	Mat im;
	vector<Point2f> pts;

};
void mouseHandler(int event, int x, int y, int flags, void* data_ptr) {
	if (event == EVENT_LBUTTONDBLCLK) {
		userdata* data = ((userdata*)data_ptr);
		circle(data->im, Point(x, y), 3, Scalar(0, 155, 255), 5, LINE_AA
		);
		imshow("Image", data->im);
		if (data->pts.size() < 4) {
			data->pts.push_back(Point2f(x, y));
		}
	}
}
int main(int argc, char** argv) {
	Mat im_src = imread("nar2.jpg");

	Size size(300, 400);


	Mat im_dst = Mat::zeros(size, CV_8UC3);
	vector<Point2f>pt_dst;
	pt_dst.push_back(Point2f(0,0));
	pt_dst.push_back(Point2f(size.width-1,0));
	pt_dst.push_back(Point2f(size.width-1,size.height-1));
	pt_dst.push_back(Point2f(0,size.height-1));

	//saving mouse clicks
	Mat im_temp = im_src.clone();
	userdata data;
	data.im = im_temp;

	cout << "Click for input--one corner at a time rhen hit ENTER"<<endl;
	imshow("Image", im_temp);
	setMouseCallback("Image", mouseHandler, &data);
	waitKey(0);

	Mat h = findHomography(data.pts, pt_dst);

	warpPerspective(im_src, im_dst, h, size);

	imshow("Dst img", im_dst);

	waitKey(0);
	return 0;
}