#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

using namespace cv;


void edgeDiff();
void edgeSobel();
void edgeLaplacian();
void edgeLOG2();
void edgeLaplacian2();

int main(){
	edgeDiff();
	edgeLOG2();
	edgeLaplacian2();

	//OpenCVの関数．何かしらのボタンの入力があるまで，プログラムを止める．
	waitKey(0);
	return 0;
}

void edgeDiff(){

	//Mat型変数の宣言
	Mat inputImage, outputImage, tempImage;

	//画像の読み込み
	inputImage = imread("input.jpg");

	//RGB画像をグレースケールに変換
	GaussianBlur(inputImage, outputImage, Size(11,11), 5, 5);

	//画像の表示
	imshow("gaussian",outputImage);
	//画像の書き出し
	imwrite("gaussian_lena.jpg",outputImage);

}

void edgeSobel(){

	//Mat型変数の宣言
	Mat inputImage, outputImage, tempImage;

	//画像の読み込み
	inputImage = imread("lena.jpg");
	
	cv::Sobel(inputImage, tempImage, CV_32F, 1, 1);
	cv::convertScaleAbs(tempImage, outputImage, 1, 0);
	
	//画像の表示
	imshow("Sobel",outputImage);
	//画像の書き出し
	imwrite("Sobel_lena.jpg",outputImage);

}
/*
void edgeLaplacian(){

	//Mat型変数の宣言
	Mat inputImage, outputImage, tempImage;

	//画像の読み込み
	inputImage = imread("lena.jpg");


	Laplacian(inputImage, tempImage, CV_32F, 3);
//	convertScaleAbs(tempImage, tempImage, 1, 0);
	
//	cvtColor(tempImage,tempImage,CV_BGR2GRAY);

	imshow("laplacian",tempImage);
	waitKey(10);

	Mat zeroCross = Mat::zeros(cv::Size(tempImage.rows, tempImage.cols), CV_32F);

	for(int y = 1; y < tempImage.rows - 1; y++){
		for(int x = 1; x < tempImage.cols - 1; x++){
			for(int c = 0; c < 3; c++){
				if(tempImage.data[y * tempImage.cols * 3 + x * 3 + c] && 
					(tempImage.data[y * tempImage.cols * 3 + (x - 1) * 3 + c] > 0 && tempImage.data[y * tempImage.cols * 3 + (x + 1) * 3 + c] < 0 ||
					tempImage.data[y * tempImage.cols * 3 + (x - 1) * 3 + c] < 0 && tempImage.data[y * tempImage.cols * 3 + (x + 1) * 3 + c] > 0)){
						zeroCross.data[y * tempImage.cols * 3 + x * 3 + c] = 255.0;
				}else{
					zeroCross.data[y * tempImage.cols * 3 + x * 3 + c] = 122.0;
				}
			}
		}
	}
	convertScaleAbs(zeroCross, zeroCross, 1, 0);
	
	for(int y = 1; y < tempImage.rows - 1; y++){
		for(int x = 1; x < tempImage.cols - 1; x++){
				if(tempImage.data[y * tempImage.cols + x] && 
					(tempImage.data[y * tempImage.cols + (x - 1)] > 0 && tempImage.data[y * tempImage.cols + (x + 1)] < 0 ||
					tempImage.data[y * tempImage.cols + (x - 1)] < 0 && tempImage.data[y * tempImage.cols + (x + 1)] > 0)){
						zeroCross.data[y * tempImage.cols + x] = 255;
				}else{
					zeroCross.data[y * tempImage.cols + x] = 0;
				}
		}
	}



	//画像の表示
	imshow("zerocrossing",zeroCross);
	waitKey(10);

	//convertScaleAbs(tempImage, outputImage, 1, 0);

	//画像の表示

	//画像の書き出し
	//imwrite("median_lena.jpg",outputImage);

}
*/

void edgeLOG2(){
	Mat src, gray, dst, abs_dst;


    src = imread( "input2.jpg" );

    /// Remove noise by blurring with a Gaussian filter
    GaussianBlur( src, src, Size(3,3), 1, 1, BORDER_DEFAULT );
	
	cvtColor( src, gray, CV_RGB2GRAY );

    /// Apply Laplace function
    Laplacian( gray, dst, CV_16SC1, 3, 1, 0, BORDER_DEFAULT );

	convertScaleAbs(dst, abs_dst );

    imshow( "resultlog", abs_dst );
	imwrite("result_log.jpg",abs_dst);
 
    waitKey(0);
}

