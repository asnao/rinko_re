#include "ClickableImage.h"
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void main()
{
	Mat input[2], points[2], output;
	Mat epipolarPoint[2];

	String inputimgString1 = "./dropbox/right.jpg";
	String inputimgString2 = "./dropbox/left.jpg";

	input[0] = imread(inputimgString1);
	input[1] = imread(inputimgString2);

	ClickableImage c1 = ClickableImage(inputimgString1);
	ClickableImage c2 = ClickableImage(inputimgString2);

	//c1 c2...ClickableImageオブジェクト
	//points...画像の対応点
	//epipolar...エピポーラ線用の点
	clickLoop(&c1, &c2, &points[0], &points[1], &epipolarPoint[0], &epipolarPoint[1]);

	//平面射影変換行列を取得
	Mat homography = findHomography(points[0], points[1], CV_RANSAC);

	//変換
	Mat result;
	imwrite("./dropbox/result.jpg", result);
	warpPerspective(input[0], result, homography, Size(input[1].cols * 1.8, input[1].rows*1.8));
	imwrite("./dropbox/result2.jpg", result);

	//合成と表示
	input[1].copyTo(Mat(result, Rect(0, 0, input[1].cols, input[1].rows)));
	imshow("test", result);
	imwrite("./dropbox/result.jpg", result);

	waitKey(0);
}

/*
int main()
{
   // モザイキングする画像
  std::string fn1 = "./dropbox/right.jpg";//right
  std::string fn2 = "./dropbox/left.jpg";//left

  // 画像読み込み（グレースケールで読み込み）
  cv::Mat_< unsigned char > image1 = cv::imread( fn1, 0 );
  cv::Mat_< unsigned char > image2 = cv::imread( fn2, 0 );

  // 特徴点の検出
  cv::SurfFeatureDetector detector;
  std::vector< cv::KeyPoint > keys1, keys2;
  detector.detect( image1, keys1 );
  detector.detect( image2, keys2 );
 
  // 特徴量の記述
  cv::SurfDescriptorExtractor extractor;
  cv::Mat desp1, desp2;
  extractor.compute( image1, keys1, desp1 );
  extractor.compute( image2, keys2, desp2 );
 
  // 特徴点の対応付け
  std::vector< cv::DMatch > matches;
  cv::FlannBasedMatcher matcher;
  matcher.match( desp1, desp2, matches );
 
  // 対応点をstd::vectorに格納
  std::vector< cv::Vec2f > points1( matches.size() );
  std::vector< cv::Vec2f > points2( matches.size() );
 
  for( size_t i = 0 ; i < matches.size() ; ++i )
  {
    points1[ i ][ 0 ] = keys1[ matches[ i ].queryIdx ].pt.x;
    points1[ i ][ 1 ] = keys1[ matches[ i ].queryIdx ].pt.y;
 
    points2[ i ][ 0 ] = keys2[ matches[ i ].trainIdx ].pt.x;
    points2[ i ][ 1 ] = keys2[ matches[ i ].trainIdx ].pt.y;
  }

  // 画像間の平面射影変換行列を取得
  cv::Mat_< float > homography = cv::findHomography( points1, points2, CV_RANSAC );
 
  // 画像の読み込み（RGB画像で読み込み直し）
  cv::Mat_< cv::Vec3b > rgb_image1 = cv::imread( fn1, 1 );
  cv::Mat_< cv::Vec3b > rgb_image2 = cv::imread( fn2, 1 );
 
  // 画像1を画像2の空間に投影
  // 出力画像の大きさは適当に決定
  cv::Mat_< cv::Vec3b > result;
  cv::warpPerspective( rgb_image1, result, homography, cv::Size(rgb_image1.cols * 1.8 , rgb_image1.rows * 1.8 ) );
  cv::imwrite("./dropbox/result2.jpg", result); 
  cv::imshow( "Mosaicing2", result );

  // 画像2を結果画像にコピー
  // 合成方法は上書き
  for( int y = 0 ; y < rgb_image2.rows ; ++y )
  {
    for( int x = 0 ; x < rgb_image2.cols ; ++x )
    {
       result( y, x ) = rgb_image2( y, x );
    }
  }
 

  // モザイキング結果を表示
  cv::imwrite("./dropbox/result.jpg", result);
  cv::imshow( "Mosaicing", result );
  cv::waitKey();
  	return 0;
}
*/










	/*
   // モザイキングする画像
  std::string fn1 = "right.jpg";//right
  std::string fn2 = "left.jpg";//left

  // 画像読み込み（グレースケールで読み込み）
  cv::Mat_< unsigned char > image1 = cv::imread( fn1, 0 );
  cv::Mat_< unsigned char > image2 = cv::imread( fn2, 0 );

  // 特徴点の検出
  cv::SurfFeatureDetector detector;
  std::vector< cv::KeyPoint > keys1, keys2;
  detector.detect( image1, keys1 );
  detector.detect( image2, keys2 );
 
  // 特徴量の記述
  cv::SurfDescriptorExtractor extractor;
  cv::Mat desp1, desp2;
  extractor.compute( image1, keys1, desp1 );
  extractor.compute( image2, keys2, desp2 );
 
  // 特徴点の対応付け
  std::vector< cv::DMatch > matches;
  cv::FlannBasedMatcher matcher;
  matcher.match( desp1, desp2, matches );
 
  // 対応点をstd::vectorに格納
  std::vector< cv::Vec2f > points1( matches.size() );
  std::vector< cv::Vec2f > points2( matches.size() );
 
  for( size_t i = 0 ; i < matches.size() ; ++i )
  {
    points1[ i ][ 0 ] = keys1[ matches[ i ].queryIdx ].pt.x;
    points1[ i ][ 1 ] = keys1[ matches[ i ].queryIdx ].pt.y;
 
    points2[ i ][ 0 ] = keys2[ matches[ i ].trainIdx ].pt.x;
    points2[ i ][ 1 ] = keys2[ matches[ i ].trainIdx ].pt.y;
  }

  // 画像間の平面射影変換行列を取得
  cv::Mat_< float > homography = cv::findHomography( points1, points2, CV_RANSAC );
 
  // 画像の読み込み（RGB画像で読み込み直し）
  cv::Mat_< cv::Vec3b > rgb_image1 = cv::imread( fn1, 1 );
  cv::Mat_< cv::Vec3b > rgb_image2 = cv::imread( fn2, 1 );
 
  // 画像1を画像2の空間に投影
  // 出力画像の大きさは適当に決定
  cv::Mat_< cv::Vec3b > result;
  cv::warpPerspective( rgb_image1, result, homography, cv::Size(rgb_image1.cols * 8 , rgb_image1.rows * 8 ) );
 
  // 画像2を結果画像にコピー
  // 合成方法は上書き
  for( int y = 0 ; y < rgb_image2.rows ; ++y )
  {
    for( int x = 0 ; x < rgb_image2.cols ; ++x )
    {
      result( y, x ) = rgb_image2( y, x );
    }
  }
 

  // モザイキング結果を表示
  cv::imwrite("result.jpg", result);
  cv::imshow( "Mosaicing", result );
  cv::waitKey();
*/

