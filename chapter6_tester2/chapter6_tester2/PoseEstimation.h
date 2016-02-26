#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <windows.h>
#include <cmath>
#define M_PI 3.1415926535897932384626433832795

#include <GL/glew.h> // before including gl.h 
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#pragma comment(lib,"glut32.lib")
//#pragma comment(lib,"glew32.lib")

#include <opencv2/opencv.hpp>


#ifdef _DEBUG
// debug
#pragma comment(lib,"opencv_core300d.lib")    
#pragma comment(lib,"opencv_features2d300d.lib")   
#pragma comment(lib,"opencv_flann300d.lib")   
#pragma comment(lib,"opencv_videoio300d.lib")   
#pragma comment(lib,"opencv_calib3d300d.lib")   
//#pragma comment(lib,"opencv_xfeatures2d300d.lib")   
#pragma comment(lib,"opencv_imgcodecs300d.lib")    
#else
// release
#pragma comment(lib,"opencv_core300.lib")    
#pragma comment(lib,"opencv_features2d300.lib")   
#pragma comment(lib,"opencv_flann300.lib")   
#pragma comment(lib,"opencv_videoio300.lib")   
#pragma comment(lib,"opencv_calib3d300.lib")   
//#pragma comment(lib,"opencv_xfeatures2d300.lib")    
#pragma comment(lib,"opencv_imgcodecs300.lib")    
#endif

/* ���ʃ}�[�J���g�����J�����ʒu�E�p������ */
class FeatureTracker{
public:
	double f;                           // �œ_����[mm]
	double sx, sy;             // �Z���T�T�C�Y
	double w, h;        // �B�e�摜�̃T�C�Y
	double fovy, aspect;	// gluPerspective�ɓ��͂���p�����[�^
	
	cv::Mat intrinsic;	// �����p�����[�^�s��
	cv::Mat distcoeffs;	//�c�݃p�����[�^
	cv::Mat extrinsic;	// �O���p�����[�^�s��
	cv::Mat extrinsicGL;	// GL�p�O���p�����[�^�s��
	cv::Mat rvec;          // ��]�x�N�g��
	cv::Mat tvec;          // ���s�ړ��x�N�g��

	cv::Ptr<cv::Feature2D> detector;	// �L�[�|�C���g�E�f�B�X�N���v�^�̌v�Z�C���X�^���X
	
	cv::Ptr<cv::DescriptorMatcher> matcher;	// �}�b�`���O�C���X�^���X

	cv::Mat marker;		// �}�[�J�摜
	std::vector<cv::KeyPoint> markerKeypoint;	// �}�[�J�摜�̃L�[�|�C���g
	cv::Mat markerDescriptor;	// �}�[�J�L�[�|�C���g�̃f�B�X�N���v�^
	double markerSize[2];	// �}�[�J�摜�̉����E���� [mm]

	bool tracked;	// �J�����ʒu�E�p����������Ƌ��߂��Ă��邩�i�O��l��������false�j

	/* �R���X�g���N�^1�FORB���g�����R���X�g���N�^ */
	FeatureTracker(){
		tracked = false;

		// ORB�̃C���X�^���X�𐶐��B������ύX����Ƒ��̎�@�ɐ؂�ւ��\
		detector = cv::ORB::create();

		// ������ύX����Ƌ����ړx�ƒT����@��ύX�\
		matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");

		extrinsic = cv::Mat::eye(4, 4, CV_64F);
	}

	/* �R���X�g���N�^2�F�����Ƃ��ĔC�ӂ̃L�[�|�C���g/�f�B�X�N���v�^�v�Z�A�}�b�`���O��@����� */
	FeatureTracker(cv::Ptr<cv::Feature2D> _detector, cv::Ptr<cv::DescriptorMatcher> _matcher)
		:detector(_detector), matcher(_matcher){
		tracked = false;

		extrinsic = cv::Mat::eye(4, 4, CV_64F);
	}

	/* �ǂݍ��ݍς݂̃}�[�J�摜��o�^ */
	void LoadMarker(cv::Mat markerImg, double widthMM, double heightMM){
		marker = markerImg;
		markerSize[0] = widthMM;
		markerSize[1] = heightMM;

		// �}�[�J�摜�̃L�[�|�C���g�ƃf�B�X�N���v�^�𒊏o
		detector->detect(marker, markerKeypoint);
		detector->compute(marker, markerKeypoint, markerDescriptor);
	}

