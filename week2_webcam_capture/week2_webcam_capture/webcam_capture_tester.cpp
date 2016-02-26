#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

using namespace cv;

int main(int argc, char** argv){

	cv::VideoCapture cap(0); // �f�t�H���g�J�������I�[�v��
    if(!cap.isOpened())  // �����������ǂ������`�F�b�N
        return -1;

    cv::Mat input_image;
    cv::namedWindow("window1",1);
    for(;;)
    {
        cv::Mat frame;
        cap >> frame; // �J��������V�����t���[�����擾
        input_image=frame;      //mat�̃R�s�[�͕��ʂ�=�Ō��ׂΗǂ��݂����D
        imshow("window1", input_image);
        if(cv::waitKey(30) >= 0) break;
    }
    // VideoCapture �f�X�g���N�^�ɂ��C�J�����͎����I�ɏI����������܂�
    return 0;


}