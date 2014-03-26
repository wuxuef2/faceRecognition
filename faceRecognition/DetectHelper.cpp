#include "DetectHelper.h"

/*
1、采用给出的参数在图像中寻找目标，例如人脸
2、可以使用Haar级联器或者LBP级联器做人脸检测，或者甚至眼睛，鼻子，汽车检测
3、为了使检测更快，输入图像暂时被缩小到'scaledWidth'，因为寻找人脸200的尺度已经足够了。
*/
void DetectHelper::detectObjectsCustom(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors)
{

	//如果输入的图像不是灰度图像,那么将BRG或者BGRA彩色图像转换为灰度图像
	Mat gray;
	if (img.channels() == 3) {
		cvtColor(img, gray, CV_BGR2GRAY);
	}
	else if (img.channels() == 4) {
		cvtColor(img, gray, CV_BGRA2GRAY);
	}
	else {
		// 直接使用输入图像，既然它已经是灰度图像
		gray = img;
	}

	// 可能的缩小图像，是检索更快
	Mat inputImg;

	float scale = img.cols / (float)scaledWidth;
	if (img.cols > scaledWidth) {
		// 缩小图像并保持同样的宽高比
		int scaledHeight = cvRound(img.rows / scale);
		resize(gray, inputImg, Size(scaledWidth, scaledHeight));
	}
	else {
		// 直接使用输入图像，既然它已经小了
		inputImg = gray;
	}

	//标准化亮度和对比度来改善暗的图像
	Mat equalizedImg;
	equalizeHist(inputImg, equalizedImg);

	// 在小的灰色图像中检索目标
	cascade.detectMultiScale(equalizedImg, objects, searchScaleFactor, minNeighbors, flags, minFeatureSize);

	// 如果图像在检测之前暂时的被缩小了，则放大结果图像
	if (img.cols > scaledWidth) {
		for (int i = 0; i < (int)objects.size(); i++ ) {
			objects[i].x = cvRound(objects[i].x * scale);
			objects[i].y = cvRound(objects[i].y * scale);
			objects[i].width = cvRound(objects[i].width * scale);
			objects[i].height = cvRound(objects[i].height * scale);
		}
	}

	//确保目标全部在图像内部，以防它在边界上 
	for (int i = 0; i < (int)objects.size(); i++ ) {
		if (objects[i].x < 0)
			objects[i].x = 0;
		if (objects[i].y < 0)
			objects[i].y = 0;
		if (objects[i].x + objects[i].width > img.cols)
			objects[i].x = img.cols - objects[i].width;
		if (objects[i].y + objects[i].height > img.rows)
			objects[i].y = img.rows - objects[i].height;
	}

	gray.release();
	equalizedImg.release();
	inputImg.release();
	// 返回检测到的人脸矩形，存储在objects中
}

/*
1、仅寻找图像中的单个目标，例如最大的人脸，存储结果到largestObject
2、可以使用Haar级联器或者LBP级联器做人脸检测，或者甚至眼睛，鼻子，汽车检测
3、为了使检测更快，输入图像暂时被缩小到'scaledWidth'，因为寻找人脸200的尺度已经足够了。
4、注释：detectLargestObject()要比 detectManyObjects()快。
*/
void DetectHelper::detectLargestObject(const Mat &img, CascadeClassifier &cascade, Rect &largestObject, int scaledWidth)
{
	//仅寻找一个目标 (图像中最大的).
	int flags = CV_HAAR_FIND_BIGGEST_OBJECT;// | CASCADE_DO_ROUGH_SEARCH;
	// 最小的目标大小.
	Size minFeatureSize = Size(20, 20);
	// 寻找细节,尺度因子,必须比1大
	float searchScaleFactor = 1.1f;
	// 多少检测结果应当被滤掉，这依赖于你的检测系统是多坏,如果minNeighbors=2 ，大量的good or bad 被检测到。如果
	// minNeighbors=6，意味着只good检测结果，但是一些将漏掉。即可靠性 VS  检测人脸数量
	int minNeighbors = 4;

	// 执行目标或者人脸检测，仅寻找一个目标（图像中最大的）
	vector<Rect> objects;
	detectObjectsCustom(img, cascade, objects, scaledWidth, flags, minFeatureSize, searchScaleFactor, minNeighbors);
	if (objects.size() > 0) {
		// 返回仅检测到的目标
		largestObject = (Rect)objects.at(0);
	}
	else {
		// 返回一个无效的矩阵
		largestObject = Rect(-1,-1,-1,-1);
	}
}