/*
int main(){
  // モザイキングする画像
  std::string fn1 = "right.jpg";//right
  std::string fn2 = "center.jpg"; //center
  std::string fn3 = "left.jpg";//left
 
  // 画像読み込み（グレースケールで読み込み）
  cv::Mat_< unsigned char > image1 = cv::imread( fn1, 0 );
  cv::Mat_< unsigned char > image2 = cv::imread( fn2, 0 );
  cv::Mat_< unsigned char > image3 = cv::imread( fn3, 0 );
 
  // 特徴点の検出
  cv::SurfFeatureDetector detector;
  std::vector< cv::KeyPoint > keys1, keys2 , keys3;
  detector.detect( image1, keys1 );
  detector.detect( image2, keys2 );
  detector.detect( image3, keys3 );

  // 特徴量の記述
  cv::SurfDescriptorExtractor extractor;
  cv::Mat desp1, desp2, desp3;
  extractor.compute( image1, keys1, desp1 );
  extractor.compute( image2, keys2, desp2 );
  extractor.compute( image3, keys3, desp3 ); 
  // 特徴点の対応付け
  std::vector< cv::DMatch > matches1,matches2;
  cv::FlannBasedMatcher matcher1,matcher2;
  matcher1.match( desp1, desp2, matches1 );
  matcher2.match( desp1, desp2, matches2 );

  // 対応点をstd::vectorに格納
  std::vector< cv::Vec2f > points1( matches1.size() );
  std::vector< cv::Vec2f > points2( matches1.size() );
 
  for( size_t i = 0 ; i < matches1.size() ; ++i )
  {
    points1[ i ][ 0 ] = keys1[ matches1[ i ].queryIdx ].pt.x;
    points1[ i ][ 1 ] = keys1[ matches1[ i ].queryIdx ].pt.y;
 
    points2[ i ][ 0 ] = keys2[ matches1[ i ].trainIdx ].pt.x;
    points2[ i ][ 1 ] = keys2[ matches1[ i ].trainIdx ].pt.y;
  }

  std::vector< cv::Vec2f > points3( matches2.size() );
  std::vector< cv::Vec2f > points4( matches2.size() );
 
  for( size_t i = 0 ; i < matches2.size() ; ++i )
  {
    points3[ i ][ 0 ] = keys2[ matches2[ i ].queryIdx ].pt.x;
    points3[ i ][ 1 ] = keys2[ matches2[ i ].queryIdx ].pt.y;
 
    points4[ i ][ 0 ] = keys3[ matches2[ i ].trainIdx ].pt.x;
    points4[ i ][ 1 ] = keys3[ matches2[ i ].trainIdx ].pt.y;
  }

  // 画像間の平面射影変換行列を取得
  cv::Mat_< float > homography1 = cv::findHomography( points1, points2, CV_RANSAC );
  cv::Mat_< float > homography2 = cv::findHomography( points4, points3, CV_RANSAC );
 
  // 画像の読み込み（RGB画像で読み込み直し）
  cv::Mat_< cv::Vec3b > rgb_image1 = cv::imread( fn1, 1 );
  cv::Mat_< cv::Vec3b > rgb_image2 = cv::imread( fn2, 1 );
  cv::Mat_< cv::Vec3b > rgb_image3 = cv::imread( fn3, 1 );
 


  // 画像1を画像2の空間に投影
  // 出力画像の大きさは適当に決定
  cv::Mat_< cv::Vec3b > result1;
  cv::warpPerspective( rgb_image2, result1, homography1, cv::Size(rgb_image2.cols * 2 , rgb_image1.rows  ) );
 
  // 画像2を結果画像にコピー
  // 合成方法は上書き
  for( int y = 0 ; y < rgb_image2.rows ; ++y )
  {
    for( int x = 0 ; x < rgb_image2.cols ; ++x )
    {
      result( y, x ) = rgb_image2( y, x );
    }
  }
 

  // モザイキング結果を表示
  cv::imwrite("result.jpg", result);
  cv::imshow( "Mosaicing", result );
  cv::waitKey();
 
  return 0;


}

*/