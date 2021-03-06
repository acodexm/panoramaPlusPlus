#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "functions.h"
#include <iostream>
#include "logger.h"
#include <fstream>
#include <string>
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
using namespace std;
using namespace cv;
#define TAG "Histogram compare "
#define ENABLE_LOG true
#define LOGLN(debug, msg) if(debug) { LOG() << TAG << msg << endl ; }

void histScore(Mat& image1, Mat& image2) {
#ifdef ENABLE_LOG
	int64 app_start_time = getTickCount();
	LOGLN(debug, "started... ");
#endif
	Mat hsv, hsv2;
	if (preview) {
		namedWindow("picture 1", 1);
		imshow("H-S Histogram", image1);
		namedWindow("picture 2", 2);
		imshow("H-S Histogram2", image2);
	}
	else {
		imwrite(prefix + "ref.png", image1);
		imwrite(prefix + "test.png", image2);
	}
	cvtColor(image1, hsv, COLOR_BGR2HSV);
	cvtColor(image2, hsv2, COLOR_BGR2HSV);
	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int hbins = 30, sbins = 32;
	int histSize[] = { hbins, sbins };
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	MatND hist, hist2;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = { 0, 1 };
	calcHist(&hsv, 1, channels, Mat(), // do not use mask
		hist, 2, histSize, ranges,
		true, // the histogram is uniform
		false);
	calcHist(&hsv2, 1, channels, Mat(), // do not use mask
		hist2, 2, histSize, ranges,
		true, // the histogram is uniform
		false);
	double maxVal = 0;
	double maxVal2 = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);
	minMaxLoc(hist2, 0, &maxVal2, 0, 0);
	int scale = 10;
	Mat histImg = Mat::zeros(sbins * scale, hbins * 10, CV_8UC3);
	Mat histImg2 = Mat::zeros(sbins * scale, hbins * 10, CV_8UC3);
	for (int h = 0; h < hbins; h++)
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxVal);
			rectangle(histImg, Point(h * scale, s * scale),
				Point((h + 1) * scale - 1, (s + 1) * scale - 1),
				Scalar::all(intensity),
				-1);
			float binVal2 = hist2.at<float>(h, s);
			int intensity2 = cvRound(binVal2 * 255 / maxVal2);
			rectangle(histImg2, Point(h * scale, s * scale),
				Point((h + 1) * scale - 1, (s + 1) * scale - 1),
				Scalar::all(intensity2),
				-1);
		}
	double hist_score = compareHist(hist, hist2, HISTCMP_BHATTACHARYYA);

	LOGLN(debug, "hist_score: " << hist_score);
	LOGLN(debug, "Finished, total time: " << ((getTickCount() - app_start_time) / getTickFrequency()) << " sec");

	if (preview) {
		namedWindow("H-S Histogram", 1);
		imshow("H-S Histogram", histImg);
		namedWindow("H-S Histogram2", 2);
		imshow("H-S Histogram2", histImg2);
	}
	else {
		imwrite(prefix + "histImg.png", histImg);
		imwrite(prefix + "histImg2.png", histImg2);
	}
}
