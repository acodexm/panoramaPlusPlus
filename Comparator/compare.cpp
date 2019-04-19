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
#define TAG "Compare images "
#define ENABLE_LOG true
#define LOGLN(debug, msg) if(debug) { LOG() << TAG << msg << endl ; }

string prefix="default_";
bool debug = false;
bool preview = false;
void histScore(Mat& image1, Mat& image2);
Mat extractDiff(Mat& image1, Mat& image2, Point& matchLoc);
Match_desc matchImage(Mat& src_img, Mat& template_img);

int main(int argc, char* argv[])
{
#if ENABLE_LOG
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
	LOGLN(debug, "started...");

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

	Match_desc found = matchImage(src_img, template_img);
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
