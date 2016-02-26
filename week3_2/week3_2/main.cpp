#include "ClickableImage.h"

using namespace cv;

int main()
{
	
	// ウィンドウ名やファイル名に関するパラメータ
	const std::string						windowNameSrc = "Source";
	const std::string						windowNameUnd = "Undistorted Image";
	const std::string						fileNameCameraParameters = "cam_param.xml";

	// チェッカーパターンに関する変数とパラメータ
	std::vector<cv::Mat>					checkerImgs;								// チェッカーパターン画像
	const cv::Size							patternSize( 7, 5 );						// チェッカーパターンの交点の数
	float									sizeOfSquare = 30.0;						// チェッカーパターンのマス目のサイズ [mm]

	// 座標に関する変数



	// カメラキャリブレーションのパラメータ
	cv::TermCriteria						criteria( cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.001 );

	// カメラパラメータ行列
	cv::Mat									cameraMatrix;		// 内部パラメータ行列
	cv::Mat									distCoeffs(5,1,cv::DataType<float>::type);			// レンズ歪み行列
	std::vector<cv::Mat>					rotationVectors;	// 撮影画像ごとに得られる回転ベクトル
	std::vector<cv::Mat>					translationVectors;	// 撮影画像ごとに得られる平行移動ベクトル


	
	distCoeffs.at<float>(0,0) = 0; 
	distCoeffs.at<float>(1,0) = 0; 
	distCoeffs.at<float>(2,0) = 0;
	distCoeffs.at<float>(3,0) = 0;
	distCoeffs.at<float>(4,0) = 0;


	cv::VideoCapture cap(0); // デフォルトカメラをオープン
    if(!cap.isOpened())  // 成功したかどうかをチェック
        return -1;

    cv::Mat input_image;


	
	std::vector<cv::Point3f>	objectPoints;		// チェッカー交点座標と対応する世界座標の値を格納する行列
	objectPoints.push_back( cv::Point3f(	0.0, 0.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	0.0, 120.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, 120.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, 0.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	0.0, 0.0, -80.0 ));
	objectPoints.push_back( cv::Point3f(	0.0, 120.0, -80.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, 120.0, -80.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, 0.0, -80.0 ));



    for(;;)
    {
			std::vector<std::vector<cv::Point3f> >	worldPoints( 1 );		// チェッカー交点座標と対応する世界座標の値を格納する行列
			std::vector<std::vector<cv::Point2f> >	imagePoints( 1 );		// チェッカー交点座標を格納する行列
			std::vector<cv::Point2f>	drawPoints;
		    cv::Mat frame;
		    cap >> frame; // カメラから新しいフレームを取得
			input_image = frame;      //matのコピーは普通に=で結べば良いみたい．
			imshow("UNKO",input_image);
			cv::waitKey(10);

			if(cv::findChessboardCorners( input_image, patternSize, imagePoints[0]) == true){
				std::cout << " - 交点をすべて検出しました" << std::endl;
				// 検出点を描画する
			cv::drawChessboardCorners(input_image, patternSize, imagePoints[0], true );
			} else {
				continue;
			}
			cv::imshow( "dist", input_image );
			cv::waitKey(10);
		
				// 世界座標におけるチェッカーパターンの交点座標を決定
		for( int k = 0; k < patternSize.area(); k++ ) {
			worldPoints[0].push_back( cv::Point3f(	static_cast<float>( k % patternSize.width * sizeOfSquare ),
													static_cast<float>( k / patternSize.width * sizeOfSquare ),
													0.0 ) );
		}



			cv::calibrateCamera(worldPoints, imagePoints, input_image.size(), cameraMatrix, distCoeffs, rotationVectors, translationVectors );
		
			// 回転ベクトルと平行移動ベクトルを4行4列の外部パラメータ行列に書き換え
			// なお、ここではZhangのキャリブレーション手法における1枚目の外部パラメータ行列のみ出力する
/*			
			// 出力先となる4行4列の外部パラメータ行列。単位行列で初期化
			cv::Mat	extrinsic( 4, 4, CV_64F );
			cv::setIdentity( extrinsic );
			// 回転ベクトルを回転行列に変換し、外部パラメータ行列に格納
			cv::Rodrigues( rotationVectors[0], extrinsic( cv::Rect( 0, 0, 3, 3 ) ) );
			// 平行移動ベクトルを外部パラメータ行列に格納
			translationVectors[0].copyTo( extrinsic( cv::Rect( 3, 0, 1, 3 ) ) );
			
			std::cout << "extrinsic = " << extrinsic << std::endl;
			std::cout << "intrinsic = " << cameraMatrix << std::endl;
			std::cout << "distcoeffs = " <<distCoeffs << std::endl;
*/
			
			



	/*
	cv::Mat rotationMatrixR(3,1,cv::DataType<double>::type);//rodrigues rotation matrix 
	cv::Rodrigues(extrinsic,rotationMatrixR); 
	*/
	cv::projectPoints(objectPoints, rotationVectors[0], translationVectors[0], cameraMatrix, distCoeffs, drawPoints); 

	std::cout << drawPoints;



	cv::line(input_image,drawPoints[0],drawPoints[1],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[1],drawPoints[2],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[2],drawPoints[3],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[3],drawPoints[0],cv::Scalar(0,0,200),2);

	cv::line(input_image,drawPoints[0],drawPoints[4],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[1],drawPoints[5],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[2],drawPoints[6],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[3],drawPoints[7],cv::Scalar(0,0,200),2);

	cv::line(input_image,drawPoints[4],drawPoints[5],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[5],drawPoints[6],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[6],drawPoints[7],cv::Scalar(0,0,200),2);
	cv::line(input_image,drawPoints[7],drawPoints[4],cv::Scalar(0,0,200),2);

	cv::imshow("UNKO2",input_image);
	cv::waitKey(10);

	}
			// Zhangの手法によるキャリブレーション
	return 0;
}

