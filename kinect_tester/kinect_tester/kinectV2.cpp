#include "kinectV2.h"

template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL){
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

KinectV2::KinectV2() //初期化
{

	hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)){
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		//return -1;
	}

	hResult = pSensor->Open();
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
		//return -1;
	}

	hResult = pSensor->get_CoordinateMapper(&pCoordinateMapper);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_CoordinateMapper()" << std::endl;
		//return -1;
	}

	hResult = pSensor->get_ColorFrameSource(&pColorSource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
		//return -1;
	}

	hResult = pSensor->get_DepthFrameSource(&pDepthSource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
		//return -1;
	}

	hResult = pColorSource->OpenReader(&pColorReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
		//return -1;
	}

	hResult = pDepthSource->OpenReader(&pDepthReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IDepthFrameSource::OpenReader()" << std::endl;
		//return -1;
	}

	hResult = pColorSource->get_FrameDescription(&pColorDescription);
	if (FAILED(hResult)){
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
		//return -1;
	}

	colorWidth = 0;
	colorHeight = 0;
	pColorDescription->get_Width(&colorWidth); // 1920
	pColorDescription->get_Height(&colorHeight); // 1080

	colorBuffer.resize(colorWidth * colorHeight);


	hResult = pDepthSource->get_FrameDescription(&pDepthDescription);
	if (FAILED(hResult)){
		std::cerr << "Error : IDepthFrameSource::get_FrameDescription()" << std::endl;
		//return -1;
	}

	depthWidth = 0;
	depthHeight = 0;
	pDepthDescription->get_Width(&depthWidth); // 512
	pDepthDescription->get_Height(&depthHeight); // 424

	depthBuffer.resize(depthWidth * depthHeight);


	//入力画像を点群として扱う準備

	pointcloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
	
	pointcloud->width = static_cast<uint32_t>(depthWidth);
	pointcloud->height = static_cast<uint32_t>(depthHeight);
	pointcloud->is_dense = false;
	pointcloud->resize(pointcloud->width * pointcloud->height);


	hResult = pSensor->get_BodyIndexFrameSource(&pBodyIndexSource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_BodyIndexFrameSource()" << std::endl;
		//return -1;
	}

	// Reader

	hResult = pBodyIndexSource->OpenReader(&pBodyIndexReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IBodyIndexFrameSource::OpenReader()" << std::endl;
		//return -1;
	}

	// Description

	hResult = pBodyIndexSource->get_FrameDescription(&pBodyDescription);
	if (FAILED(hResult)){
		std::cerr << "Error : IBodyIndexFrameSource::get_FrameDescription()" << std::endl;
		//return -1;
	}

	hResult = pSensor->get_BodyFrameSource(&pBodySource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_BodyFrameSource()" << std::endl;
		//return -1;
	}


	hResult = pBodySource->OpenReader(&pBodyReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IBodyFrameSource::OpenReader()" << std::endl;
		//return -1;
	}


	updatePointcloud = false;

}

