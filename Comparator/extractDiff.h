#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
#ifndef HISTOGRAM_H
#define HISTOGRAM_H
template<typename T>
Mat extractDiff(Mat& image1, Mat& image2, Point& matchLoc);
#endif