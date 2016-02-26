#ifndef __KINECTV2_H__
#define __KINECTV2_H__

#include "main.h"
#include <pcl/ModelCoefficients.h> 

class KinectV2
{
private: //���������ł��Ȃ��悤�ɂ���

	///������ӂ̕ϐ��͕ύX���Ȃ�///
	IKinectSensor* pSensor;
	HRESULT hResult;
	ICoordinateMapper* pCoordinateMapper;
	IColorFrameSource* pColorSource;
	IDepthFrameSource* pDepthSource;
	IColorFrameReader* pColorReader;
	IDepthFrameReader* pDepthReader;
	IFrameDescription* pColorDescription;
	int colorWidth;
	int colorHeight;
	std::vector<RGBQUAD> colorBuffer;
	IFrameDescription* pDepthDescription;
	int depthWidth;
	int depthHeight;
	std::vector<UINT16> depthBuffer;
	std::vector<DepthSpacePoint> depthSpace;
	IDepthFrame* pDepthFrame;
	///�����܂�///

	///�l�����o�Ɏg�p����///
	IBodyIndexFrameSource* pBodyIndexSource;
	IBodyIndexFrameReader* pBodyIndexReader;
	IFrameDescription* pBodyDescription;
	IBodyIndexFrame* pBodyIndexFrame;

	IBodyFrameReader* pBodyReader;
	IBodyFrameSource* pBodySource;
	IBodyFrame* pBodyFrame;
	IColorFrame* pColorFrame;
	///�����܂�///

	///�_�Q�������ۂɎg�p����///
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointcloud;
	bool updatePointcloud;
	///�����܂�///


	///�J���[�摜�Ƌ����摜�̈ʒu���킹�Ɏg�p����///
	CameraSpacePoint* cameraSpacePointsInColorIndex;
	CameraSpacePoint* cameraSpacePointsInDepthIndex;
	///�����܂�///



	////////////////////////////////////
	/////�g���ϐ��������ɉ����Ă���/////
	////////////////////////////////////

	cv::Mat colorImg;
	cv::Mat depthImg;



public: //���������\
	KinectV2(); //�R���X�g���N�^ �K����ԍŏ��ɌĂяo�����.
	KinectV2(cv::Mat argDepthImg);

	~KinectV2(); //�f�B�X�g���N�^


	///�֐��̐錾���s��///
	int RenewColorFrame();
	int ShowColorImg();
	int RenewDepthFrame();
	int ShowDepthImg();
	int RenewPointCloud();
	int ShowPointCloud();
	int SaveData();
};





#endif __KINECTV2_H__