#include "Helper.h"

void Helper::preprocess() {
	FileHelper fileHelper;
	DetectHelper detectHelper; 
	vector<string> imgFileHolder;

	CascadeClassifier lbpFaceDetector;  
	CascadeClassifier haarFaceDetector;  
	CascadeClassifier  eyeDetector;
	CascadeClassifier  eyeGlassesDetector;
	CascadeClassifier  leftEyeDetector;
	CascadeClassifier  rightEyeDetector;
	
	CascadeClassifier  harrRighteyeSplits;
	CascadeClassifier  harrlefteyeSplits;
	CascadeClassifier  leftEyeMore;
	CascadeClassifier  rightEyeMore;

	fstream detectFailImg;
	detectFailImg.open("D:\\CD2\\unprocessedImgs.txt");
	string filesPath = "D:\\CD2\\database";
	string normalizeImg = "D:\\CD2\\processedImgs\\";
	string dealedImg = "D:\\CD2\\workedImgs\\";

	try { 		 
		lbpFaceDetector.load(LBPCASCADE_FACE);  
		haarFaceDetector.load(HAARCASCADES_FACE);

		eyeDetector.load(HAARCASCADES_EYES);
		eyeGlassesDetector.load(HAARCASCADES_EYEGLASSES);

		leftEyeDetector.load(HAARCASCADES_LEFTEYE);
		rightEyeDetector.load(HAARCASCADES_RIGHTEYE);

		harrRighteyeSplits.load(HAARCASCADES_RIGHTEYE_2SPLITS);
		harrlefteyeSplits.load(HAARCASCADES_LEFTEYE_2SPLITS);
		leftEyeMore.load(HAARCASCADES_LEFTEYE_MORE);
		rightEyeMore.load(HAARCASCADES_RIGHTEYE_MORE);
	} catch (cv::Exception e) {
		cerr<<"error:couldn't load face detector (";  
		cerr<<"lbpcascade_frontalface.xml)!"<<endl;  
		exit(1); 
	}
	
	fileHelper.getImgFilesPath(filesPath, imgFileHolder, "JPG");

	cv::Mat img;
	Rect largestObject;	
	Point leftEye, rightEye;
	Rect searchedLeftEye, searchedRightEye;
	Point  invalid = Point(-1, -1);
	bool isDetect = true;
	
	for (int i = 0; i < imgFileHolder.size(); i++) {
		img = cv::imread(filesPath + "\\" + imgFileHolder[i], CV_LOAD_IMAGE_COLOR); 		
		
		detectHelper.detectLargestObject(img, lbpFaceDetector, largestObject);
		if (largestObject.width == -1 || largestObject.height == -1) {
			isDetect = false;
			detectHelper.detectLargestObject(img, haarFaceDetector, largestObject);
			if (largestObject.width != -1 && largestObject.height != -1) {
				isDetect = true;
			}
		}

		Mat warped, img_rect;
		if (isDetect) {
			img_rect = Mat(img, largestObject);
			detectHelper.detectBothEyes(img_rect, eyeDetector, eyeGlassesDetector, leftEyeDetector, rightEyeDetector, leftEye, rightEye, &searchedLeftEye, &searchedRightEye);
		
			if (leftEye == invalid || rightEye == invalid) {
				detectHelper.detectBothEyes(img_rect, eyeDetector, eyeGlassesDetector, leftEyeDetector, rightEyeDetector, leftEye, rightEye, &searchedLeftEye, &searchedRightEye);
				if (leftEye == invalid || rightEye == invalid) {
					isDetect = false;
				}
			}
			else {
				faceNomalize(img_rect, leftEye, rightEye, warped);
			}
		}
		
		if (!isDetect) {
			cout << i << ":" << imgFileHolder[i] << endl;
			//detectFailImg << imgFileHolder[i] << endl;
		}
		else {	
			//imshow("face", img_rect);
			//imshow("nomalize", warped);	
			//waitKey();
			string fileName = filesPath + "\\" + imgFileHolder[i];
			string succeedImgsName = dealedImg + imgFileHolder[i];
			MoveFile(fileName.c_str(), succeedImgsName.c_str());
			imwrite(normalizeImg + imgFileHolder[i], warped);
		}

		warped.release();		
		img_rect.release();
	}

	detectFailImg.close();
}

//仿射变换
void Helper::faceNomalize(Mat img_rect, Point leftEye, Point rightEye, Mat& warped) {
	
	Point2f eyesCenter;
	eyesCenter.x = (leftEye.x + rightEye.x) * 0.5f;
	eyesCenter.y = (leftEye.y + rightEye.y) * 0.5f;
	
	//获取两个人眼的角度
	double dy = (rightEye.y - leftEye.y);
	double dx = (rightEye.x - leftEye.x);
	double len = sqrt(dx * dx + dy * dy);

	double angle = atan2(dy, dx) * 180.0 / CV_PI;
	double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X);
	double scale = desiredLen * DESIRED_FACE_WIDTH / len;
	cout << scale << endl;
	Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);
	double ex = DESIRED_FACE_WIDTH * 0.5f - eyesCenter.x;
	double ey = DESIRED_FACE_HEIGHT * DESIRED_LEFT_EYE_Y - eyesCenter.y;
	
	rot_mat.at<double>(0, 2) += ex;
	rot_mat.at<double>(1, 2) += ey;
	warped = Mat(DESIRED_FACE_HEIGHT, DESIRED_FACE_WIDTH, CV_8U, Scalar(128));
	warpAffine(img_rect, warped, rot_mat, warped.size());
}
