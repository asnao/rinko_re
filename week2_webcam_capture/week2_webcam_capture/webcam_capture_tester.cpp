#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

using namespace cv;

int main(int argc, char** argv){

	cv::VideoCapture cap(0); // デフォルトカメラをオープン
    if(!cap.isOpened())  // 成功したかどうかをチェック
        return -1;

    cv::Mat input_image;
    cv::namedWindow("window1",1);
    for(;;)
    {
        cv::Mat frame;
        cap >> frame; // カメラから新しいフレームを取得
        input_image=frame;      //matのコピーは普通に=で結べば良いみたい．
        imshow("window1", input_image);
        if(cv::waitKey(30) >= 0) break;
    }
    // VideoCapture デストラクタにより，カメラは自動的に終了処理されます
    return 0;


}