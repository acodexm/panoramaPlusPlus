#include "stdafx.h"
#include "cropp.h"
#include "stitch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching/detail/autocalib.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/timelapsers.hpp"
#include "opencv2/stitching/detail/camera.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/warpers.hpp"
#include "opencv2/stitching/warpers.hpp"


#define ENABLE_LOG 1
#define LOG(msg) std::cout << msg
#define LOGLN(msg) std::cout << msg << std::endl

using namespace std;
using namespace cv;
using namespace cv::detail;
namespace fs = std::filesystem;

int advancedStitch(int argc, char** argv);
int stitch(int argc, char** argv);
int simpleStitch(int argc, char** argv);
enum Options {
	Advanced,
	Custom,
	Simple
};
Options resolveOption(string input) {
	if (input == "advanced") return Advanced;
	if (input == "custom") return Custom;
	return Simple;
}
int main(int argc, char** argv) {
	string mode = "simple";

	Options resolveOption(string input);
	if (argc == 1)
	{
		cout << "not enaugh arguments";
		return -1;
	}
	for (int i = 1; i < argc; ++i) {
		if (string(argv[i]) == "--advanced") {
			mode = "advanced";
			remove(argv[i]);
		}
		else if (string(argv[i]) == "--custom") {
			mode = "custom";
			remove(argv[i]);
		}
	}
	switch (resolveOption(mode)) {
	case Advanced:
		return advancedStitch(argc, argv);
	case Custom:
		return stitch(argc, argv);
	default:
		return simpleStitch(argc, argv);
	}
};