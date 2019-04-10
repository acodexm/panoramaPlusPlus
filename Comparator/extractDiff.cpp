#include "extractDiff.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>

Mat extractDiff(Mat& image1, Mat& image2, Point& matchLoc) {
	Mat diffImage, currentImage;
	Rect contour(matchLoc, Point(matchLoc.x + image2.cols, matchLoc.y + image2.rows));

	currentImage = image1.clone()(contour);

	absdiff(currentImage, image2, diffImage);

	Mat foregroundMask = Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);

	float threshold = 10.0f;
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
	return currentImage;
}