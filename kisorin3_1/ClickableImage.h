#ifndef INCLUDED_CLICKABLEIMAGE_H
#define INCLUDED_CLICKABLEIMAGE_H

//OpenCVのlibファイルを結合するヘッダファイル名を入れてください
#include <opencv2/opencv.hpp>

#include <vector>

namespace cv {
	class Mat;
}

class CorrespondingPoints;

class ClickableImage {
public:
	ClickableImage(const char* filename);
	ClickableImage(const cv::String& filename);
	~ClickableImage();

	int maxIndex() const;
	cv::Mat* image() const;

	const char* filename() const;
	cv::Point getPoint(int index) const;
	cv::Point epipolarPoint() const;

	void reset();
	void resetImage();

	void undo();
	void update();

	void setEpipolarPoint(const cv::Point& p);
	void setCorrespondingPoint(const cv::Point& p);

private:
	static void clickFunc(int event, int x, int y, int flags, void *param);

	int mMaxIndex;
	cv::Mat* mImage;

	const char* mFileName;
	std::vector<cv::Point> mPoints;
	cv::Point mEpipolarPoint;

};

void clickLoop(
	ClickableImage* inputA,
	ClickableImage* inputB,
	cv::Mat* outputA,
	cv::Mat* outputB,
	cv::Mat* epipolarA = 0,
	cv::Mat* epipolarB = 0);

#endif