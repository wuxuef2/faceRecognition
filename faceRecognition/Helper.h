#ifndef _HELPER_
#define _HELPER_

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "Config.h"
#include "FileHelper.h"
#include "DetectHelper.h"

class Helper {
public:
	Helper(){};
	~Helper(){};

	void preprocess();
	//void test();
	void faceNomalize(Mat img_rect, Point leftEye, Point rightEye, Mat& warped);
private:
};

#endif