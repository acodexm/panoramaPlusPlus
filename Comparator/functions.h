#include <opencv2/opencv.hpp>
#include <string>
using namespace cv;
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
template<typename T>
void histScore(Mat& image1,Mat& image2);
Mat extractDiff(Mat& image1, Mat& image2, Point& matchLoc);
extern std::string prefix;
extern bool preview;
extern bool debug;
#endif