#include "stdafx.h"
#include "../Comparator/logger.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include "stitch.h"
#include "cropp.h"
#include "opencv2/opencv.hpp"

#define ENABLE_LOG true
#define TAG "simple stitcher "
#define LOGLN(debug, msg) if(debug) { LOG() << TAG << msg << endl ; }



using namespace std;
using namespace cv;
using namespace cv::detail;
namespace fs = std::filesystem;
namespace {
	vector<String> _imagesPath;
	vector<Mat> imgVec;
	int _nbImages;
	bool compressed = false;
	static string _resultPath = "simpleResult.jpg";
}

void cropp(Mat& result);

int simpleStitch(int argc, char** argv) {
#ifdef ENABLE_LOG
	int64 app_start_time = getTickCount();
#endif
	if (argc == 1)
	{
		LOGLN(debug, "not enaugh arguments");
		return -1;
	}
	LOGLN(debug, "loading...");
	for (int i = 1; i < argc; ++i) {
		
		if (string(argv[i]) == "--compressed") {
			compressed = true;
		}
		else if (string(argv[i]).find("--") != string::npos) {
			continue;
		}
		else
		{
			struct stat info;

			char* pathname = argv[i];
			if (stat(pathname, &info) != 0)
			{
				printf("cannot access %s\n", pathname);
				continue;
			}
			else if (info.st_mode & S_IFDIR)
				printf("%s is a directory\n", pathname);
			else
			{
				printf("%s is no directory\n", pathname);
				continue;
			}
			for (const auto& entry : fs::directory_iterator(pathname))
			{
				cout << entry.path() << endl;
				_imagesPath.push_back(entry.path().u8string());
			}
		}
	}
	_nbImages = static_cast<int>(_imagesPath.size());
	if (_nbImages < 2)
	{
		LOGLN(debug,  "Not enaugh images...");
		return -1;
	}
	
	LOGLN(debug, "loading and compressing...");
	for (int k = 0; k < _nbImages; k++) {
		// Get the image
		Mat curimage = imread(_imagesPath[k]);
		Mat newimage;
		// Convert to a 3 channel Mat to use with Stitcher module
		cvtColor(curimage, newimage, CV_BGRA2RGB);
		// Reduce the resolution for fast computation
		if (compressed) {
			float scale = 1000.0f / curimage.cols;
			resize(newimage, newimage,
				Size((int)(scale * curimage.cols), (int)(scale * curimage.rows)));
		}
		imgVec.push_back(newimage);
	};
	LOGLN(debug, "loaded and compressed, total time: " << ((getTickCount() - app_start_time) / getTickFrequency()) << " sec");
	
	LOGLN(debug, "stitching...");
	Mat result;
	Stitcher::Mode mode = Stitcher::PANORAMA;
	Ptr<Stitcher> stitcher = Stitcher::create(mode);
	Stitcher::Status status = stitcher->stitch(imgVec, result);
	LOGLN(debug, "stitched, total time: " << ((getTickCount() - app_start_time) / getTickFrequency()) << " sec");

	LOGLN(debug, "cropping...");
	cropp(result);
	LOGLN(debug, "saving...");
	imwrite(_resultPath, result);

	LOGLN(debug, "Finished, total time: " << ((getTickCount() - app_start_time) / getTickFrequency()) << " sec");
	return status;
}
