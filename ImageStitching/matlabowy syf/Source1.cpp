//// ImageStitching.cpp : Defines the entry point for the console application.
////
//
#include "stdafx.h"
//#include<opencv2/opencv.hpp>
//#include<opencv2/stitching.hpp>
//using namespace std;
//using namespace cv;
//int main()
//{
//	vector< Mat > vImg;
//
//	Mat rImg;
//
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\23.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\24.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\25.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\26.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\34.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\35.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\36.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\37.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\45.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\46.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\47.png"));
//	vImg.push_back(imread("C:\\Users\\Adam\\Desktop\\acodexm\\Android\\Study\\OpenCVImageStitching\\OpenCVImageStitching\\pictures\\48.png"));
//	int imSize = vImg.size();
//	vector<Mat> imgVec;
//	for (int k = 0; k < imSize; k++) {
//		// Get the image
//
//		Mat newimage;
//		// Convert to a 3 channel Mat to use with Stitcher module
//		//cvtColor(vImg[k], newimage, CV_BGRA2RGB);
//		// Reduce the resolution for fast computation
//		float scale = 1000.0f / vImg[k].cols;
//		resize(vImg[k], newimage, Size((int)(scale * vImg[k].cols),
//			(int)(scale * vImg[k].rows)));
//		imgVec.push_back(newimage);
//	}
//
//	Stitcher::Mode mode = Stitcher::PANORAMA;
//	Ptr<Stitcher> stitcher = Stitcher::create(mode, false);
//
//
//	unsigned long AAtime = 0, BBtime = 0;
//	AAtime = getTickCount();
//
//	//stitcher.stitch(vImg, vvRect, rImg);
//	Stitcher::Status status = stitcher->stitch(imgVec, rImg);
//	if (status != Stitcher::OK) {
//		printf("Can't stitch images, error code = %d", int(status));
//	}
//	else {
//		printf("Success code = %d", int(status));
//	}
//	BBtime = getTickCount();
//	printf("ProcessingTime %.2lf sec \n", (BBtime - AAtime) / getTickFrequency());
//
//	imwrite("output.jpg", rImg);
//
//	waitKey(0);
//	return 0;
//}
//
