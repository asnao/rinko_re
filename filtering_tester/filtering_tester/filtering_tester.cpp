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

	//OpenCVの関数．何かしらのボタンの入力があるまで，プログラムを止める．
	waitKey(0);
	return 0;
}

void filterGaussian(){

	//Mat型変数の宣言
	Mat inputImage, outputImage, inputImage2;

	//画像の読み込み
	inputImage = imread("ISOnoise.jpg");

	//RGB画像をグレースケールに変換
	GaussianBlur(inputImage, outputImage, Size(0,0), 6, 6);

	//画像の表示
	imshow("gaussian",outputImage);
	//画像の書き出し
	imwrite("gaussian_lena.jpg",outputImage);

}

void filterBilateral(){

	//Mat型変数の宣言
	Mat inputImage, outputImage, inputImage2;

	//画像の読み込み
	inputImage = imread("ISOnoise.jpg");

	//RGB画像をグレースケールに変換
	bilateralFilter(inputImage, outputImage, 6, 50 ,400);

	//画像の表示
	imshow("bilateral",outputImage);
	//画像の書き出し
	imwrite("bilateral_lena.jpg",outputImage);

}

void filterMedian(){

	//Mat型変数の宣言
	Mat inputImage, outputImage, inputImage2;

	//画像の読み込み
	inputImage = imread("webcam8.jpg");

	//RGB画像をグレースケールに変換
	medianBlur(inputImage, outputImage, 21);

	//画像の表示
	imshow("median",outputImage);
	//画像の書き出し
	imwrite("median_lena.jpg",outputImage);

}


