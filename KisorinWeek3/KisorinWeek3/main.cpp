#include "ClickableImage.h"

using namespace cv;

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
}