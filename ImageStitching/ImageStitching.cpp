#include "stdafx.h"
#include "stitch.h"
#include "../Comparator/logger.h"
#include <iostream>
#include <string>

#define TAG "ImageStitching... "
#define ENABLE_LOG true
#define LOGLN(debug, msg) if(debug) { LOG() << TAG << msg << endl ; }

using namespace std;

int advancedStitch(int argc, char** argv);
int stitch(int argc, char** argv);
int simpleStitch(int argc, char** argv);
bool debug = false;
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
		else if (string(argv[i]) == "--debug") {
			debug = true;
			remove(argv[i]);
		}
	}
	LOGLN(debug, mode);
	switch (resolveOption(mode)) {
	case Advanced:
		return advancedStitch(argc, argv);
	case Custom:
		return stitch(argc, argv);
	default:
		return simpleStitch(argc, argv);
	}
};