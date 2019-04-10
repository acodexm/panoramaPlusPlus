#include "stdafx.h"
#include "histogram.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
using namespace std;
using namespace cv;
void histogram(Mat& image1, Mat& image2);
int main(int argc, char* argv[])
{
	Mat src_img, template_img, arg_img_1, arg_img_2, result_mat, debug_img;

	arg_img_1 = imread(argv[2], CV_LOAD_IMAGE_COLOR);
	if (arg_img_1.data == NULL) {
		printf("imread() failed...\n");
		return -1;
	}

	arg_img_2 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (arg_img_2.data == NULL) {
		printf("imread() failed...\n");
		return -1;
	}
	if (arg_img_1.cols > arg_img_2.cols || arg_img_1.rows > arg_img_2.rows) {
		src_img = arg_img_1;
		template_img = arg_img_2;
	}
	else {
		src_img = arg_img_2;
		template_img = arg_img_1;
	}


	cvtColor(src_img, debug_img, CV_RGB2GRAY);
	cvtColor(debug_img, debug_img, CV_GRAY2BGR);

	// method: CV_TM_SQDIFF, CV_TM_SQDIFF_NORMED, CV_TM _CCORR, CV_TM_CCORR_NORMED, CV_TM_CCOEFF, CV_TM_CCOEFF_NORMED
	int match_method = CV_TM_CCORR_NORMED;


	matchTemplate(src_img, template_img, result_mat, match_method);
	normalize(result_mat, result_mat, 0, 1, NORM_MINMAX, -1, Mat());

	double minValue; double maxValue;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(result_mat, &minValue, &maxValue, &minLoc, &maxLoc, Mat());
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)  matchLoc = minLoc;
	else matchLoc = maxLoc;
	cout << "matchLoc: " << matchLoc << " | ";
	cout << "minVal: " << minValue << " | ";
	cout << "minVal: " << minValue << " | ";
	cout << "minLoc: " << minLoc << " | ";
	cout << "maxLoc: " << maxLoc << "\n";
	rectangle(
		debug_img,
		matchLoc,
		Point(matchLoc.x + template_img.cols, matchLoc.y + template_img.rows),
		CV_RGB(255, 0, 0),
		3);

	imshow("debug_img", debug_img);

	Mat diffImage,currentImage;

	Rect contour(matchLoc, Point(matchLoc.x + template_img.cols, matchLoc.y + template_img.rows));
	
	currentImage = src_img.clone()(contour);
	
	absdiff(currentImage, template_img, diffImage);

	Mat foregroundMask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);

	float threshold = 30.0f;
	float dist;

	for (int j = 0; j < diffImage.rows; ++j)
		for (int i = 0; i < diffImage.cols; ++i)
		{
			Vec3b pix = diffImage.at<Vec3b>(j, i);

			dist = (pix[0] * pix[0] + pix[1] * pix[1] + pix[2] * pix[2]);
			dist = sqrt(dist);

			if (dist > threshold)
			{
				foregroundMask.at<unsigned char>(j, i) = 255;
			}
		}

	imshow("diffImage", diffImage);
	imshow("foregroundMask", foregroundMask);

	histogram(currentImage, template_img);

	waitKey();
}
