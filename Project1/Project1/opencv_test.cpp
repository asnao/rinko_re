#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

using namespace cv;

int main()
{
	Mat src = Mat::zeros(150, 220, CV_8UC3);
 //赤色で画像にHello Worldを描く
 putText(src, "Hello World", Point(5, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 200), 2, CV_AA);
 //緑色で画像に線を描く
 line(src, Point(190, 25), Point(190, 45), Scalar(0, 200, 0), 3);
 //要素を直接操作し画像に点を描く
 for (int x = 188; x < 192; x++)
  for (int y = 53; y < 57; y++) //y座標
   for (int i = 0; i < 2; i++) //i < 3にするとBGRすべての要素を200にする
    src.at<uchar>(Point(x * 3 + i, y)) = saturate_cast<uchar>(200);
 //画像を表示
 imshow("", src);
 //ウインドウの表示時間(0はキーが押されるまで閉じない)
 waitKey(0);
 return 0;
}
