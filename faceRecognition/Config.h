#ifndef _CONFIG_
#define _CONFIG_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
enum Error_code {};

const double DESIRED_LEFT_EYE_X = 0.20;     // 控制处理后人脸的多少部分是可见的
const double DESIRED_LEFT_EYE_Y = 0.21;
const double DESIRED_RIGHT_EYE_X = 1.0 - DESIRED_LEFT_EYE_X;
//得到我们想要的尺度化大小
const int DESIRED_FACE_WIDTH = 150;
const int DESIRED_FACE_HEIGHT = 150;

const string HAARCASCADES_FACE = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
const string LBPCASCADE_FACE = "C:\\opencv\\sources\\data\\lbpcascades\\lbpcascade_frontalface.xml";
const string HAARCASCADES_EYES = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
const string HAARCASCADES_EYEGLASSES = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
const string HAARCASCADES_LEFTEYE = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_lefteye.xml";
const string HAARCASCADES_RIGHTEYE = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_righteye.xml";
const string HAARCASCADES_LEFTEYE_2SPLITS = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_lefteye_2splits.xml";
const string HAARCASCADES_RIGHTEYE_2SPLITS = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_righteye_2splits.xml";
const string HAARCASCADES_LEFTEYE_MORE = "LEye.xml";
const string HAARCASCADES_RIGHTEYE_MORE = "REye.xml";

	//跳过人脸边界，因为它们经常是头发和耳朵，这不是我们关心的 
	
	// For "2splits.xml": Finds both eyes in roughly 60% of detected faces, also detects closed eyes.
	const float EYE_SX = 0.12f;
	const float EYE_SY = 0.17f;
	const float EYE_SW = 0.37f;
	const float EYE_SH = 0.36f;
	
	
	/*
	// For mcs.xml: Finds both eyes in roughly 80% of detected faces, also detects closed eyes.
	const float EYE_SX = 0.10f;
	const float EYE_SY = 0.19f;
	const float EYE_SW = 0.40f;
	const float EYE_SH = 0.36f;
	*/

	/*
	// For default eye.xml or eyeglasses.xml: Finds both eyes in roughly 40% of detected faces, but does not detect closed eyes.
	//haarcascade_eye.xml检测器在由下面确定的人脸区域内搜索最优。
	const float EYE_SX = 0.16f;//x
	const float EYE_SY = 0.26f;//y
	const float EYE_SW = 0.30f;//width
	const float EYE_SH = 0.28f;//height
	*/
#endif