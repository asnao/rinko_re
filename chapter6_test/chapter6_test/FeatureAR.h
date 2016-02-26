#pragma once


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <windows.h>
#include <cmath>

#include <GL/glew.h> // before including gl.h 
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib,"glew32.lib")

#include <opencv2/opencv.hpp>

#ifdef _DEBUG
// debug
#pragma comment(lib,"opencv_core300d.lib")    
#pragma comment(lib,"opencv_features2d300d.lib")   
#pragma comment(lib,"opencv_flann300d.lib")   
#pragma comment(lib,"opencv_videoio300d.lib")   
#pragma comment(lib,"opencv_calib3d300d.lib")   
#pragma comment(lib,"opencv_xfeatures2d300d.lib")   
#else
// release
#pragma comment(lib,"opencv_core300.lib")    
#pragma comment(lib,"opencv_features2d300.lib")   
#pragma comment(lib,"opencv_flann300.lib")   
#pragma comment(lib,"opencv_videoio300.lib")   
#pragma comment(lib,"opencv_calib3d300.lib")   
#pragma comment(lib,"opencv_xfeatures2d300.lib")    
#endif
