#include "DetectHelper.h"

/*
1�����ø����Ĳ�����ͼ����Ѱ��Ŀ�꣬��������
2������ʹ��Haar����������LBP��������������⣬���������۾������ӣ��������
3��Ϊ��ʹ�����죬����ͼ����ʱ����С��'scaledWidth'����ΪѰ������200�ĳ߶��Ѿ��㹻�ˡ�
*/
void DetectHelper::detectObjectsCustom(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors)
{

	//��������ͼ���ǻҶ�ͼ��,��ô��BRG����BGRA��ɫͼ��ת��Ϊ�Ҷ�ͼ��
	Mat gray;
	if (img.channels() == 3) {
		cvtColor(img, gray, CV_BGR2GRAY);
	}
	else if (img.channels() == 4) {
		cvtColor(img, gray, CV_BGRA2GRAY);
	}
	else {
		// ֱ��ʹ������ͼ�񣬼�Ȼ���Ѿ��ǻҶ�ͼ��
		gray = img;
	}

	// ���ܵ���Сͼ���Ǽ�������
	Mat inputImg;

	float scale = img.cols / (float)scaledWidth;
	if (img.cols > scaledWidth) {
		// ��Сͼ�񲢱���ͬ���Ŀ�߱�
		int scaledHeight = cvRound(img.rows / scale);
		resize(gray, inputImg, Size(scaledWidth, scaledHeight));
	}
	else {
		// ֱ��ʹ������ͼ�񣬼�Ȼ���Ѿ�С��
		inputImg = gray;
	}

	//��׼�����ȺͶԱȶ������ư���ͼ��
	Mat equalizedImg;
	equalizeHist(inputImg, equalizedImg);

	// ��С�Ļ�ɫͼ���м���Ŀ��
	cascade.detectMultiScale(equalizedImg, objects, searchScaleFactor, minNeighbors, flags, minFeatureSize);

	// ���ͼ���ڼ��֮ǰ��ʱ�ı���С�ˣ���Ŵ���ͼ��
	if (img.cols > scaledWidth) {
		for (int i = 0; i < (int)objects.size(); i++ ) {
			objects[i].x = cvRound(objects[i].x * scale);
			objects[i].y = cvRound(objects[i].y * scale);
			objects[i].width = cvRound(objects[i].width * scale);
			objects[i].height = cvRound(objects[i].height * scale);
		}
	}

	//ȷ��Ŀ��ȫ����ͼ���ڲ����Է����ڱ߽��� 
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
	// ���ؼ�⵽���������Σ��洢��objects��
}

/*
1����Ѱ��ͼ���еĵ���Ŀ�꣬���������������洢�����largestObject
2������ʹ��Haar����������LBP��������������⣬���������۾������ӣ��������
3��Ϊ��ʹ�����죬����ͼ����ʱ����С��'scaledWidth'����ΪѰ������200�ĳ߶��Ѿ��㹻�ˡ�
4��ע�ͣ�detectLargestObject()Ҫ�� detectManyObjects()�졣
*/
void DetectHelper::detectLargestObject(const Mat &img, CascadeClassifier &cascade, Rect &largestObject, int scaledWidth)
{
	//��Ѱ��һ��Ŀ�� (ͼ��������).
	int flags = CV_HAAR_FIND_BIGGEST_OBJECT;// | CASCADE_DO_ROUGH_SEARCH;
	// ��С��Ŀ���С.
	Size minFeatureSize = Size(20, 20);
	// Ѱ��ϸ��,�߶�����,�����1��
	float searchScaleFactor = 1.1f;
	// ���ټ����Ӧ�����˵�������������ļ��ϵͳ�Ƕ໵,���minNeighbors=2 ��������good or bad ����⵽�����
	// minNeighbors=6����ζ��ֻgood�����������һЩ��©�������ɿ��� VS  �����������
	int minNeighbors = 4;

	// ִ��Ŀ�����������⣬��Ѱ��һ��Ŀ�꣨ͼ�������ģ�
	vector<Rect> objects;
	detectObjectsCustom(img, cascade, objects, scaledWidth, flags, minFeatureSize, searchScaleFactor, minNeighbors);
	if (objects.size() > 0) {
		// ���ؽ���⵽��Ŀ��
		largestObject = (Rect)objects.at(0);
	}
	else {
		// ����һ����Ч�ľ���
		largestObject = Rect(-1,-1,-1,-1);
	}
}

