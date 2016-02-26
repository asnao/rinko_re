//2013�N2��5���쐬by���c�ցC�O�c����
//2014�N2��6������by���R�S��
//2015�N2��10������by���n��

#include "main.h"

void grayscale();
void BrightnessInversion();
void BrightnessInversion2();
void BrightnessInversion3();

int main(){

	grayscale();
	BrightnessInversion();
	BrightnessInversion2();
	BrightnessInversion3();

	//OpenCV�̊֐��D��������̃{�^���̓��͂�����܂ŁC�v���O�������~�߂�D
	waitKey(0);
	return 0;
}

void grayscale(){

	//Mat�^�ϐ��̐錾
	Mat inputImage, outputImage, inputImage2;

	//�摜�̓ǂݍ���
	inputImage = imread("./image/lena.jpg");

	//RGB�摜���O���[�X�P�[���ɕϊ�
	cvtColor(inputImage, outputImage, CV_RGB2GRAY);

	//�摜�̕\��
	imshow("gray",outputImage);
	//�摜�̏����o��
	imwrite("./output/gray_lena.jpg",outputImage);




	//�摜�̓ǂݍ��� �ǂݍ��ݎ��ɉ摜���̌���0�Ǝw�肵�Ă�����΁C�O���[�X�P�[���œǂݍ��߂�D1�Ǝw�肵�Ă�����΁CRGB�œǂݍ���
	//.bmp��ǂޏꍇ�͎኱�������K�v�����A.jpg��.png�Ȃǂ͓����悤�ɓǂݍ��߂�B
	inputImage2 = imread("./image/lena.jpg",0);

	imshow("gray2",inputImage2);
	//imshow��
	//�ǂ�����waitKey(0)���Ăяo���B�~�߂����Ȃ��ꍇwaitKey(1);�̂悤��1mmsec�����~�߂�B����������Ă����Ȃ��Ɖ�ʂ��^����
	imwrite("./output/gray_lena2.jpg",inputImage2);
	//�������ނƂ���.bmp��.png�Ȃǂ͊g���q��ς��邾���ŏ������߂�B
}

void BrightnessInversion(){

	Mat inputImage,outputImage;
	inputImage = imread("./image/lena.jpg");

	//outputImg��inputImg.cols, ����inputImg.rows, CV_8UC3 �� 8bit unsigned char�^ (0~255) 3�`�����l��(RGB)�ɐݒ肵�CMat::zeros�ɂ���đS�Ă̒l��0�ɂ���D
	//CV_8UC3�Ƃ������^���萔�ɂ��Ă�http://d.hatena.ne.jp/arche_t/20090120/1232445728���Q��(8bit=1byte�ɒ���)
	//Mat�^�ł�CV_�`�Ƃ����悤��OpenCV�Ō��߂�ꂽ�^���g��
	//�܂��C
	//Mat outputImg(inputImg.cols, inputImg.rows, CV_8UC3);
	//�Ƃ����悤�ɕϐ��錾�ƌ`����`�𓯎��ɍs�����Ƃ��\
	//���̑��̏�������Mat�̑���ɂ��ẮuOpenCV�t�������t�@�����X�v�Q��

	outputImage = Mat::zeros(inputImage.rows,inputImage.cols,CV_8UC3);

	//�摜�̃`�����l�������擾
	int ch = inputImage.channels();

	//���x�l�𔽓]������D
	//Mat�^�̖��x�l�A�N�Z�X�Ɋւ��Ă�http://opencv.jp/opencv2-x-samples/access_pixel_value���Q�Ƃ���Ƃ����Ǝv���܂��D
	for (int y = 0; y < inputImage.rows; y++) {

		//unsigned char�^�̃|�C���^ip,op��inputImg,outputImg��y�s�ڂ̐擪�A�h���X����
		//�摜�̉������擾����C���[�W�ŁD
		uchar *ip = inputImage.ptr(y);
		uchar *op = outputImage.ptr(y);
		for (int x = 0; x < inputImage.cols; x++) {
			for (int k = 0; k < ch; k++) {
				//�e�J���[�̖��x�l�ɃA�N�Z�X����ɂ�3�̔{�����Ƃɕ����ăA�N�Z�X����D
				op[x*ch+k] = 255 - (unsigned char)ip[x*ch+k];
			}
		}
	}
	imshow("negative",outputImage);
	imwrite("./output/negative_lena.jpg",outputImage);
}

