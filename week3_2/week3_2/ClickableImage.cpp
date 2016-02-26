#include "main.h"
#include "ClickableImage.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

ClickableImage::ClickableImage(const char* filename) :
mMaxIndex(0),
	mImage(0),
	mFileName(filename),
	mPoints(0),
	mEpipolarPoint( Point(-1, -1) )
{
	Mat input = imread(filename);
	mImage = new Mat(input);

	namedWindow(filename);
	setMouseCallback(filename, clickFunc, this);
	mPoints = vector<cv::Point>();
	imshow(filename, *mImage);
	cout << "Image Name:" << mFileName << endl;
}


ClickableImage::ClickableImage(const cv::String& filename) :
mMaxIndex(0),
	mImage(0),
	mFileName( filename.c_str() ),
	mPoints(0),
	mEpipolarPoint( Point(-1, -1) )
{
	Mat input = imread(filename);
	mImage = new Mat(input);

	namedWindow(filename);
	setMouseCallback(filename, clickFunc, this);
	mPoints = vector<cv::Point>();
	imshow(filename, *mImage);
	cout << "Image Name:" << mFileName << endl;
}


ClickableImage::~ClickableImage()
{
	delete mImage;
	mImage = 0;
}

int ClickableImage::maxIndex() const
{
	return mMaxIndex;
}

Mat* ClickableImage::image() const
{
	return mImage;
}

const char* ClickableImage::filename() const
{
	return mFileName;
}

Point ClickableImage::getPoint(int index) const
{
	return mPoints[index];
}

Point ClickableImage::epipolarPoint() const
{
	return mEpipolarPoint;
}

void ClickableImage::resetImage()
{
	//削除
	delete mImage;
	mImage = 0;

	mImage = new Mat(imread(mFileName));
}

void ClickableImage::reset()
{
	mMaxIndex = 0;

	resetImage();

	mPoints.clear();

	imshow(mFileName, *mImage);
}

void ClickableImage::undo()
{
	if ( !mPoints.empty() ) {
		mPoints.pop_back();
		--mMaxIndex;
		cout << "undo!" << endl;
	} else {
		cout << "Can not undo!" << endl;
	}
}

void ClickableImage::update()
{
	int font = FONT_HERSHEY_PLAIN;
	double fontScale = 1.3;
	int radius = 3;
	int offset = 5;

	this->resetImage();

	Mat* image = this->image();

	ostringstream oss;

	for (int i = 0; i < mMaxIndex; ++i) {
		Point p = this->getPoint(i);
		// 点と数字の描画
		circle(*image, p, radius, Scalar(255, 255, 0), -1, CV_AA);
		oss << i;
		putText(*image, oss.str().c_str(), Point(p.x - offset, p.y - offset), font, fontScale, Scalar(255, 255, 0), 1, CV_AA);
		oss.str("");
	}

	// 点と文字の描画
	circle(*image, mEpipolarPoint, radius, Scalar(0, 255, 0), -1, CV_AA);
	oss << "R";
	putText(*image, oss.str().c_str(), Point(mEpipolarPoint.x - offset, mEpipolarPoint.y - offset), font, fontScale, Scalar(0, 255, 0), 1, CV_AA);

	imshow(mFileName, *image);
}

void ClickableImage::setEpipolarPoint(const cv::Point& p)
{
	mEpipolarPoint = p;
}

void ClickableImage::setCorrespondingPoint(const cv::Point& p)
{
	mPoints.push_back(p);
	++mMaxIndex;
}

void ClickableImage::clickFunc(int event, int mouseX, int mouseY, int flags, void *param)
{
	ClickableImage* clickImage = reinterpret_cast<ClickableImage*>(param);

	Mat* image = clickImage->image();

	int maxIndex = clickImage->maxIndex();
	const char* filename = clickImage->filename();

	ostringstream oss;

	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		if (flags & CV_EVENT_FLAG_CTRLKEY) {
			Point p = Point(mouseX, mouseY);

			clickImage->setCorrespondingPoint(p);
			cout << filename << ": (" << mouseX << ", " << mouseY << ")" << endl;
			clickImage->update();
		} else if (flags & CV_EVENT_FLAG_ALTKEY) {
			// Undo
			clickImage->undo();
			clickImage->update();
		}
		break;
	case CV_EVENT_RBUTTONDOWN:
		if (flags & CV_EVENT_FLAG_CTRLKEY) {
			// エピポーラ線用
			Point ep = Point(mouseX, mouseY);
			clickImage->setEpipolarPoint(ep);
			clickImage->update();
		}
		break;
	}
}

void clickLoop(
	ClickableImage* inputA,
	ClickableImage* inputB,
	Mat* outputA,
	Mat* outputB,
	Mat* epipolarA,
	Mat* epipolarB)
{
	while (true) {
		bool breakFlag = false;
		char c = waitKey(0);
		switch (c) {
		case 'c':
			// Clear
			inputA->reset();
			inputB->reset();
			break;
		case 'q':
			if ( inputA->maxIndex() == inputB->maxIndex() ) {
				breakFlag = true;
			} else {
				cout << "2つの画像間の対応点の数が同じではありません" << endl;
			}
			break;
		case 'p':
			cout << "画像キャプチャ...出力ファイルの名前を入力してください" << endl;
			string filenameA;
			string filenameB;

			cout << inputA->filename() << "->" ;
			cin >> filenameA;
			cout << inputB->filename() << "->" ;
			cin >> filenameB;
			imwrite(filenameA, *inputA->image());
			imwrite(filenameB, *inputB->image());
			cout << "画像キャプチャ完了" << endl;
		}
		if (breakFlag) {
			break;
		}
	}

	int size = inputA->maxIndex();

	*outputA = Mat::Mat(Size(1, size), CV_32FC2);
	*outputB = Mat::Mat(Size(1, size), CV_32FC2);

	for (int i = 0; i < size; ++i) {
		int arrayIndex = i * 2;
		//A
		outputA->at<Vec2f>(i, 0)[0] = static_cast<float>(inputA->getPoint(i).x);
		outputA->at<Vec2f>(i, 0)[1] = static_cast<float>(inputA->getPoint(i).y);
		//B
		outputB->at<Vec2f>(i, 0)[0] = static_cast<float>(inputB->getPoint(i).x);
		outputB->at<Vec2f>(i, 0)[1] = static_cast<float>(inputB->getPoint(i).y);
	}

	float pointArrayA[2];
	float pointArrayB[2];

	if (epipolarA != 0) {
		pointArrayA[0] = static_cast<float>(inputA->epipolarPoint().x);
		pointArrayA[1] = static_cast<float>(inputA->epipolarPoint().y);
		*epipolarA = Mat::Mat( Size(1, 1), CV_32FC2);
		for (int i = 0; i < 2; ++i) {
			epipolarA->at<Vec2f>(0, 0)[i] = pointArrayA[i];
		}
		cout << "Epipolar Point A = " << *epipolarA << endl;
	}

	if (epipolarB != 0) {
		pointArrayB[0] = static_cast<float>(inputB->epipolarPoint().x);
		pointArrayB[1] = static_cast<float>(inputB->epipolarPoint().y);

		*epipolarB = Mat::Mat( Size(1, 1), CV_32FC2);
		for (int i = 0; i < 2; ++i) {
			epipolarB->at<Vec2f>(0, 0)[i] = pointArrayB[i];
		}

		cout << "Epipolar Point B = " << *epipolarB << endl;
	}

	cout << "Corresponding points A = " << *outputA << endl;
	cout << "Corresponding points B = " << *outputB << endl;
}