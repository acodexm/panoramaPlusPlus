#include "stdafx.h"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <opencv2/opencv.hpp>

int main(int argc, char**argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <Image_Path>\n";
		return -1;
	}
	cv::Mat origSource = cv::imread(argv[1], 1);

	if (!origSource.data) {
		std::cerr << "Can't read image";
		return -1;
	}

	// this will select a subsection of the original source image - WITHOUT copying the data
	// (the header will point to a region of interest, adjusting data pointers and row step sizes)
	cv::Mat sourceMat = origSource(cv::Range(origSource.size[0] / 4, (3 * origSource.size[0]) / 4), cv::Range(origSource.size[1] / 4, (3 * origSource.size[1]) / 4));

	// correctly copy the contents of an N dimensional cv::Mat
	// works just as fast as copying a 2D mat, but has much more difficult to read code :)
	// see http://stackoverflow.com/questions/18882242/how-do-i-get-the-size-of-a-multi-dimensional-cvmat-mat-or-matnd
	// copy this code in your own cvMat_To_Char_Array() function which really OpenCV should provide somehow...
	// keep in mind that even Mat::clone() aligns each row at a 4 byte boundary, so uneven sized images always have stepgaps
	size_t totalsize = sourceMat.step[sourceMat.dims - 1];
	const size_t rowsize = sourceMat.step[sourceMat.dims - 1] * sourceMat.size[sourceMat.dims - 1];
	size_t coordinates[sourceMat.dims - 1] = { 0 };
	std::cout << "Image dimensions: ";
	for (int t = 0; t<sourceMat.dims; t++)
	{
		// calculate total size of multi dimensional matrix by multiplying dimensions
		totalsize *= sourceMat.size[t];
		std::cout << (t>0 ? " X " : "") << sourceMat.size[t];
	}
	// Allocate destination image buffer
	uint8_t * imagebuffer = new uint8_t[totalsize];
	size_t srcptr = 0, dptr = 0;
	std::cout << std::endl;
	std::cout << "One pixel in image has " << sourceMat.step[sourceMat.dims - 1] << " bytes" << std::endl;
	std::cout << "Copying data in blocks of " << rowsize << " bytes" << std::endl;
	std::cout << "Total size is " << totalsize << " bytes" << std::endl;
	while (dptr<totalsize) {
		// we copy entire rows at once, so lowest iterator is always [dims-2]
		// this is legal since OpenCV does not use 1 dimensional matrices internally (a 1D matrix is a 2d matrix with only 1 row)
		std::memcpy(&imagebuffer[dptr], &(((uint8_t*)sourceMat.data)[srcptr]), rowsize);
		// destination matrix has no gaps so rows follow each other directly
		dptr += rowsize;
		// src matrix can have gaps so we need to calculate the address of the start of the next row the hard way
		// see *brief* text in opencv2/core/mat.hpp for address calculation
		coordinates[sourceMat.dims - 2]++;
		srcptr = 0;
		for (int t = sourceMat.dims - 2; t >= 0; t--) {
			if (coordinates[t] >= sourceMat.size[t]) {
				if (t == 0) break;
				coordinates[t] = 0;
				coordinates[t - 1]++;
			}
			srcptr += sourceMat.step[t] * coordinates[t];
		}
	}

	// this constructor assumes that imagebuffer is gap-less (if not, a complete array of step sizes must be given, too)
	cv::Mat destination = cv::Mat(sourceMat.dims, sourceMat.size, sourceMat.type(), (void*)imagebuffer);

	// and just to proof that sourceImage points to the same memory as origSource, we strike it through
	cv::line(sourceMat, cv::Point(0, 0), cv::Point(sourceMat.size[1], sourceMat.size[0]), CV_RGB(255, 0, 0), 3);

	cv::imshow("original image", origSource);
	cv::imshow("partial image", sourceMat);
	cv::imshow("copied image", destination);
	while (cv::waitKey(60) != 'q');
}