//Mat�̕ʂ̎w��̎d��
void BrightnessInversion2(){

	//�摜��3�`�����l���̎w��
	Mat_<Vec3b> inputImage;
	inputImage = imread("./image/lena.jpg");
	//cv::Mat_<_Tp> �́C cv::Mat ���p�������N���X�e���v���[�g�ł��D cv::Mat_<_Tp> �ɂ� cv::Mat �ɂ��Cvirtual �ȃ��\�b�h�͑��݂����C�f�[�^�t�B�[���h������Ȃ̂ŁC�݂��̃|�C���^�����R�ɕϊ����邱�Ƃ��ł��܂��D �Ƃ͌����Ă��C�݂��� data �����o�̌^���ϊ������킯�ł͂Ȃ��̂ŁC���ӂ��K�v�ł�byOpenCV�D
	//�v��Mat�^�̔ėp��������Ver��<>�̂Ȃ��ɍD���Ȍ^���w��ł��܂��B
	//�摜��3�`�����l���̎w��C�c�Ɖ��̃T�C�Y�w��
	Mat_<Vec3b> outputImage( inputImage.rows, inputImage.cols);

	int ch = inputImage.channels();

	for(int y = 0; y < inputImage.rows; y++){
		for(int x = 0; x < inputImage.cols; x++){
			for(int k = 0; k  < ch; k++){
				//���̕��@����2�����z��̂悤�ɃA�N�Z�X���邱�Ƃ��ł���D
				//RGB�̃A�N�Z�X���@�͍\���̂̂悤�ɁCval[]�̒l��0~2�ɕς��Ă��������D
				outputImage[y][x].val[k] = 255 - inputImage[y][x].val[k];
			}
		}
	}

	imshow("negative2",outputImage);
	imwrite("./output/negative_lena2.jpg",outputImage);
}
//Mat�̕ʂ̎w��̎d��2
void BrightnessInversion3(){

	Mat inputImage,outputImage;
	inputImage = imread("./image/lena.jpg");
	outputImage = Mat::zeros(inputImage.rows,inputImage.cols,CV_8UC3);

	//���x�l�𔽓]������D
	//Mat�^�̖��x�l�A�N�Z�X�Ɋւ��Ă�http://opencv.jp/opencv2-x-samples/access_pixel_value���Q�Ƃ���Ƃ����Ǝv���܂��D
	for (int y = 0; y < inputImage.rows; y++) {
		for (int x = 0; x < inputImage.cols; x++) {

				//�e�J���[�̖��x�l�ɃA�N�Z�X����ɂ͉摜�T�C�Y���Ƃ�3ch�܂Ƃ߂ČĂяo���B
				outputImage.at<cv::Vec3b>(y,x) = cv::Vec3b(255,255,255)-inputImage.at<cv::Vec3b>(y,x);
				//Mat�^�̈�̗v�f��double�^�ň���̒l�Ő錾���Ă����outputImage.at<double>(y,x)�̂悤�Ɏ��o�����Ƃ��ł���B
				//�܂�Mat.at<�v�f���Ƃ̌^>(y,x)�Ŏ��o����B
				//Mat���͓̂񎟌��z�񂾂��A��̗v�f�ɂ��낢��Ȍ^�𓖂Ă͂߂��B
				//		Mat test=(Mat_<double>(3, 3) <<1,0,0,
				//	                                   0,1,0,
				//									�@ 0,0,1);
				//std::cout<< test.at<double>(1,1);

		}
	}
	imshow("negative3",outputImage);
	imwrite("./output/negative_lena3.jpg",outputImage);
}