KinectV2::KinectV2(cv::Mat argDepthImg)
{
	// Create Sensor Instance

	hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)){
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
	}

	// Open Sensor
	hResult = pSensor->Open();
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
	}

	// Retrieved Coordinate Mapper

	hResult = pSensor->get_CoordinateMapper(&pCoordinateMapper);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_CoordinateMapper()" << std::endl;
	}

	// Retrieved Color Frame Source

	hResult = pSensor->get_ColorFrameSource(&pColorSource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
	}

	// Retrieved Depth Frame Source

	hResult = pSensor->get_DepthFrameSource(&pDepthSource);
	if (FAILED(hResult)){
		std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
	}

	// Open Color Frame Reader

	hResult = pColorSource->OpenReader(&pColorReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
	}

	// Open Depth Frame Reader

	hResult = pDepthSource->OpenReader(&pDepthReader);
	if (FAILED(hResult)){
		std::cerr << "Error : IDepthFrameSource::OpenReader()" << std::endl;
	}

	// Retrieved Color Frame Size

	hResult = pColorSource->get_FrameDescription(&pColorDescription);
	if (FAILED(hResult)){
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
	}

	colorWidth = 0;
	colorHeight = 0;
	pColorDescription->get_Width(&colorWidth); // 1920
	pColorDescription->get_Height(&colorHeight); // 1080

	// To Reserve Color Frame Buffer
	colorBuffer.resize(colorWidth * colorHeight);

	// Retrieved Depth Frame Size

	hResult = pDepthSource->get_FrameDescription(&pDepthDescription);
	if (FAILED(hResult)){
		std::cerr << "Error : IDepthFrameSource::get_FrameDescription()" << std::endl;
	}
	depthWidth = 0;
	depthHeight = 0;
	pDepthDescription->get_Width(&depthWidth); // 512
	pDepthDescription->get_Height(&depthHeight); // 424

	depthSpace.resize(colorWidth * colorHeight);	
	depthBuffer.resize(depthWidth * depthHeight);


	cameraSpacePointsInColorIndex = new CameraSpacePoint;

	cameraSpacePointsInDepthIndex = new CameraSpacePoint;

}

KinectV2::~KinectV2() //解放
{
	SafeRelease(pColorSource);
	SafeRelease(pDepthSource);
	SafeRelease(pColorReader);
	SafeRelease(pDepthReader);
	SafeRelease(pColorDescription);
	SafeRelease(pDepthDescription);
	SafeRelease(pCoordinateMapper);
	if (pSensor){
		pSensor->Close();
	}
	SafeRelease(pSensor);

}

int KinectV2::RenewColorFrame()
{
	// Acquire Latest Color Frame
	IColorFrame* pColorFrame = nullptr;
	hResult = pColorReader->AcquireLatestFrame(&pColorFrame);
	if (SUCCEEDED(hResult)){
		// Retrieved Color Data
		hResult = pColorFrame->CopyConvertedFrameDataToArray(colorBuffer.size() * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]), ColorImageFormat::ColorImageFormat_Bgra);
		if (FAILED(hResult)){
			std::cerr << "Error : IColorFrame::CopyConvertedFrameDataToArray()" << std::endl;
			return -1;
		}
	}
	SafeRelease(pColorFrame);

	colorImg = cv::Mat(cv::Size(colorWidth, colorHeight), CV_8UC4, reinterpret_cast<BYTE*>(&colorBuffer[0]));

	return 0;
}
int KinectV2::ShowColorImg()
{
	while (1) {
		RenewColorFrame();
		cv::imshow("colorimage", colorImg);
		cv::waitKey(1);
	}
	return 0;
}

int KinectV2::RenewDepthFrame()
{
	// Acquire Latest Depth Frame
	IDepthFrame* pDepthFrame = nullptr;
	hResult = pDepthReader->AcquireLatestFrame(&pDepthFrame);
	if (SUCCEEDED(hResult)){
		// Retrieved Depth Data
		hResult = pDepthFrame->CopyFrameDataToArray(depthBuffer.size(), &depthBuffer[0]);
		if (FAILED(hResult)){
			std::cerr << "Error : IDepthFrame::CopyFrameDataToArray()" << std::endl;
		}
	}
	SafeRelease(pDepthFrame);

	//////// ここを書き足す///////
	////////①クラス変数の設定→kinectV2.hのprivateに"depthImg"等の名前でMat型の変数を追加////////
	////////②カラー画像の取得関数を参考に完成させる(基本はcolorをdepthに変えるだけ)////
	depthImg = cv::Mat(cv::Size(depthWidth, depthHeight), CV_16UC1, reinterpret_cast<BYTE*>(&depthBuffer[0]));
	

	return 0;
}

int KinectV2::ShowDepthImg()
{
	while (1) {
		RenewDepthFrame();
		cv::imshow("depthimage", depthImg);
		cv::waitKey(1);
	}
	return 0;
}

