#ifndef __KINECTV2_H__
#define __KINECTV2_H__

#include "main.h"
#include <pcl/ModelCoefficients.h> 

class KinectV2
{
private: //書き換えできないようにする

	///ここら辺の変数は変更しない///
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
	///ここまで///

	///人物検出に使用する///
	IBodyIndexFrameSource* pBodyIndexSource;
	IBodyIndexFrameReader* pBodyIndexReader;
	IFrameDescription* pBodyDescription;
	IBodyIndexFrame* pBodyIndexFrame;

	IBodyFrameReader* pBodyReader;
	IBodyFrameSource* pBodySource;
	IBodyFrame* pBodyFrame;
	IColorFrame* pColorFrame;
	///ここまで///

	///点群を扱う際に使用する///
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointcloud;
	bool updatePointcloud;
	///ここまで///


	///カラー画像と距離画像の位置合わせに使用する///
	CameraSpacePoint* cameraSpacePointsInColorIndex;
	CameraSpacePoint* cameraSpacePointsInDepthIndex;
	///ここまで///



	////////////////////////////////////
	/////使う変数をここに加えていく/////
	////////////////////////////////////

	cv::Mat colorImg;
	cv::Mat depthImg;



public: //書き換え可能
	KinectV2(); //コンストラクタ 必ず一番最初に呼び出される.
	KinectV2(cv::Mat argDepthImg);

	~KinectV2(); //ディストラクタ


	///関数の宣言を行う///
	int RenewColorFrame();
	int ShowColorImg();
	int RenewDepthFrame();
	int ShowDepthImg();
	int RenewPointCloud();
	int ShowPointCloud();
	int SaveData();
};





#endif __KINECTV2_H__