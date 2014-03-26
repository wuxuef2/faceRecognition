#ifndef _HELPER_
#define _HELPER_

#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include "Config.h"

class Helper {
public:
	Helper(){};
	~Helper(){};

	void outputEysPosition(string filesPath);
	void getOpenCvLibFilesName(string openCvPath);
	void getEyesPosition(string filesPath, string haarcascade, vector<cv::Point>& detectResult);
	void deleteFailImg();
	void detect(string filesPath, string haarcascade, vector<cv::Rect>& detectResult);

private:
	void findFIels(vector<string>&, string);
};


void Helper::detect(string filesPath, string haarcascade, vector<cv::Rect>& detectResult) {
	
	cv::CascadeClassifier cascade = cv::CascadeClassifier::CascadeClassifier(haarcascade);  

	// 载入图像
	cv::Mat SrcImage = cv::imread(filesPath, CV_LOAD_IMAGE_COLOR);     
    cv::Mat GrayImage; 
  
    cvtColor(SrcImage, GrayImage, CV_BGR2GRAY);
	
	if (!cascade.empty()) {      
        // 识别  
        cascade.detectMultiScale(GrayImage, detectResult);  
    }  
		
	GrayImage.release();
	SrcImage.release();
}

void Helper::outputEysPosition(string filesPath) {
	ofstream eyesPositionOutput;
	bool isWorked;

	string eyesPositionOutputPath = "C:\\Users\\wuxf\\Documents\\Visual Studio 2012\\Projects\\faceRecognition\\faceRecognition\\eyesPositionOutput.txt";
	eyesPositionOutput.open(eyesPositionOutputPath, ios::app);

	vector<string> imgsNameHolder;
	vector<cv::Rect> detectResult;	

	string succeedPath = "D:\\CD2\\workedImgs";
	string haarcascade = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye.xml";
	string lefteyeHaarcascade = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_lefteye.xml";
	string rightHaarcascade = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_mcs_righteye.xml";

	eyesPositionOutput << '\n' << '\n';

	findFIels(imgsNameHolder, filesPath);

	for (int i = 0; i < imgsNameHolder.size(); i++) {
		if (imgsNameHolder[i].find("JPG") < imgsNameHolder[i].length()) {
			isWorked = false;
			string fileName = filesPath + "\\" + imgsNameHolder[i];
			string succeedImgsName = succeedPath + "\\" + imgsNameHolder[i];
			cv::Point leftEye, rightEye;

			detectResult.clear();
			detect(fileName, haarcascade, detectResult);
			if (detectResult.size() == 2) {
				MoveFile(fileName.c_str(), succeedImgsName.c_str());

				leftEye.x = cvRound((detectResult[0].x + detectResult[0].width * 0.5)); 
				leftEye.y = cvRound((detectResult[0].y + detectResult[0].height * 0.5));
				rightEye.x = cvRound((detectResult[1].x + detectResult[1].width * 0.5)); 
				rightEye.y = cvRound((detectResult[1].y + detectResult[1].height * 0.5));
				

				if (leftEye.x > rightEye.x) {
					cv::Point tmp;
					tmp = leftEye;
					leftEye = rightEye;
					rightEye = tmp;
				}
				eyesPositionOutput << imgsNameHolder[i] << ' ' << leftEye.x << ' ' << leftEye.y 
					<< ' ' << rightEye.x << ' ' << rightEye.y << endl;
				cout << imgsNameHolder[i] << ' ' << leftEye.x << ' ' << leftEye.y 
					<< ' ' << rightEye.x << ' ' << rightEye.y << endl;
				isWorked = true;
			}

			if (!isWorked) {
				detectResult.clear();
				detect(fileName, lefteyeHaarcascade, detectResult);
				if (detectResult.size() == 1) {
					leftEye.x = cvRound((detectResult[0].x + detectResult[0].width * 0.5)); 
					leftEye.y = cvRound((detectResult[0].y + detectResult[0].height * 0.5));

					detectResult.clear();
					detect(fileName, rightHaarcascade, detectResult);
					if (detectResult.size() == 1) {
						MoveFile(fileName.c_str(), succeedImgsName.c_str());

						rightEye.x = cvRound((detectResult[0].x + detectResult[0].width * 0.5)); 
						rightEye.y = cvRound((detectResult[0].y + detectResult[0].height * 0.5));
						if (leftEye.x > rightEye.x) {
							cv::Point tmp;
							tmp = leftEye;
							leftEye = rightEye;
							rightEye = tmp;
						}

						eyesPositionOutput << imgsNameHolder[i] << ' ' << leftEye.x << ' ' << leftEye.y 
							<< ' ' << rightEye.x << ' ' << rightEye.y << endl;
						
						cout << imgsNameHolder[i] << ' ' << leftEye.x << ' ' << leftEye.y 
							<< ' ' << rightEye.x << ' ' << rightEye.y << endl;
						isWorked = true;
					}
				}
				if (!isWorked)
					cout << i << ": " << imgsNameHolder[i] << endl;
			}
		}
	}

	eyesPositionOutput.close();

}

void Helper::deleteFailImg() {
	vector<string> imgsNameHolder;
	string filesPath = "D:\\CD2\\Album2";
	string failImgsPath = "D:\\CD2\\invalidImgs";
	string haarcascade = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	vector<cv::Rect> detectResult;

	
	findFIels(imgsNameHolder, filesPath);

	for (int i = 34000; i < imgsNameHolder.size(); i++) {
		if (imgsNameHolder[i].find("JPG") < imgsNameHolder[i].length()) {
			detectResult.clear();
			string fileName = filesPath + "\\" + imgsNameHolder[i];
			detect(fileName, haarcascade, detectResult);

			if (detectResult.size() == 0) {
				string failImgsName = failImgsPath + "\\" + imgsNameHolder[i];
				MoveFile(fileName.c_str(), failImgsName.c_str());
			}
			
			cout << i << ": " << imgsNameHolder[i] << endl;
		}
	}
}

void Helper::findFIels(vector<string>& sFileNames,string sPath) {

    WIN32_FIND_DATAA wfd;
    sPath = sPath + "\\*.*";//查找指定目录下的所有格式的文件。"*.xml":表示查找指定目录册xml文件

    HANDLE hFile = FindFirstFile(sPath.c_str(),&wfd);

    if(INVALID_HANDLE_VALUE == hFile) {
		return;
    }

	do {
		sFileNames.push_back(wfd.cFileName);
    } while(FindNextFile(hFile, &wfd));

}

void Helper::getOpenCvLibFilesName(string openCvPath) {
	vector<string> openCvFilesNameHolder;
	ofstream outfile;

	string outFilePath = "D:\\output.txt";
	string debugFileName = "";
	string releaseFileName = "";

	outfile.open(outFilePath);

	if (openCvPath == "") {
		openCvPath = "C:\\opencv";
	}

	openCvPath += "\\build\\x86\\vc11\\lib";
	findFIels(openCvFilesNameHolder, openCvPath);

	for (int i = 0; i < openCvFilesNameHolder.size(); i++) {
		if (openCvFilesNameHolder[i].find(".lib") < openCvFilesNameHolder[i].length()) {
			if (openCvFilesNameHolder[i].find("d.lib") < openCvFilesNameHolder[i].length()) {
				debugFileName += openCvFilesNameHolder[i] + '\n';
			}
			else {
				releaseFileName += openCvFilesNameHolder[i] + '\n';
			}
		}
	}

	outfile << debugFileName << "\n\n" << releaseFileName << endl;
}

#endif