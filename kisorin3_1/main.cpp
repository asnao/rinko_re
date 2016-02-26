#include "ClickableImage.h"
#include <iostream>

using namespace cv;
using namespace std;

void main()
{
	Mat input[2], points[2], output;
	Mat epipolarPoint[2];

	String inputimgString1 = "./left_before.png";
	String inputimgString2 = "./right_before.png";

	input[0] = imread(inputimgString1);
	input[1] = imread(inputimgString2);

	ClickableImage c1 = ClickableImage(inputimgString1);
	ClickableImage c2 = ClickableImage(inputimgString2);

	//c1 c2...ClickableImageオブジェクト
	//points...画像の対応点
	//epipolar...エピポーラ線用の点
	clickLoop(&c1, &c2, &points[0], &points[1], &epipolarPoint[0], &epipolarPoint[1]);

	//平面射影変換行列を取得
	Mat homography = findHomography(points[0], points[1], CV_RANSAC);

	//変換
	Mat result;
	warpPerspective(input[0], result, homography, Size(input[0].cols * 2, input[0].rows));

	//合成と表示
	input[1].copyTo(Mat(result, Rect(0, 0, input[1].cols, input[1].rows)));
	imshow("test", result);

	waitKey(0);
}