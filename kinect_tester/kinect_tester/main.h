#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <Windows.h>
#include <Kinect.h>


#include <opencv2\opencv.hpp>
#include <pcl/point_types.h> 
#include <pcl/visualization/cloud_viewer.h> // opencvの後にinclude

#include <pcl/io/pcd_io.h>


#include <iostream>
#include <sstream>

////////////自分のOpenCVのバージョンに合わせて変更して下さい////////////

#if _DEBUG
#pragma comment(lib, "opencv_calib3d2410d.lib")
#pragma comment(lib, "opencv_contrib2410d.lib")
#pragma comment(lib, "opencv_core2410d.lib")
#pragma comment(lib, "opencv_features2d2410d.lib")
#pragma comment(lib, "opencv_flann2410d.lib")
#pragma comment(lib, "opencv_gpu2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib")
#pragma comment(lib, "opencv_imgproc2410d.lib")
#pragma comment(lib, "opencv_legacy2410d.lib")
#pragma comment(lib, "opencv_ml2410d.lib")
#pragma comment(lib, "opencv_nonfree2410d.lib")
#pragma comment(lib, "opencv_objdetect2410d.lib")
#pragma comment(lib, "opencv_ocl2410d.lib")
#pragma comment(lib, "opencv_photo2410d.lib")
#pragma comment(lib, "opencv_stitching2410d.lib")
#pragma comment(lib, "opencv_superres2410d.lib")
#pragma comment(lib, "opencv_ts2410d.lib")
#pragma comment(lib, "opencv_video2410d.lib")
#pragma comment(lib, "opencv_videostab2410d.lib")
#else
#pragma comment(lib, "opencv_calib3d2410.lib")
#pragma comment(lib, "opencv_contrib2410.lib")
#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_features2d2410.lib")
#pragma comment(lib, "opencv_flann2410.lib")
#pragma comment(lib, "opencv_gpu2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#pragma comment(lib, "opencv_imgproc2410.lib")
#pragma comment(lib, "opencv_legacy2410.lib")
#pragma comment(lib, "opencv_ml2410.lib")
#pragma comment(lib, "opencv_nonfree2410.lib")
#pragma comment(lib, "opencv_objdetect2410.lib")
#pragma comment(lib, "opencv_ocl2410.lib")
#pragma comment(lib, "opencv_photo2410.lib")
#pragma comment(lib, "opencv_stitching2410.lib")
#pragma comment(lib, "opencv_superres2410.lib")
#pragma comment(lib, "opencv_ts2410.lib")
#pragma comment(lib, "opencv_video2410.lib")
#pragma comment(lib, "opencv_videostab2410.lib")
#endif

#endif __MAIN_H__