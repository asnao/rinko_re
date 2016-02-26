//2013年2月5日作成by沼田敦，前田直哉
//2014年2月6日改変by中山祐介
//2015年2月10日改変by八馬遼

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

	//OpenCVの関数．何かしらのボタンの入力があるまで，プログラムを止める．
	waitKey(0);
	return 0;
}

void grayscale(){

	//Mat型変数の宣言
	Mat inputImage, outputImage, inputImage2;

	//画像の読み込み
	inputImage = imread("./image/lena.jpg");

	//RGB画像をグレースケールに変換
	cvtColor(inputImage, outputImage, CV_RGB2GRAY);

	//画像の表示
	imshow("gray",outputImage);
	//画像の書き出し
	imwrite("./output/gray_lena.jpg",outputImage);




	//画像の読み込み 読み込み時に画像名の後ろで0と指定してあげれば，グレースケールで読み込める．1と指定してあげれば，RGBで読み込む
	//.bmpを読む場合は若干処理が必要だが、.jpgや.pngなどは同じように読み込める。
	inputImage2 = imread("./image/lena.jpg",0);

	imshow("gray2",inputImage2);
	//imshowは
	//どこかでwaitKey(0)を呼び出す。止めたくない場合waitKey(1);のように1mmsecだけ止める。←これをしておかないと画面が真っ暗
	imwrite("./output/gray_lena2.jpg",inputImage2);
	//書き込むときも.bmpや.pngなどは拡張子を変えるだけで書き込める。
}

void BrightnessInversion(){

	Mat inputImage,outputImage;
	inputImage = imread("./image/lena.jpg");

	//outputImgを幅inputImg.cols, 高さinputImg.rows, CV_8UC3 は 8bit unsigned char型 (0~255) 3チャンネル(RGB)に設定し，Mat::zerosによって全ての値を0にする．
	//CV_8UC3といった型情報定数についてはhttp://d.hatena.ne.jp/arche_t/20090120/1232445728を参照(8bit=1byteに注意)
	//Mat型ではCV_～というようなOpenCVで決められた型を使う
	//また，
	//Mat outputImg(inputImg.cols, inputImg.rows, CV_8UC3);
	//というように変数宣言と形式定義を同時に行うことも可能
	//その他の初期化やMatの操作については「OpenCV逆引きリファレンス」参照

	outputImage = Mat::zeros(inputImage.rows,inputImage.cols,CV_8UC3);

	//画像のチャンネル数を取得
	int ch = inputImage.channels();

	//明度値を反転させる．
	//Mat型の明度値アクセスに関してはhttp://opencv.jp/opencv2-x-samples/access_pixel_valueを参照するといいと思います．
	for (int y = 0; y < inputImage.rows; y++) {

		//unsigned char型のポインタip,opにinputImg,outputImgのy行目の先頭アドレスを代入
		//画像の横一列を取得するイメージで．
		uchar *ip = inputImage.ptr(y);
		uchar *op = outputImage.ptr(y);
		for (int x = 0; x < inputImage.cols; x++) {
			for (int k = 0; k < ch; k++) {
				//各カラーの明度値にアクセスするには3の倍数ごとに分けてアクセスする．
				op[x*ch+k] = 255 - (unsigned char)ip[x*ch+k];
			}
		}
	}
	imshow("negative",outputImage);
	imwrite("./output/negative_lena.jpg",outputImage);
}

//Matの別の指定の仕方
void BrightnessInversion2(){

	//画像の3チャンネルの指定
	Mat_<Vec3b> inputImage;
	inputImage = imread("./image/lena.jpg");
	//cv::Mat_<_Tp> は， cv::Mat を継承したクラステンプレートです． cv::Mat_<_Tp> にも cv::Mat にも，virtual なメソッドは存在せず，データフィールドも同一なので，互いのポインタを自由に変換することができます． とは言っても，互いの data メンバの型が変換されるわけではないので，注意が必要ですbyOpenCV．
	//要はMat型の汎用性が高いVerで<>のなかに好きな型を指定できます。
	//画像の3チャンネルの指定，縦と横のサイズ指定
	Mat_<Vec3b> outputImage( inputImage.rows, inputImage.cols);

	int ch = inputImage.channels();

	for(int y = 0; y < inputImage.rows; y++){
		for(int x = 0; x < inputImage.cols; x++){
			for(int k = 0; k  < ch; k++){
				//この方法だと2次元配列のようにアクセスすることができる．
				//RGBのアクセス方法は構造体のように，val[]の値を0~2に変えてください．
				outputImage[y][x].val[k] = 255 - inputImage[y][x].val[k];
			}
		}
	}

	imshow("negative2",outputImage);
	imwrite("./output/negative_lena2.jpg",outputImage);
}
//Matの別の指定の仕方2
void BrightnessInversion3(){

	Mat inputImage,outputImage;
	inputImage = imread("./image/lena.jpg");
	outputImage = Mat::zeros(inputImage.rows,inputImage.cols,CV_8UC3);

	//明度値を反転させる．
	//Mat型の明度値アクセスに関してはhttp://opencv.jp/opencv2-x-samples/access_pixel_valueを参照するといいと思います．
	for (int y = 0; y < inputImage.rows; y++) {
		for (int x = 0; x < inputImage.cols; x++) {

				//各カラーの明度値にアクセスするには画像サイズごとに3chまとめて呼び出す。
				outputImage.at<cv::Vec3b>(y,x) = cv::Vec3b(255,255,255)-inputImage.at<cv::Vec3b>(y,x);
				//Mat型の一の要素にdouble型で一つずつの値で宣言していればoutputImage.at<double>(y,x)のように取り出すこともできる。
				//つまりMat.at<要素ごとの型>(y,x)で取り出せる。
				//Mat自体は二次元配列だが、一つの要素にいろいろな型を当てはめれる。
				//		Mat test=(Mat_<double>(3, 3) <<1,0,0,
				//	                                   0,1,0,
				//									　 0,0,1);
				//std::cout<< test.at<double>(1,1);

		}
	}
	imshow("negative3",outputImage);
	imwrite("./output/negative_lena3.jpg",outputImage);
}