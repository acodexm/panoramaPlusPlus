#include <opencv2/opencv.hpp>
#include <string>
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
// Contains the description of the match
typedef struct Match_desc {
	cv::Mat resized;
	bool init;
	double maxVal;
	cv::Point maxLoc;
	double scale;
	Match_desc() : init(0) {}
} Match_desc;
template<typename T>
void histScore(cv::Mat& image1, cv::Mat& image2);
cv::Mat extractDiff(cv::Mat& image1, cv::Mat& image2, cv::Point& matchLoc);
Match_desc matchImage(cv::Mat& image1, cv::Mat& image2);
extern std::string prefix;
extern bool preview;
extern bool debug;
#endif