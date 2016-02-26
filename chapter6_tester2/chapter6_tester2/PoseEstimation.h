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

/* 平面マーカを使ったカメラ位置・姿勢推定 */
class FeatureTracker{
public:
	double f;                           // 焦点距離[mm]
	double sx, sy;             // センササイズ
	double w, h;        // 撮影画像のサイズ
	double fovy, aspect;	// gluPerspectiveに入力するパラメータ
	
	cv::Mat intrinsic;	// 内部パラメータ行列
	cv::Mat distcoeffs;	//歪みパラメータ
	cv::Mat extrinsic;	// 外部パラメータ行列
	cv::Mat extrinsicGL;	// GL用外部パラメータ行列
	cv::Mat rvec;          // 回転ベクトル
	cv::Mat tvec;          // 平行移動ベクトル

	cv::Ptr<cv::Feature2D> detector;	// キーポイント・ディスクリプタの計算インスタンス
	
	cv::Ptr<cv::DescriptorMatcher> matcher;	// マッチングインスタンス

	cv::Mat marker;		// マーカ画像
	std::vector<cv::KeyPoint> markerKeypoint;	// マーカ画像のキーポイント
	cv::Mat markerDescriptor;	// マーカキーポイントのディスクリプタ
	double markerSize[2];	// マーカ画像の横幅・高さ [mm]

	bool tracked;	// カメラ位置・姿勢がきちんと求められているか（外れ値が多いとfalse）

	/* コンストラクタ1：ORBを使ったコンストラクタ */
	FeatureTracker(){
		tracked = false;

		// ORBのインスタンスを生成。ここを変更すると他の手法に切り替え可能
		detector = cv::ORB::create();

		// ここを変更すると距離尺度と探索手法を変更可能
		matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");

		extrinsic = cv::Mat::eye(4, 4, CV_64F);
	}

	/* コンストラクタ2：引数として任意のキーポイント/ディスクリプタ計算、マッチング手法を入力 */
	FeatureTracker(cv::Ptr<cv::Feature2D> _detector, cv::Ptr<cv::DescriptorMatcher> _matcher)
		:detector(_detector), matcher(_matcher){
		tracked = false;

		extrinsic = cv::Mat::eye(4, 4, CV_64F);
	}

	/* 読み込み済みのマーカ画像を登録 */
	void LoadMarker(cv::Mat markerImg, double widthMM, double heightMM){
		marker = markerImg;
		markerSize[0] = widthMM;
		markerSize[1] = heightMM;

		// マーカ画像のキーポイントとディスクリプタを抽出
		detector->detect(marker, markerKeypoint);
		detector->compute(marker, markerKeypoint, markerDescriptor);
	}

	/* マーカ画像を読み込み */
	void LoadMarker(std::string path, double heightMM, double widthMM){
		// マーカ画像のサイズを登録
		markerSize[0] = widthMM;
		markerSize[1] = heightMM;

		// マーカ画像のキーポイントとディスクリプタを抽出
		marker = cv::imread(path);	// 画像の読み込み
		detector->detect(marker, markerKeypoint);	// キーポイントの検出
		detector->compute(marker, markerKeypoint, markerDescriptor);	// ディスクリプタの計算
	}

	/* 内部パラメータを入力*/
	void LoadCameraIntrinsic(double f, double sx, double sy, double w, double h){
		intrinsic = cv::Mat::zeros(3, 3, CV_64F);   // 内部パラメータ
		intrinsic.at<double>(0, 0) = w*f / sx;	//      [ fx   0  cx ]
		intrinsic.at<double>(1, 1) = h*f / sy;		//      [  0  fy  cy ]
		intrinsic.at<double>(0, 2) = w / 2;		//      [  0   0   1  ]
		intrinsic.at<double>(1, 2) = h / 2;
		intrinsic.at<double>(2, 2) = 1;

		fovy = abs(atan(h / f) * 2.0) * 180.0 / M_PI;
		aspect = sx / sy;
	}

	/* ファイルから内部パラメータを読み込み*/
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

	/* 転置し、3x4行列を4x4行列に変換 */
	double *getGLModelView(){
		cv::Mat trans = cv::Mat::eye(4, 4, CV_64F);
		extrinsicGL = (trans*extrinsic).t();

		return extrinsicGL.ptr<double>();
	}