	/* �}�[�J�摜��ǂݍ��� */
	void LoadMarker(std::string path, double heightMM, double widthMM){
		// �}�[�J�摜�̃T�C�Y��o�^
		markerSize[0] = widthMM;
		markerSize[1] = heightMM;

		// �}�[�J�摜�̃L�[�|�C���g�ƃf�B�X�N���v�^�𒊏o
		marker = cv::imread(path);	// �摜�̓ǂݍ���
		detector->detect(marker, markerKeypoint);	// �L�[�|�C���g�̌��o
		detector->compute(marker, markerKeypoint, markerDescriptor);	// �f�B�X�N���v�^�̌v�Z
	}

	/* �����p�����[�^�����*/
	void LoadCameraIntrinsic(double f, double sx, double sy, double w, double h){
		intrinsic = cv::Mat::zeros(3, 3, CV_64F);   // �����p�����[�^
		intrinsic.at<double>(0, 0) = w*f / sx;	//      [ fx   0  cx ]
		intrinsic.at<double>(1, 1) = h*f / sy;		//      [  0  fy  cy ]
		intrinsic.at<double>(0, 2) = w / 2;		//      [  0   0   1  ]
		intrinsic.at<double>(1, 2) = h / 2;
		intrinsic.at<double>(2, 2) = 1;

		fovy = abs(atan(h / f) * 2.0) * 180.0 / M_PI;
		aspect = sx / sy;
	}

	/* �t�@�C����������p�����[�^��ǂݍ���*/
	void LoadCameraIntrinsic(std::string path, double imgWidth = 640, double imgHeight = 480){
		intrinsic = cv::Mat::zeros(3, 3, CV_64F);   

		cv::FileStorage fs(path, cv::FileStorage::READ);
		fs["intrinsic"] >> intrinsic;
		fs["distortion"] >> distcoeffs;
		fs.release();

		w = imgWidth;
		h = imgHeight;
		f = intrinsic.at<double>(1, 1) / h;

		fovy = abs(atan(h / 2.0 / intrinsic.at<double>(1, 1)) * 2.0) * 180.0 / M_PI;
		aspect = (w*f / intrinsic.at<double>(0, 0)) / (h*f / intrinsic.at<double>(1, 1));
	}

	/* �]�u���A3x4�s���4x4�s��ɕϊ� */
	double *getGLModelView(){
		cv::Mat trans = cv::Mat::eye(4, 4, CV_64F);
		extrinsicGL = (trans*extrinsic).t();

		return extrinsicGL.ptr<double>();
	}

	/* �}�[�J���������Ă��邩�ǂ��� */
	bool isTracked(){
		return tracked;
	}

	/* OpenGL�p�����p�����[�^�s��𐶐� */
	void getGLProjection(double nearClip, double farClip){
		gluPerspective(fovy, aspect, nearClip, farClip);
	}

