#include "stdafx.h"
#include "../../../../../opencv3.3.1/opencv/build/include/opencv2/imgproc.hpp"
using namespace std;
using namespace cv;
#include "cropp.h"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>

#define TAG "crop image "
#define ENABLE_LOG true
#define LOG(msg) std::cout << msg
#define LOGLN(msg) std::cout << msg << std::endl
bool checkInteriorExterior(const cv::Mat& mask, const cv::Rect& interiorBB, int& top, int& bottom, int& left, int& right)
{
	// return true if the rectangle is fine as it is!
	bool returnVal = true;

	cv::Mat sub = mask(interiorBB);

	int x = 0;
	int y = 0;

	// count how many exterior pixels are at the
	LOGLN("count exterior pixels");
	int cTop = 0; // top row
	int cBottom = 0; // bottom row
	int cLeft = 0; // left column
	int cRight = 0; // right column
	// and choose that side for reduction where mose exterior pixels occured (that's the heuristic)

	for (y = 0, x = 0; x < sub.cols; ++x)
	{
		// if there is an exterior part in the interior we have to move the top side of the rect a bit to the bottom
		if (sub.at< char>(y, x) == 0)
		{
			returnVal = false;
			++cTop;
		}
	}

	for (y = sub.rows - 1, x = 0; x < sub.cols; ++x)
	{
		// if there is an exterior part in the interior we have to move the bottom side of the rect a bit to the top
		if (sub.at< char>(y, x) == 0)
		{
			returnVal = false;
			++cBottom;
		}
	}

	for (y = 0, x = 0; y < sub.rows; ++y)
	{
		// if there is an exterior part in the interior
		if (sub.at< char>(y, x) == 0)
		{
			returnVal = false;
			++cLeft;
		}
	}

	for (x = sub.cols - 1, y = 0; y < sub.rows; ++y)
	{
		// if there is an exterior part in the interior
		if (sub.at< char>(y, x) == 0)
		{
			returnVal = false;
			++cRight;
		}
	}

	// that part is ugly and maybe not correct, didn't check whether all possible combinations are handled. Check that one please. The idea is to set `top = 1` iff it's better to reduce the rect at the top than anywhere else.
	if (cTop > cBottom)
	{
		if (cTop > cLeft)
			if (cTop > cRight)
				top = 1;
	}
	else
		if (cBottom > cLeft)
			if (cBottom > cRight)
				bottom = 1;

	if (cLeft >= cRight)
	{
		if (cLeft >= cBottom)
			if (cLeft >= cTop)
				left = 1;
	}
	else
		if (cRight >= cTop)
			if (cRight >= cBottom)
				right = 1;



	return returnVal;
};

bool sortX(cv::Point a, cv::Point b)
{
	return a.x < b.x;
};

bool sortY(cv::Point a, cv::Point b)
{
	return a.y < b.y;
};
void cropp(Mat& result) {
	LOGLN("cropping...");
	int64 cropp_start_time = getTickCount();

	Mat gray;
	result.convertTo(result, CV_8U);
	cvtColor(result, gray, CV_BGR2GRAY);

	// extract all the black background (and some interior parts maybe)
	Mat mask1 = gray > 0;

	// now extract the outer contour
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;

	findContours(mask1, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	Mat contourImage = Mat::zeros(result.size(), CV_8UC3);;

	// find contour with max elements
	// remark: in theory there should be only one single outer contour surrounded by black regions!!

	int maxSize = 0;
	int id = 0;
	for (int i = 0; i < contours.size(); ++i)
	{
		if (contours.at(i).size() > maxSize)
		{
			maxSize = (int)contours.at(i).size();
			id = i;
		}
	}

	LOGLN("Draw filled contour...");

	// Draw filled contour to obtain a mask with interior parts
	Mat contourMask = Mat::zeros(result.size(), CV_8UC1);
	drawContours(contourMask, contours, id, Scalar(255), -1, 8, hierarchy, 0, Point());


	// sort contour in x/y directions to easily find min/max and next
	std::vector<Point> cSortedX = contours.at(id);
	std::sort(cSortedX.begin(), cSortedX.end(), sortX);

	std::vector<Point> cSortedY = contours.at(id);
	std::sort(cSortedY.begin(), cSortedY.end(), sortY);


	int minXId = 0;
	int maxXId = (int)(cSortedX.size() - 1);

	int minYId = 0;
	int maxYId = (int)(cSortedY.size() - 1);

	Rect interiorBB;
	LOGLN("Find interior...");

	while ((minXId < maxXId) && (minYId < maxYId))
	{
		Point min(cSortedX[minXId].x, cSortedY[minYId].y);
		Point max(cSortedX[maxXId].x, cSortedY[maxYId].y);

		interiorBB = Rect(min.x, min.y, max.x - min.x, max.y - min.y);

		// out-codes: if one of them is set, the rectangle size has to be reduced at that border
		int ocTop = 0;
		int ocBottom = 0;
		int ocLeft = 0;
		int ocRight = 0;

		bool finished = checkInteriorExterior(contourMask, interiorBB, ocTop, ocBottom, ocLeft, ocRight);
		if (finished)
		{
			break;
		}

		// reduce rectangle at border if necessary
		if (ocLeft)++minXId;
		if (ocRight) --maxXId;
		if (ocTop) ++minYId;
		if (ocBottom)--maxYId;
	}

	result = result(interiorBB);
	LOGLN("cropped, total time: " << ((getTickCount() - cropp_start_time) / getTickFrequency()) << " sec");

	return;
};