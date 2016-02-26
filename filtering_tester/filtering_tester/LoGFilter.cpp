/*
 *  LoGFilter.cpp
 *  xc_QtimageViewer
 *
 *  Created by JoseMiranda on 2/1/12.
 *  Copyright 2012 José Iguelmar Miranda. All rights reserved.
 *
 */

#include "LoGFilter.h"

LoGFilter::LoGFilter() {
  
}

cv::Mat LoGFilter::process(cv::Mat &a_image) {
  cv::Mat result;
  
  // Allocate if necessary
  result.create(a_image.size(), a_image.type());
  
  // Constroi kernel Gauss; todas entradas = 1/16
  cv::Mat kernelGauss(3, 3, CV_32F, cv::Scalar(0.0625));
  
  // Define matriz do filtro de Gauss
  kernelGauss.at<float>(0, 1) = 0.125;
  kernelGauss.at<float>(1, 0) = 0.125;
  kernelGauss.at<float>(1, 1) = 0.25;
  kernelGauss.at<float>(1, 2) = 0.125;
  kernelGauss.at<float>(2, 1) = 0.125;
    
  // Convolve imagem original com filtro de Gauss
  cv::filter2D(a_image, result, a_image.depth(), kernelGauss);

  // Constroi kernel Laplace; todas entradas = 0
  cv::Mat kernelLaplace(3, 3, CV_32F, cv::Scalar(0.0));
  
  // Define matriz do filtro de Laplace
  kernelLaplace.at<float>(0, 1) = -1.0;
  kernelLaplace.at<float>(1, 0) = -1.0;
  kernelLaplace.at<float>(1, 1) = +4.0;
  kernelLaplace.at<float>(1, 2) = -1.0;
  kernelLaplace.at<float>(2, 1) = -1.0;
  
  /*
   // versao negativa
   float matrizLaplace [] = {
   0f,  1f,  0f,
   1f, -4f,  1f,
   0f,  1f,  0f
   };
   */
  
  // Convolve imagem Gauss com filtro de Laplace
  cv::filter2D(result, result, a_image.depth(), kernelLaplace);
    
  return result;
}     

