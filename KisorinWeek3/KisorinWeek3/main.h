#ifndef INCLUDED_MAIN_H
#define INCLUDED_MAIN_H

//�v���W�F�N�g�̃v���p�e�B��C/C++�ˑS�ʁ@�̒ǉ��̃C���N���[�h�f�B���N�g����
// opencv2�̂���t�H���_�wC:\OpenCV2.4\include�x�Ȃǂ�ǉ��̂���
#include "opencv2\opencv.hpp"

//OpenCV�̃o�[�W�����ɂ����~.lib�̑O�̐������Ⴄ
//ex) ...\\opencv_core243d.lib (ver 2.4.3)
//ex) ...\\opencv_core240d.lib (ver 2.4.0)

#ifdef _DEBUG
//Debug���[�h�̏ꍇ
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_core243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_imgproc243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_highgui243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_objdetect243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_contrib243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_features2d243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_flann243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_gpu243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_haartraining_engined.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_legacy243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_ts243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_video243d.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_calib3d243d.lib")
#else
//Release���[�h�̏ꍇ
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_core243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_imgproc243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_highgui243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_objdetect243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_contrib243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_features2d243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_flann243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_gpu243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_haartraining_engined.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_legacy243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_ts243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_video243.lib")
#pragma comment(lib,"C:\\OpenCV2.4\\build\\x86\\vc10\\lib\\opencv_calib3d243.lib")

#endif

#endif