/*
	cv::Mat									inImg;

	const int								numberOfPoints = 8;				// チェッカーパターンが何枚あるか
	std::vector<cv::Point3f>	objectPoints;		// チェッカー交点座標と対応する世界座標の値を格納する行列
	std::vector<cv::Point2f>	imagePoints;	

	cv::Mat									cameraMatrix(3,3,cv::DataType<float>::type);		// 内部パラメータ行列A
	cv::Mat									distCoeffs(5,1,cv::DataType<float>::type);			// レンズ歪み行列
	cv::Mat									rotationMatrix(3,3,cv::DataType<float>::type);		// 回転行列R
	cv::Mat									translationMatrix(3,1,cv::DataType<float>::type);		// 平行移動行列T
	
	std::vector<cv::Mat>					rotationVectors;	// 撮影画像ごとに得られる回転ベクトルR
	std::vector<cv::Mat>					translationVectors;	// 撮影画像ごとに得られる平行移動ベクトルT

	inImg = cv::imread( "p_sample.bmp" );

	objectPoints.push_back( cv::Point3f(	-180.0, -180.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	-180.0, 180.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, 180.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, -180.0, 0.0 ));
	objectPoints.push_back( cv::Point3f(	-180.0, -180.0, 80.0 ));
	objectPoints.push_back( cv::Point3f(	-180.0, 180.0, 80.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, 180.0, 80.0 ));
	objectPoints.push_back( cv::Point3f(	180.0, -180.0, 80.0 ));


	cameraMatrix.at<float>(0,0) = 380.8079; 
	cameraMatrix.at<float>(0,1) = 0.0;
	cameraMatrix.at<float>(0,2) = 165.0;
	cameraMatrix.at<float>(1,0) = 0.0;
	cameraMatrix.at<float>(1,1) = -422.44276;
	cameraMatrix.at<float>(1,2) = 142.0;
	cameraMatrix.at<float>(2,0) = 0.0;
	cameraMatrix.at<float>(2,1) = 0.0;
	cameraMatrix.at<float>(2,2) = 1.0;

	distCoeffs.at<float>(0,0) = 0; 
	distCoeffs.at<float>(1,0) = 0; 
	distCoeffs.at<float>(2,0) = 0;
	distCoeffs.at<float>(3,0) = 0;
	distCoeffs.at<float>(4,0) = 0;


	rotationMatrix.at<float>(0,0) = -0.999570; 
	rotationMatrix.at<float>(0,1) = -0.010079;
	rotationMatrix.at<float>(0,2) = 0.027543;
	rotationMatrix.at<float>(1,0) = 0.020762;
	rotationMatrix.at<float>(1,1) = 0.420135;
	rotationMatrix.at<float>(1,2) = 0.907224;
	rotationMatrix.at<float>(2,0) = -0.020715;
	rotationMatrix.at<float>(2,1) = 0.9071806;
	rotationMatrix.at<float>(2,2) = -0.419745; 

	translationMatrix.at<float>(0,0) = -3.517020; 
	translationMatrix.at<float>(1,0) = -14.776317; 
	translationMatrix.at<float>(2,0) = 296.088277;

	cv::Mat rotationMatrixR(3,1,cv::DataType<double>::type);//rodrigues rotation matrix 
	cv::Rodrigues(rotationMatrix,rotationMatrixR);   
	cv::projectPoints(objectPoints, rotationMatrixR, translationMatrix, cameraMatrix, distCoeffs, imagePoints); 

	std::cout << imagePoints;

	cv::waitKey(0);




	cv::line(inImg,imagePoints[0],imagePoints[1],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[1],imagePoints[2],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[2],imagePoints[3],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[3],imagePoints[0],cv::Scalar(0,0,200),2);

	cv::line(inImg,imagePoints[0],imagePoints[4],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[1],imagePoints[5],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[2],imagePoints[6],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[3],imagePoints[7],cv::Scalar(0,0,200),2);

	cv::line(inImg,imagePoints[4],imagePoints[5],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[5],imagePoints[6],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[6],imagePoints[7],cv::Scalar(0,0,200),2);
	cv::line(inImg,imagePoints[7],imagePoints[4],cv::Scalar(0,0,200),2);

	cv::imwrite( "p_sample2.bmp" ,inImg);


	return;



}
*/