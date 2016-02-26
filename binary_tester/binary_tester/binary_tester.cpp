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
	//Mat型変数の宣言
	Mat inImg, bgImg, diffClrImg, diffGryImg, biBgImg, outImg;
	//画像の読み込み
	inImg = imread("input6.jpg");
	bgImg = imread("background6.jpg");

	imshow("in", inImg);
	imshow("bg", bgImg);
	waitKey(10);


	absdiff(inImg,bgImg,diffClrImg);//背景画像との差分を取得
	cv::imwrite("diff.jpg", diffClrImg);	

    cvtColor(diffClrImg,diffGryImg,CV_BGR2GRAY);//差分画像をグレイスケールに
    threshold(diffGryImg, biBgImg, th, 255, THRESH_BINARY);//二値化画像化
	
		
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

	//画像の表示
	imshow("subtraction",outImg);
	waitKey(10);
		//画像の書き出し
	imwrite("subtraction.jpg",outImg);

	imshow("beforelabel", biBgImg);





	// Labelingの結果を受け取る
	cv::Mat label(inImg.size(), CV_16SC1);
 
	// ラベリングを実施 ２値化した画像に対して実行する。
	LabelingBS	labeling;
	labeling.Exec(biBgImg.data, (short *)label.data, inImg.cols, inImg.rows, false, 0);	
	
	// ラベリング結果を出力する、真っ白な状態で初期化
	cv::Mat outimg(inImg.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	
	// ラベルされた領域をひとつずつ描画
	for( int i = 0; i < labeling.GetNumOfRegions(); i++)
	{
		// ラベリング結果でイロイロする。
		// ラベリング結果の領域を抽出する。
		cv::Mat labelarea;
		cv::compare(label, i + 1, labelarea, CV_CMP_EQ);
		// 抽出した領域にランダムな色を設定して出力画像に追加。
		cv::Mat color(inImg.size(), CV_8UC3, randomColor());
  		color.copyTo(outimg, labelarea);
	}
	// 入力画像とラベリング結果の画像を画面表示
	imshow("label", outimg);
	cv::waitKey();
	cv::hconcat(inImg, outimg, outimg);
	// ラベルされた出力画像を保存
	cv::imwrite("labeled.png", outimg);



	//OpenCVの関数．何かしらのボタンの入力があるまで，プログラムを止める．
	waitKey(0);
	return 0;
}