	/* ���t���[���̏����i�J�����ʒu�E�p������ */
	void Track(cv::Mat frame){

		/* �B�e�摜�̃L�[�|�C���g�ƃf�B�X�N���v�^�𒊏o */
		std::vector<cv::KeyPoint> frameKeypoint;	//�L�[�|�C���g
		cv::Mat frameDescriptor;	// �f�B�X�N���v�^
		detector->detect(frame, frameKeypoint);	// �L�[�|�C���g�����o
		detector->compute(frame, frameKeypoint, frameDescriptor);	// �f�B�X�N���v�^���v�Z
		if (frameDescriptor.rows == 0) return;	// �L�[�|�C���g���ЂƂ����o����Ȃ������ꍇ

		/* �}�b�`���O�i�ł��ގ�����_�Ƃ��̎��ɗގ�����_�̗ގ��x�̔䂩��A�M�����̍����Ή����c���j */
		// �ł��ގ�����_�ƁA���̎��ɗގ�����_���Z�o
		std::vector<std::vector<cv::DMatch>> matches;	// �_�̑Ή����i�[�i�ŗގ��_��2�ʂ̓_�j
		matcher->knnMatch(markerDescriptor, frameDescriptor, matches, 2);	// �}�[�J�ƎB�e�摜�̃}�b�`���O

		// �ł��ގ�����_�ƁA���̎��ɗގ�����_�̗ގ��x�̔䂩��A�M�����̍����Ή����c��
		std::vector<cv::KeyPoint> markerMatched, frameMatched;
		for (int i = 0; i < matches.size(); i++) {
			if (matches[i].size() < 2) continue;

			if (matches[i][0].distance < 0.8 * matches[i][1].distance) {
				markerMatched.push_back(markerKeypoint[matches[i][0].queryIdx]);
				frameMatched.push_back(frameKeypoint[matches[i][0].trainIdx]);
			}
		}	
		
		/* �J�����ʒu�E�p������ */
		// �Ή��_�̓o�^
		std::vector<cv::Point3f> objectPoints;    // ���E���W���3�����_�Q
		std::vector<cv::Point2f> imagePoints;    // �摜���W���2�����_�Q

		for (int i = 0; i < markerMatched.size(); i++){
			// ���ʃ}�[�J���3�������W�v�Z�i���E���W�n: �}�[�J�������_�AZ��������������j
			cv::Point3f point3d;	// 3�����_
			point3d.x = markerMatched[i].pt.x*markerSize[0] / marker.cols;
			point3d.y = markerSize[1] - (markerMatched[i].pt.y*markerSize[1] / marker.rows);
			point3d.z = 0;
			objectPoints.push_back(point3d);

			// �摜���2�������W
			cv::Point2f point2d = frameMatched[i].pt;    // 2�����_
			imagePoints.push_back(point2d); 
		}

		// PnP���������A��]�E���s�ړ��x�N�g�����Z�o
		// intrinsic �� distcoeffs �́A�J�����L�����u���[�V�����ɂ�苁�߂�ꂽ�����p�����[�^�s��ƃJ�����̃����Y�c��
		cv::Mat inliers;
		if (objectPoints.size() == 0) return;	// �Ή��_���Ȃ��ꍇ
		
		if (rvec.empty()){
			// �ŏ��̃t���[��
			cv::solvePnPRansac(objectPoints, imagePoints, intrinsic, distcoeffs, rvec, tvec, false, 100, 8.0, 0.99/*, inliers, cv::SOLVEPNP_EPNP*/);
		}
		else{
			// 2�t���[���ڈȍ~�́A�O�t���[���̉�]�E���s�ړ��x�N�g���������l�Ƃ���
			cv::solvePnPRansac(objectPoints, imagePoints, intrinsic, distcoeffs, rvec, tvec, true, 100, 8.0, 0.99/*, inliers, cv::SOLVEPNP_EPNP*/);
		}

		// �O��l�łȂ��_��10�_�����Ȃ�A�}�[�J���ʂ��Ă��Ȃ��Ɣ��f
		if (double(inliers.rows) < 10){
			tracked = false;
		}
		else{
			tracked = true;
		}
	
		// ��]�E���s�ړ��x�N�g�����O���p�����[�^�s��ɕϊ�
		cv::Mat rmat;         // ��]�s��
		cv::Rodrigues(rvec, rmat);	// ���h���Q�X�̌����ɂ��A��]�x�N�g�����s��ɕϊ�
		
		// ���W�ϊ��s����쐬
		// R R R | t
		// R R R | t
		// R R R | t
		extrinsic.at<double>(0, 0) = rmat.at<double>(0, 0);
		extrinsic.at<double>(0, 1) = rmat.at<double>(0, 1);
		extrinsic.at<double>(0, 2) = rmat.at<double>(0, 2);
		extrinsic.at<double>(1, 0) = rmat.at<double>(1, 0);
		extrinsic.at<double>(1, 1) = rmat.at<double>(1, 1);
		extrinsic.at<double>(1, 2) = rmat.at<double>(1, 2);
		extrinsic.at<double>(2, 0) = rmat.at<double>(2, 0);
		extrinsic.at<double>(2, 1) = rmat.at<double>(2, 1);
		extrinsic.at<double>(2, 2) = rmat.at<double>(2, 2);
		extrinsic.at<double>(0, 3) = tvec.at<double>(0);
		extrinsic.at<double>(1, 3) = tvec.at<double>(1);
		extrinsic.at<double>(2, 3) = tvec.at<double>(2);
	}	

};