void DetectHelper::detectManyObjects(const Mat &img, CascadeClassifier &cascade, vector<Rect> &objects, int scaledWidth)
{
	// Ѱ��ͼ���е����Ŀ��
	int flags = CV_HAAR_SCALE_IMAGE;

	// ��С��Ŀ���С.
	Size minFeatureSize = Size(20, 20);
	//  Ѱ��ϸ��,�߶�����,�����1��
	float searchScaleFactor = 1.1f;
	// ���ټ����Ӧ�����˵�������������ļ��ϵͳ�Ƕ໵,���minNeighbors=2 ��������good or bad ����⵽�����
	// minNeighbors=6����ζ��ֻgood�����������һЩ��©�������ɿ��� VS  �����������
	int minNeighbors = 4;

	// ִ��Ŀ�����������⣬Ѱ��ͼ���е����Ŀ��
	detectObjectsCustom(img, cascade, objects, scaledWidth, flags, minFeatureSize, searchScaleFactor, minNeighbors);
}

/*
1���ڸ���������ͼ����Ѱ��˫�ۣ��������ۺ����۵����ģ�������Ҳ�������ʱ,��������ΪPoint(-1,-1)
2��ע�����������������ͬ�ļ�����Ѱ�����ۣ�����Դ��ݵڶ������ۼ���������������ʹ�õ�һ���������ۼ�����ʹ��۾������ۼ����һ���ã��������ۼ���������ۼ����һ���ã�
��������㲻��ڶ��������������һ��δ��ʼ�������������
3�������Ҫ�Ļ���Ҳ���Դ洢��⵽�����ۺ����۵�����
*/
void DetectHelper::detectBothEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, 
								  CascadeClassifier &leftEyeDetector, CascadeClassifier &rightEyeDetector, Point &leftEye, 
								  Point &rightEye, Rect *searchedLeftEye, Rect *searchedRightEye)
{

	int leftX = cvRound(face.cols * EYE_SX);
	int topY = cvRound(face.rows * EYE_SY);
	int widthX = cvRound(face.cols * EYE_SW);
	int heightY = cvRound(face.rows * EYE_SH);
	int rightX = cvRound(face.cols * (1.0-EYE_SX-EYE_SW) );  // ���۵Ŀ�ʼ����

	Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));
	Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));
	Rect leftEyeRect, rightEyeRect;

	// �����Ҫ�Ļ���Ȼ���������Ĵ��ڸ�������
	if (searchedLeftEye)
		*searchedLeftEye = Rect(leftX, topY, widthX, heightY);
	if (searchedRightEye)
		*searchedRightEye = Rect(rightX, topY, widthX, heightY);

	// Ѱ��������Ȼ��������ʹ�õ�һ�����ۼ����
	detectLargestObject(topLeftOfFace, eyeCascade1, leftEyeRect, topLeftOfFace.cols);
	detectLargestObject(topRightOfFace, eyeCascade1, rightEyeRect, topRightOfFace.cols);

	// �������û�м�⵽����������һ����ͬ�ļ��������
	if (leftEyeRect.width <= 0 && !eyeCascade2.empty()) {
		detectLargestObject(topLeftOfFace, eyeCascade2, leftEyeRect, topLeftOfFace.cols);
		if (leftEyeRect.width <= 0) {
			detectLargestObject(topLeftOfFace, leftEyeDetector, leftEyeRect, topLeftOfFace.cols);
		}
	}

	// �������û�м�⵽����������һ����ͬ�ļ��������
	if (rightEyeRect.width <= 0 && !eyeCascade2.empty()) {
		detectLargestObject(topRightOfFace, eyeCascade2, rightEyeRect, topRightOfFace.cols);
		if (rightEyeRect.width <= 0)
		    detectLargestObject(topRightOfFace, rightEyeDetector, rightEyeRect, topRightOfFace.cols);
		
	}

	if (leftEyeRect.width > 0) {   // ������Ƿ񱻼�⵽
		leftEyeRect.x += leftX;    //�������۾��Σ���Ϊ�����߽类ȥ������ 
		leftEyeRect.y += topY;
		leftEye = Point(leftEyeRect.x + leftEyeRect.width/2, leftEyeRect.y + leftEyeRect.height/2);
	}
	else {
		leftEye = Point(-1, -1);    // ����һ����Ч�ĵ�
	}

	if (rightEyeRect.width > 0) { //������Ƿ񱻼�⵽
		rightEyeRect.x += rightX; // �������۾��Σ���Ϊ����ͼ����ұ߽翪ʼ
		rightEyeRect.y += topY;  // �������۾��Σ���Ϊ�����߽类ȥ������
		rightEye = Point(rightEyeRect.x + rightEyeRect.width/2, rightEyeRect.y + rightEyeRect.height/2);
	}
	else {
		rightEye = Point(-1, -1);    // ����һ����Ч�ĵ�
	}
}


