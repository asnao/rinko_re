#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

int main( int argc, char** argv )
{
	// ウィンドウ名やファイル名に関するパラメータ
	const std::string						windowNameSrc = "Source";
	const std::string						windowNameUnd = "Undistorted Image";
	const std::string						fileNameCameraParameters = "cam_param.xml";

	// チェッカーパターンに関する変数とパラメータ
	std::vector<cv::Mat>					checkerImgs;								// チェッカーパターン画像
	const int								numberOfCheckerPatterns = 1;				// チェッカーパターンが何枚あるか
	const cv::Size							patternSize( 9, 6 );						// チェッカーパターンの交点の数
	float									sizeOfSquare = 10.0;						// チェッカーパターンのマス目のサイズ [mm]

	// 座標に関する変数
	std::vector<std::vector<cv::Point3f> >	worldPoints( numberOfCheckerPatterns );		// チェッカー交点座標と対応する世界座標の値を格納する行列
	std::vector<std::vector<cv::Point2f> >	imagePoints( numberOfCheckerPatterns );		// チェッカー交点座標を格納する行列


	// カメラキャリブレーションのパラメータ
	cv::TermCriteria						criteria( cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.001 );

	// カメラパラメータ行列
	cv::Mat									cameraMatrix;		// 内部パラメータ行列
	cv::Mat									distCoeffs;			// レンズ歪み行列
	std::vector<cv::Mat>					rotationVectors;	// 撮影画像ごとに得られる回転ベクトル
	std::vector<cv::Mat>					translationVectors;	// 撮影画像ごとに得られる平行移動ベクトル


	cv::VideoCapture cap(0); // デフォルトカメラをオープン
    if(!cap.isOpened())  // 成功したかどうかをチェック
        return -1;

    cv::Mat input_image;

    for(int j = 0;; j++)
    {
		for(int i = 0; i < numberOfCheckerPatterns; i++ ) {
		    cv::Mat frame;
		    cap >> frame; // カメラから新しいフレームを取得
			input_image = frame;      //matのコピーは普通に=で結べば良いみたい．
	
	
			if(cv::findChessboardCorners( input_image, patternSize, imagePoints[i]) == true){
				std::cout << " - 交点をすべて検出しました" << std::endl;
				// 検出点を描画する
			cv::drawChessboardCorners(input_image, patternSize, imagePoints[i], true );
			}
			cv::imshow( "dist", input_image );
			cv::waitKey(10);
		
			// 世界座標におけるチェッカーパターンの交点座標を決定
				for( int k = 0; k < patternSize.area(); k++ ) {
					worldPoints[i].push_back( cv::Point3f(	static_cast<float>( k % patternSize.width * sizeOfSquare ),
															static_cast<float>( k / patternSize.width * sizeOfSquare ),
															0.0 ) );
				}
			

		}
			// Zhangの手法によるキャリブレーション
			cv::calibrateCamera(worldPoints, imagePoints, input_image.size(), cameraMatrix, distCoeffs, rotationVectors, translationVectors );
		
			// 回転ベクトルと平行移動ベクトルを4行4列の外部パラメータ行列に書き換え
			// なお、ここではZhangのキャリブレーション手法における1枚目の外部パラメータ行列のみ出力する
		
			// 出力先となる4行4列の外部パラメータ行列。単位行列で初期化
			cv::Mat	extrinsic( 4, 4, CV_64F );
			cv::setIdentity( extrinsic );
			// 回転ベクトルを回転行列に変換し、外部パラメータ行列に格納
			cv::Rodrigues( rotationVectors[0], extrinsic( cv::Rect( 0, 0, 3, 3 ) ) );
			// 平行移動ベクトルを外部パラメータ行列に格納
			translationVectors[j].copyTo( extrinsic( cv::Rect( 3, 0, 1, 3 ) ) );
			
			std::cout << "extrinsic = " << extrinsic << std::endl;
			std::cout << "intrinsic = " << cameraMatrix << std::endl;
			std::cout << "distcoeffs = " <<distCoeffs << std::endl;
			/*
			// カメラパラメータをファイルに書き出す
			cv::FileStorage			fswrite( fileNameCameraParameters, cv::FileStorage::WRITE );

			std::string extrinsic = "C:/Users/Demo/Pictures/extrinsic/extrinsic" + std::to_string(j) + ".jpg";
			std::string intrinsic = "C:/Users/Demo/Pictures/intrinsic/intrinsic" + std::to_string(j) + ".jpg";
			std::string distortion = "C:/Users/Demo/Pictures/pcd/pcd" + std::to_string(j) + ".pcd";
			std::string depthXMLName = "C:/Users/Demo/Pictures/depthXML/depthXML" + std::to_string(j) + ".xml";


			if( fswrite.isOpened() ) {
				cv::write( fswrite, "extrinsic", extrinsic );	
				cv::write( fswrite, "intrinsic", cameraMatrix );
				cv::write( fswrite, "distortion", distCoeffs );
			}
			
			// 歪み補正した画像を表示
			cv::Mat	undistorted;
			for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
				// レンズ歪み係数を用いて画像の歪みを補正
				cv::undistort( checkerImgs[i], undistorted, cameraMatrix, distCoeffs );
				// 歪み補正前と補正後の画像を表示
				cv::imshow( windowNameUnd, undistorted );
				cv::imshow( windowNameSrc, checkerImgs[i] );
				cv::waitKey( 0 );
*/
  
    }
    // VideoCapture デストラクタにより，カメラは自動的に終了処理されます
    return 0;


/*
	// チェッカーパターン画像をファイルから読み込む
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		std::stringstream		stream;
		// 通し番号からファイル名を生成
		stream << i + 1 << ".jpg";
		std::string	fileName = stream.str();
		// 読み込んだ画像をベクトルに保存
		checkerImgs.push_back( cv::imread( fileName ) );
		std::cout << "チェッカーパターン読み込み: " << fileName << std::endl;
	}

	// チェックパターンの交点座標を求め，imagePointsに格納する
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		std::cout << i + 1 << "番目の画像からチェッカーパターンの交点を検出します";
		if( cv::findChessboardCorners( checkerImgs[i], patternSize, imagePoints[i] ) ) {
			std::cout << " - 交点をすべて検出しました" << std::endl;
			// 検出点を描画する
			cv::drawChessboardCorners( checkerImgs[i], patternSize, imagePoints[i], true );
			cv::imshow( windowNameSrc, checkerImgs[i] );
			cv::waitKey( 500 );
			// 参考: サブピクセル単位までコーナー検出精度を上げる場合は下記のコメントを解除する
			//cv::Mat	gray;
			//cv::cvtColor( checkerImgs[i], gray, cv::COLOR_BGR2GRAY );
			//cv::cornerSubPix( gray, imagePoints[i], cv::Size( 11, 11 ), cv::Size( -1, -1 ), criteria );
		} else {
			std::cout << " - 検出されなかった交点があります" << std::endl;
			cv::waitKey( 0 );
			return -1;
		}
	}

	// 世界座標におけるチェッカーパターンの交点座標を決定
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		for( int j = 0; j < patternSize.area(); j++ ) {
			worldPoints[i].push_back( cv::Point3f(	static_cast<float>( j % patternSize.width * sizeOfSquare ),
													static_cast<float>( j / patternSize.width * sizeOfSquare ),
													0.0 ) );
		}
	}

	// Zhangの手法によるキャリブレーション
	cv::calibrateCamera( worldPoints, imagePoints, checkerImgs[0].size(), cameraMatrix, distCoeffs, rotationVectors, translationVectors );

	// 回転ベクトルと平行移動ベクトルを4行4列の外部パラメータ行列に書き換え
	// なお、ここではZhangのキャリブレーション手法における1枚目の外部パラメータ行列のみ出力する

	// 出力先となる4行4列の外部パラメータ行列。単位行列で初期化
	cv::Mat	extrinsic( 4, 4, CV_64F );
	cv::setIdentity( extrinsic );
	// 回転ベクトルを回転行列に変換し、外部パラメータ行列に格納
	cv::Rodrigues( rotationVectors[0], extrinsic( cv::Rect( 0, 0, 3, 3 ) ) );
	// 平行移動ベクトルを外部パラメータ行列に格納
	translationVectors[0].copyTo( extrinsic( cv::Rect( 3, 0, 1, 3 ) ) );

	// カメラパラメータをファイルに書き出す
	cv::FileStorage			fswrite( fileNameCameraParameters, cv::FileStorage::WRITE );
	if( fswrite.isOpened() ) {
		cv::write( fswrite, "extrinsic", extrinsic );
		cv::write( fswrite, "intrinsic", cameraMatrix );
		cv::write( fswrite, "distortion", distCoeffs );
	}

	// 歪み補正した画像を表示
	cv::Mat	undistorted;
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		// レンズ歪み係数を用いて画像の歪みを補正
		cv::undistort( checkerImgs[i], undistorted, cameraMatrix, distCoeffs );
		// 歪み補正前と補正後の画像を表示
		cv::imshow( windowNameUnd, undistorted );
		cv::imshow( windowNameSrc, checkerImgs[i] );
		cv::waitKey( 0 );
	}

	return 0;
	*/
	}