void edgeLaplacian2(){
	Mat src, gray, dst, abs_dst;

    src = imread( "input2.jpg" );

    /// Remove noise by blurring with a Gaussian filter
//    GaussianBlur( src, src, Size(7,7), 3, 3, BORDER_DEFAULT );
	
	cvtColor( src, gray, CV_RGB2GRAY );

    /// Apply Laplace function
    Laplacian( gray, dst, CV_16SC1, 3, 1, 0, BORDER_DEFAULT );

	convertScaleAbs(dst, abs_dst );

    imshow( "resultlap", abs_dst );
	imwrite("result_lap.jpg",abs_dst);
 
    waitKey(0);
}

void edgeLaplacian(){
	Mat src, gray, dst, abs_dst;


    src = imread( "input.jpg" );

    /// Remove noise by blurring with a Gaussian filter
    GaussianBlur( src, src, Size(7,7), 3, 3, BORDER_DEFAULT );
	
	cvtColor( src, gray, CV_RGB2GRAY );

    /// Apply Laplace function
    Laplacian( gray, dst, CV_16SC1, 3, 1, 0, BORDER_DEFAULT );

	Mat zeroCross(dst.rows, dst.cols, CV_16SC1);
	for(int y = 1; y < dst.rows - 1; y++){
		for(int x = 1; x < dst.cols - 1; x++){
				/*if(dst.data[y * dst.step + x * dst.elemSize()] == 0 && (
					(dst.data[y * dst.step + (x - 1) * dst.elemSize()] > 0 && dst.data[y * dst.step + (x + 1) * dst.elemSize()] < 0) ||
					(dst.data[y * dst.step + (x - 1) * dst.elemSize()] < 0 && dst.data[y * dst.step + (x + 1) * dst.elemSize()] > 0) ||
					(dst.data[(y - 1) * dst.step + x * dst.elemSize()] > 0 && dst.data[(y + 1) * dst.step + x * dst.elemSize()] < 0) ||
					(dst.data[(y - 1) * dst.step + x * dst.elemSize()] < 0 && dst.data[(y + 1) * dst.step + x * dst.elemSize()] > 0))) {
						zeroCross.data[y * zeroCross.step + x * zeroCross.elemSize()] = 255;
				}else{
					zeroCross.data[y * zeroCross.step + x * zeroCross.elemSize()] = 125;
				}*/
			if((dst.at<short>(y,x) < 5 && dst.at<short>(y,x) > - 5) && (
				(dst.at<short>(y, x - 1) > 5 && dst.at<short>(y,x + 1) < -5) ||
				(dst.at<short>(y, x - 1) < -5 && dst.at<short>(y,x + 1) > 5) ||
				(dst.at<short>(y - 1, x) > 5 && dst.at<short>(y + 1,x) < -5) ||
				(dst.at<short>(y - 1, x) < -5 && dst.at<short>(y + 1,x) > 5) || 
				(dst.at<short>(y - 1, x - 1) > 5 && dst.at<short>(y + 1, x + 1) < -5) ||
				(dst.at<short>(y - 1, x - 1) < -5 && dst.at<short>(y + 1,x + 1) > 5) ||
				(dst.at<short>(y - 1, x + 1) > 5 && dst.at<short>(y + 1,x - 1) < -5) ||
				(dst.at<short>(y - 1, x + 1) < -5 && dst.at<short>(y + 1,x - 1) > 5)
				)){

					zeroCross.at<short>(y,x) = 255;
			}else{
				zeroCross.at<short>(y,x) = 0;
			}
		}
	}
		
	imshow( "result11", ~zeroCross );
	waitKey(0);

	convertScaleAbs(zeroCross, abs_dst );
/*
	for(int y = 1; y < dst.rows - 1; y++){
		for(int x = 1; x < dst.cols - 1; x++){
				if(dst.data[y * dst.cols + x] == 0 && 
					dst.data[y * dst.cols + (x - 1)] > 0 && dst.data[y * dst.cols + (x + 1)] > 0 ||
					dst.data[y * dst.cols + (x - 1)] > 0 && dst.data[y * dst.cols + (x + 1)] > 0 ||
					dst.data[(y - 1) * dst.cols + x] > 0 && dst.data[(y + 1) * dst.cols + x] > 0 ||
					dst.data[(y - 1) * dst.cols + x] > 0 && dst.data[(y + 1) * dst.cols + x] > 0) {
					zeroCross.data[y * dst.cols + x] = 255;
				}else{
					zeroCross.data[y * dst.cols + x] = 0;
				}
		}
	}
*/
    imshow( "result", abs_dst );
 
    waitKey(0);
}


