#include "..\ImageStitching\stdafx.h"
#include "functions.h"
#include "logger.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
using namespace std;
using namespace cv;
#define TAG "Comparator "
#define ENABLE_LOG true
#define LOGLN(debug, msg) if(debug) { LOG() << TAG << msg << endl ; }

string prefix="default";
bool debug = false;
bool preview = false;
void histScore(Mat& image1, Mat& image2);
Mat extractDiff(Mat& image1, Mat& image2, Point& matchLoc);

// Contains the description of the match
typedef struct Match_desc {
	Mat resized;
	bool init;
	double maxVal;
	Point maxLoc;
	double scale;
	Match_desc() : init(0) {}
} Match_desc;
int main(int argc, char* argv[])
{
#if ENABLE_LOG
	LOGLN(debug, "Compare images...");
	int64 app_start_time = getTickCount();
#endif
	Mat src_img, template_img, arg_img_1, arg_img_2, result_mat, debug_img;
	for (int i = 1; i < argc; ++i) {
		if (string(argv[i]) == "--prefix") {
			prefix = string(argv[i + 1]) + "_";
			remove(argv[i]);
			remove(argv[i + 1]);
		}
		else if (string(argv[i]) == "--preview") {
			preview = true;
			remove(argv[i]);
		}
		else if (string(argv[i]) == "--debug") {
			debug = true;
			remove(argv[i]);
		}
	}
	arg_img_1 = imread(argv[2], CV_LOAD_IMAGE_COLOR);
	if (arg_img_1.data == NULL) {
		LOGLN(debug, "imread() failed...\n");
		return -1;
	}
	arg_img_2 = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (arg_img_2.data == NULL) {
		LOGLN(debug, "imread() failed...\n");
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
	LOGLN(debug, prefix <<" Compare image: "<<string(argv[1])<<"to: "<<string(argv[2]));

	if (preview) {
		namedWindow("DBG", WINDOW_AUTOSIZE);
	}

	Mat template_mat = template_img.clone();

	// Find size
	int tW, tH;
	tW = template_mat.cols;
	tH = template_mat.rows;
	if (preview) {
		namedWindow("Template Image", WINDOW_AUTOSIZE);
		imshow("Template Image", template_mat);
	}
	else
	{
		imwrite(prefix + "template_mat.png", template_mat);
	}
	Mat target_img, target_resized;
	target_img = src_img.clone();
	const float SCALE_START = 1.5; // 150%
	const float SCALE_END = 0.5;   // 50%
	const int SCALE_POINTS = 20;

	Match_desc found;
	for (float scale = SCALE_START; scale >= SCALE_END; scale -= (SCALE_START - SCALE_END) / SCALE_POINTS) {
		resize(target_img, target_resized, Size(0, 0), scale, scale);// Resize

		// Break if target image becomes smaller than template
		if (tW > target_resized.cols || tH > target_resized.rows) break;

		// Match template
		Mat result;
		matchTemplate(target_resized, template_mat, result, CV_TM_CCORR_NORMED);

		double maxVal, minVal; Point maxLoc, minLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

		// If better match found
		if (found.init == false || maxVal > found.maxVal) {
			found.resized = target_resized;
			found.init = true;
			found.maxVal = maxVal;
			found.maxLoc = maxLoc;
			found.scale = scale;
		}

		// START VISUALIZATION CODE
		Mat target_clone;
		resize(target_img, target_clone, Size(0, 0), scale, scale);// Resize
		rectangle(target_clone, Point(maxLoc.x, maxLoc.y), Point(maxLoc.x + tW, maxLoc.y + tH), Scalar(0, 255, 255), 3);
		if (preview) {
			imshow("DBG", target_clone);
			waitKey(200);
		}// END VISUALIZATION CODE
	}

	int startX, startY, endX, endY;
	startX = found.maxLoc.x / found.scale;
	startY = found.maxLoc.y / found.scale;

	endX = (found.maxLoc.x + tW) / found.scale;
	endY = (found.maxLoc.y + tH) / found.scale;

	// draw a bounding box around the detected result and display the image
	rectangle(target_img, Point(startX, startY), Point(endX, endY), Scalar(0, 0, 255), 3);
	if (preview) {
		imshow("DBG", target_img);
	}
	else {
		imwrite(prefix + "target_img.png", target_img);
	}
	// extract and show diffrence between images => black image means no difference
	Mat currentImage = extractDiff(found.resized, template_img, found.maxLoc);

	// calculate histogram compare score => (0, 0.2) match (0.2, 1) no match
	histScore(currentImage, template_img);
	LOGLN(debug, "Finished, total time: " << ((getTickCount() - app_start_time) / getTickFrequency()) << " sec");
	if (preview) {
		waitKey();
	}
	return 0;
}
