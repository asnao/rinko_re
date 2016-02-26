#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include "Labeling.h"

using namespace cv;

cv::RNG rnd(1192);
cv::Scalar randomColor()
{
	return cv::Scalar(rnd.next() & 0xFF, rnd.next() & 0xFF, rnd.next() & 0xFF);
}
 

int main(){

	const char th = 58;
	//const char th = 57;
	//Mat�^�ϐ��̐錾
	Mat inImg, bgImg, diffClrImg, diffGryImg, biBgImg, outImg;
	//�摜�̓ǂݍ���
	inImg = imread("input6.jpg");
	bgImg = imread("background6.jpg");

	imshow("in", inImg);
	imshow("bg", bgImg);
	waitKey(10);


	absdiff(inImg,bgImg,diffClrImg);//�w�i�摜�Ƃ̍������擾
	cv::imwrite("diff.jpg", diffClrImg);	

    cvtColor(diffClrImg,diffGryImg,CV_BGR2GRAY);//�����摜���O���C�X�P�[����
    threshold(diffGryImg, biBgImg, th, 255, THRESH_BINARY);//��l���摜��
	
		
	biBgImg = ~biBgImg;
//	cv::dilate(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 1);
//	cv::erode(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 1);
	cv::imwrite("input0.jpg", biBgImg);	


	waitKey(10);
/*
	cv::dilate(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 3);
	cv::imwrite("afeterdilate1.jpg", biBgImg);
	waitKey(10);
	cv::erode(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 3);
	cv::imwrite("aftererode2.jpg", biBgImg);
	waitKey(10);

	cv::erode(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 7);
	cv::imwrite("aftererode3.jpg", biBgImg);
	waitKey(10);
	cv::dilate(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 7);
	cv::imwrite("afeterdilate4.jpg", biBgImg);
	waitKey(10);
*/	
	cv::erode(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 7);
	cv::imwrite("aftererode3.jpg", biBgImg);
	waitKey(10);
	cv::dilate(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 7);
	cv::imwrite("afeterdilate4.jpg", biBgImg);
	waitKey(10);

	cv::dilate(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 14);
	cv::imwrite("afeterdilate1.jpg", biBgImg);
	waitKey(10);
	cv::erode(biBgImg, biBgImg, cv::Mat(), cv::Point(-1,-1), 14);
	cv::imwrite("aftererode2.jpg", biBgImg);
	waitKey(10);

	biBgImg = ~biBgImg;

	Mat bi3BgImg(inImg.rows,inImg.cols, CV_8UC3);

	for(int y = 0; y < inImg.rows; y++){
		for(int x = 0; x < inImg.cols; x++){
			for(int c = 0; c < 3; c++){
				bi3BgImg.data[y * inImg.cols * 3 + x * 3 + c] = biBgImg.data[y * inImg.cols + x];
			}
		}
	}


	bitwise_and(inImg,bi3BgImg,outImg);

	//�摜�̕\��
	imshow("subtraction",outImg);
	waitKey(10);
		//�摜�̏����o��
	imwrite("subtraction.jpg",outImg);

	imshow("beforelabel", biBgImg);





	// Labeling�̌��ʂ��󂯎��
	cv::Mat label(inImg.size(), CV_16SC1);
 
	// ���x�����O�����{ �Q�l�������摜�ɑ΂��Ď��s����B
	LabelingBS	labeling;
	labeling.Exec(biBgImg.data, (short *)label.data, inImg.cols, inImg.rows, false, 0);	
	
	// ���x�����O���ʂ��o�͂���A�^�����ȏ�Ԃŏ�����
	cv::Mat outimg(inImg.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	
	// ���x�����ꂽ�̈���ЂƂ��`��
	for( int i = 0; i < labeling.GetNumOfRegions(); i++)
	{
		// ���x�����O���ʂŃC���C������B
		// ���x�����O���ʂ̗̈�𒊏o����B
		cv::Mat labelarea;
		cv::compare(label, i + 1, labelarea, CV_CMP_EQ);
		// ���o�����̈�Ƀ����_���ȐF��ݒ肵�ďo�͉摜�ɒǉ��B
		cv::Mat color(inImg.size(), CV_8UC3, randomColor());
  		color.copyTo(outimg, labelarea);
	}
	// ���͉摜�ƃ��x�����O���ʂ̉摜����ʕ\��
	imshow("label", outimg);
	cv::waitKey();
	cv::hconcat(inImg, outimg, outimg);
	// ���x�����ꂽ�o�͉摜��ۑ�
	cv::imwrite("labeled.png", outimg);



	//OpenCV�̊֐��D��������̃{�^���̓��͂�����܂ŁC�v���O�������~�߂�D
	waitKey(0);
	return 0;
}