void DetectHelper::detectManyObjects(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth)
{
	// 寻找图像中的许多目标
	int flags = CV_HAAR_SCALE_IMAGE;

	// 最小的目标大小.
	Size minFeatureSize = Size(20, 20);
	//  寻找细节,尺度因子,必须比1大
	float searchScaleFactor = 1.1f;
	// 多少检测结果应当被滤掉，这依赖于你的检测系统是多坏,如果minNeighbors=2 ，大量的good or bad 被检测到。如果
	// minNeighbors=6，意味着只good检测结果，但是一些将漏掉。即可靠性 VS  检测人脸数量
	int minNeighbors = 4;

	// 执行目标或者人脸检测，寻找图像中的许多目标
	detectObjectsCustom(img, cascade, objects, scaledWidth, flags, minFeatureSize, searchScaleFactor, minNeighbors);
}

/*
1、在给出的人脸图像中寻找双眼，返回左眼和右眼的中心，如果当找不到人眼时,或者设置为Point(-1,-1)
2、注意如果你想用两个不同的级联器寻找人眼，你可以传递第二个人眼检测器，例如如果你使用的一个常规人眼检测器和带眼镜的人眼检测器一样好，或者左眼检测器和右眼检测器一样好，
或者如果你不想第二个检测器，仅传一个未初始化级联检测器。
3、如果需要的话，也可以存储检测到的左眼和右眼的区域
*/
void DetectHelper::detectBothEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, 
								  CascadeClassifier &leftEyeDetector, CascadeClassifier &rightEyeDetector, Point &leftEye, 
								  Point &rightEye, Rect *searchedLeftEye, Rect *searchedRightEye)
{

	int leftX = cvRound(face.cols * EYE_SX);
	int topY = cvRound(face.rows * EYE_SY);
	int widthX = cvRound(face.cols * EYE_SW);
	int heightY = cvRound(face.rows * EYE_SH);
	int rightX = cvRound(face.cols * (1.0-EYE_SX-EYE_SW) );  // 右眼的开始区域

	Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));
	Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));
	Rect leftEyeRect, rightEyeRect;

	// 如果需要的话，然后搜索到的窗口给调用者
	if (searchedLeftEye)
		*searchedLeftEye = Rect(leftX, topY, widthX, heightY);
	if (searchedRightEye)
		*searchedRightEye = Rect(rightX, topY, widthX, heightY);

	// 寻找左区域，然后右区域使用第一个人眼检测器
	detectLargestObject(topLeftOfFace, eyeCascade1, leftEyeRect, topLeftOfFace.cols);
	detectLargestObject(topRightOfFace, eyeCascade1, rightEyeRect, topRightOfFace.cols);

	// 如果人眼没有检测到，尝试另外一个不同的级联检测器
	if (leftEyeRect.width <= 0 && !eyeCascade2.empty()) {
		detectLargestObject(topLeftOfFace, eyeCascade2, leftEyeRect, topLeftOfFace.cols);
		if (leftEyeRect.width <= 0) {
			detectLargestObject(topLeftOfFace, leftEyeDetector, leftEyeRect, topLeftOfFace.cols);
		}
	}

	// 如果人眼没有检测到，尝试另外一个不同的级联检测器
	if (rightEyeRect.width <= 0 && !eyeCascade2.empty()) {
		detectLargestObject(topRightOfFace, eyeCascade2, rightEyeRect, topRightOfFace.cols);
		if (rightEyeRect.width <= 0)
		    detectLargestObject(topRightOfFace, rightEyeDetector, rightEyeRect, topRightOfFace.cols);
		
	}

	if (leftEyeRect.width > 0) {   // 检查眼是否被检测到
		leftEyeRect.x += leftX;    //矫正左眼矩形，因为人脸边界被去除掉了 
		leftEyeRect.y += topY;
		leftEye = Point(leftEyeRect.x + leftEyeRect.width/2, leftEyeRect.y + leftEyeRect.height/2);
	}
	else {
		leftEye = Point(-1, -1);    // 返回一个无效的点
	}

	if (rightEyeRect.width > 0) { //检查眼是否被检测到
		rightEyeRect.x += rightX; // 矫正左眼矩形，因为它从图像的右边界开始
		rightEyeRect.y += topY;  // 矫正右眼矩形，因为人脸边界被去除掉了
		rightEye = Point(rightEyeRect.x + rightEyeRect.width/2, rightEyeRect.y + rightEyeRect.height/2);
	}
	else {
		rightEye = Point(-1, -1);    // 返回一个无效的点
	}
}


