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

	//c1 c2...ClickableImage�I�u�W�F�N�g
	//points...�摜�̑Ή��_
	//epipolar...�G�s�|�[�����p�̓_
	clickLoop(&c1, &c2, &points[0], &points[1], &epipolarPoint[0], &epipolarPoint[1]);
}