//////ここにint KinectV2::ShowDepthImg()等の名前で, 距離画像を表示させる関数を作る/////
//////kinectV2.hにも関数の宣言をして置く必要がある//////


int KinectV2::RenewPointCloud()
{
	RenewColorFrame();
	RenewDepthFrame();
	updatePointcloud = true;


	for (int y = 0; y < depthHeight; y++){

		for (int x = 0; x < depthWidth; x++){
			pcl::PointXYZRGB point;

			DepthSpacePoint depthSpacePoint = { static_cast<float>(x), static_cast<float>(y) };
			UINT16 depth = depthBuffer[y * depthWidth + x];

			// Coordinate Mapping Depth to Color Space, and Setting PointCloud RGB
			ColorSpacePoint colorSpacePoint = { 0.0f, 0.0f };
			pCoordinateMapper->MapDepthPointToColorSpace(depthSpacePoint, depth, &colorSpacePoint);
			int colorX = static_cast<int>(std::floor(colorSpacePoint.X + 0.5f));
			int colorY = static_cast<int>(std::floor(colorSpacePoint.Y + 0.5f));
			if ((0 <= colorX) && (colorX < colorWidth) && (0 <= colorY) && (colorY < colorHeight)){
				RGBQUAD color = colorBuffer[colorY * colorWidth + colorX];
				point.b = color.rgbBlue;
				point.g = color.rgbGreen;
				point.r = color.rgbRed;
			}

			// Coordinate Mapping Depth to Camera Space, and Setting PointCloud XYZ
			CameraSpacePoint cameraSpacePoint = { 0.0f, 0.0f, 0.0f };
			pCoordinateMapper->MapDepthPointToCameraSpace(depthSpacePoint, depth, &cameraSpacePoint);
			if ((0 <= colorX) && (colorX < colorWidth) && (0 <= colorY) && (colorY < colorHeight)){
				point.x = cameraSpacePoint.X;
				point.y = cameraSpacePoint.Y;
				point.z = cameraSpacePoint.Z;

				pointcloud->points[y * depthWidth + x] = point;

			}
			else {
				pointcloud->points[y * depthWidth + x].x = std::numeric_limits<float>::quiet_NaN();
				pointcloud->points[y * depthWidth + x].y = std::numeric_limits<float>::quiet_NaN();
				pointcloud->points[y * depthWidth + x].z = std::numeric_limits<float>::quiet_NaN();
			}
		}
	}
	return 0;
}

int KinectV2::ShowPointCloud()
{
	pcl::visualization::PCLVisualizer viewer("Point Cloud Viewer");

	while (!viewer.wasStopped()){
		RenewPointCloud();
		pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> inputRGB(pointcloud);
		if (!viewer.updatePointCloud(pointcloud, inputRGB)) {
			viewer.addPointCloud(pointcloud, inputRGB);
		}
		viewer.spinOnce(100);
	}
	return 0;
}

int KinectV2::SaveData(){

	while(1){

		RenewPointCloud();

		static int i = 1;
		std::string colorName = "C:/Users/Demo/Pictures/color/color" + std::to_string(i) + ".jpg";
		std::string depthName = "C:/Users/Demo/Pictures/depth/depth" + std::to_string(i) + ".jpg";
		std::string pcdName = "C:/Users/Demo/Pictures/pcd/pcd" + std::to_string(i) + ".pcd";
		std::string depthXMLName = "C:/Users/Demo/Pictures/depthXML/depthXML" + std::to_string(i) + ".xml";
		i++;

		cv::imwrite(colorName, colorImg);
		cv::waitKey(1);

		cv::imwrite(depthName, depthImg);
		cv::waitKey(1);

		pcl::io::savePCDFileBinary(pcdName, *pointcloud);

		cv::FileStorage cvfs(depthXMLName, CV_STORAGE_WRITE);
		write(cvfs, "depthImg", depthImg);
		cvWaitKey(1);
	}
	return 0;

}