	/* マーカが見つかっているかどうか */
	bool isTracked(){
		return tracked;
	}

	/* OpenGL用内部パラメータ行列を生成 */
	void getGLProjection(double nearClip, double farClip){
		gluPerspective(fovy, aspect, nearClip, farClip);
	}

	/* 毎フレームの処理（カメラ位置・姿勢推定 */
	void Track(cv::Mat frame){

		/* 撮影画像のキーポイントとディスクリプタを抽出 */
		std::vector<cv::KeyPoint> frameKeypoint;	//キーポイント
		cv::Mat frameDescriptor;	// ディスクリプタ
		detector->detect(frame, frameKeypoint);	// キーポイントを検出
		detector->compute(frame, frameKeypoint, frameDescriptor);	// ディスクリプタを計算
		if (frameDescriptor.rows == 0) return;	// キーポイントがひとつも検出されなかった場合

		/* マッチング（最も類似する点とその次に類似する点の類似度の比から、信頼性の高い対応を残す） */
		// 最も類似する点と、その次に類似する点を算出
		std::vector<std::vector<cv::DMatch>> matches;	// 点の対応を格納（最類似点と2位の点）
		matcher->knnMatch(markerDescriptor, frameDescriptor, matches, 2);	// マーカと撮影画像のマッチング

		// 最も類似する点と、その次に類似する点の類似度の比から、信頼性の高い対応を残す
		std::vector<cv::KeyPoint> markerMatched, frameMatched;
		for (int i = 0; i < matches.size(); i++) {
			if (matches[i].size() < 2) continue;

			if (matches[i][0].distance < 0.8 * matches[i][1].distance) {
				markerMatched.push_back(markerKeypoint[matches[i][0].queryIdx]);
				frameMatched.push_back(frameKeypoint[matches[i][0].trainIdx]);
			}
		}	
		
		/* カメラ位置・姿勢推定 */
		// 対応点の登録
		std::vector<cv::Point3f> objectPoints;    // 世界座標上の3次元点群
		std::vector<cv::Point2f> imagePoints;    // 画像座標上の2次元点群

		for (int i = 0; i < markerMatched.size(); i++){
			// 平面マーカ上の3次元座標計算（世界座標系: マーカ左下原点、Z軸が鉛直上方向）
			cv::Point3f point3d;	// 3次元点
			point3d.x = markerMatched[i].pt.x*markerSize[0] / marker.cols;
			point3d.y = markerSize[1] - (markerMatched[i].pt.y*markerSize[1] / marker.rows);
			point3d.z = 0;
			objectPoints.push_back(point3d);

			// 画像上の2次元座標
			cv::Point2f point2d = frameMatched[i].pt;    // 2次元点
			imagePoints.push_back(point2d); 
		}

		// PnP問題を解き、回転・平行移動ベクトルを算出
		// intrinsic と distcoeffs は、カメラキャリブレーションにより求められた内部パラメータ行列とカメラのレンズ歪み
		cv::Mat inliers;
		if (objectPoints.size() == 0) return;	// 対応点がない場合
		
		if (rvec.empty()){
			// 最初のフレーム
			cv::solvePnPRansac(objectPoints, imagePoints, intrinsic, distcoeffs, rvec, tvec, false, 100, 8.0, 0.99/*, inliers, cv::SOLVEPNP_EPNP*/);
		}
		else{
			// 2フレーム目以降は、前フレームの回転・平行移動ベクトルを初期値とする
			cv::solvePnPRansac(objectPoints, imagePoints, intrinsic, distcoeffs, rvec, tvec, true, 100, 8.0, 0.99/*, inliers, cv::SOLVEPNP_EPNP*/);
		}

		// 外れ値でない点が10点未満なら、マーカが写っていないと判断
		if (double(inliers.rows) < 10){
			tracked = false;
		}
		else{
			tracked = true;
		}
	
		// 回転・平行移動ベクトルを外部パラメータ行列に変換
		cv::Mat rmat;         // 回転行列
		cv::Rodrigues(rvec, rmat);	// ロドリゲスの公式により、回転ベクトルを行列に変換
		
		// 座標変換行列を作成
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