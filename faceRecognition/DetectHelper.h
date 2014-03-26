#ifndef _DETECT_HELPER_
#define _DETECT_HELPER_

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "Config.h"

using namespace cv;

class DetectHelper {
public:
	DetectHelper(){};
	~DetectHelper(){};
	void detectBothEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, CascadeClassifier &leftEyeDetector, CascadeClassifier &rightEyeDetector, Point &leftEye, Point &rightEye, Rect *searchedLeftEye, Rect *searchedRightEye);
	void detectObjectsCustom(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors);
	void detectLargestObject(const Mat &img, CascadeClassifier &cascade, Rect &largestObject, int scaledWidth = 320);
	void detectManyObjects(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth);
private:
	
};

#endif