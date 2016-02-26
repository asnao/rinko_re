#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

using namespace cv;


void filterGaussian();
void filterBilateral();
void filterMedian();


int main(){

	filterGaussian();
	filterBilateral();
	filterMedian();

	//OpenCV�̊֐��D��������̃{�^���̓��͂�����܂ŁC�v���O�������~�߂�D
	waitKey(0);
	return 0;
}

void filterGaussian(){

	//Mat�^�ϐ��̐錾
	Mat inputImage, outputImage, inputImage2;

	//�摜�̓ǂݍ���
	inputImage = imread("ISOnoise.jpg");

	//RGB�摜���O���[�X�P�[���ɕϊ�
	GaussianBlur(inputImage, outputImage, Size(0,0), 6, 6);

	//�摜�̕\��
	imshow("gaussian",outputImage);
	//�摜�̏����o��
	imwrite("gaussian_lena.jpg",outputImage);

}

void filterBilateral(){

	//Mat�^�ϐ��̐錾
	Mat inputImage, outputImage, inputImage2;

	//�摜�̓ǂݍ���
	inputImage = imread("ISOnoise.jpg");

	//RGB�摜���O���[�X�P�[���ɕϊ�
	bilateralFilter(inputImage, outputImage, 6, 50 ,400);

	//�摜�̕\��
	imshow("bilateral",outputImage);
	//�摜�̏����o��
	imwrite("bilateral_lena.jpg",outputImage);

}

void filterMedian(){

	//Mat�^�ϐ��̐錾
	Mat inputImage, outputImage, inputImage2;

	//�摜�̓ǂݍ���
	inputImage = imread("webcam8.jpg");

	//RGB�摜���O���[�X�P�[���ɕϊ�
	medianBlur(inputImage, outputImage, 21);

	//�摜�̕\��
	imshow("median",outputImage);
	//�摜�̏����o��
	imwrite("median_lena.jpg",outputImage);

}


