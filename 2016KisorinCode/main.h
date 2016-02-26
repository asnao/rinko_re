#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;


// 自分の環境に合わせて変更する
// 32bit → x86
// Visual Studio 2010 → vc10
// Visual Studio 2012 → vc11
// Visual Studio 2013 → vc12
// OpenCVのバージョンにも注意


//Property Sheetを設定する場合はここは記述する必要なし
//Headerの中に毎回書き込むとコードも長くなってしまうので記述せず，Property Sheetに記述するのがおすすめ
#ifdef _DEBUG
    //Debugモードの場合
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_calib3d249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_contrib249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_core249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_features2d249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_flann249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_gpu249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_highgui249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_imgproc249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_legacy249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_ml249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_nonfree249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_objdetect249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_ocl249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_photo249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_stitching249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_superres249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_ts249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_video249d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_videostab249d.lib")
#else
    //Releaseモードの場合
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_calib3d249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_contrib249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_core249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_features2d249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_flann249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_gpu249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_highgui249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_imgproc249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_legacy249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_ml249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_nonfree249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_objdetect249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_ocl249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_photo249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_stitching249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_superres249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_ts249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_video249.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x64\\vc11\\lib\\opencv_videostab249.lib")
#endif
