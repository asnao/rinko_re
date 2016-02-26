#include "ClickableImage.h"

using namespace cv;

int main()
{
	
	// �E�B���h�E����t�@�C�����Ɋւ���p�����[�^
	const std::string						windowNameSrc = "Source";
	const std::string						windowNameUnd = "Undistorted Image";
	const std::string						fileNameCameraParameters = "cam_param.xml";

	// �`�F�b�J�[�p�^�[���Ɋւ���ϐ��ƃp�����[�^
	std::vector<cv::Mat>					checkerImgs;								// �`�F�b�J�[�p�^�[���摜
	const cv::Size							patternSize( 7, 5 );						// �`�F�b�J�[�p�^�[���̌�_�̐�
	float									sizeOfSquare = 30.0;						// �`�F�b�J�[�p�^�[���̃}�X�ڂ̃T�C�Y [mm]

	// ���W�Ɋւ���ϐ�



	// �J�����L�����u���[�V�����̃p�����[�^
	cv::TermCriteria						criteria( cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.001 );

	// �J�����p�����[�^�s��
	cv::Mat									cameraMatrix;		// �����p�����[�^�s��
	cv::Mat									distCoeffs(5,1,cv::DataType<float>::type);			// �����Y�c�ݍs��
	std::vector<cv::Mat>					rotationVectors;	// �B�e�摜���Ƃɓ������]�x�N�g��
	std::vector<cv::Mat>					translationVectors;	// �B�e�摜���Ƃɓ����镽�s�ړ��x�N�g��


	
	distCoeffs.at<float>(0,0) = 0; 
	distCoeffs.at<float>(1,0) = 0; 
	distCoeffs.at<float>(2,0) = 0;
	distCoeffs.at<float>(3,0) = 0;
	distCoeffs.at<float>(4,0) = 0;


	cv::VideoCapture cap(0); // �f�t�H���g�J�������I�[�v��
    if(!cap.isOpened())  // �����������ǂ������`�F�b�N
        return -1;

    cv::Mat input_image;


	
	std::vector<cv::Point3f>	objectPoints;		// �`�F�b�J�[��_���W�ƑΉ����鐢�E���W�̒l���i�[����s��
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
			std::vector<std::vector<cv::Point3f> >	worldPoints( 1 );		// �`�F�b�J�[��_���W�ƑΉ����鐢�E���W�̒l���i�[����s��
			std::vector<std::vector<cv::Point2f> >	imagePoints( 1 );		// �`�F�b�J�[��_���W���i�[����s��
			std::vector<cv::Point2f>	drawPoints;
		    cv::Mat frame;
		    cap >> frame; // �J��������V�����t���[�����擾
			input_image = frame;      //mat�̃R�s�[�͕��ʂ�=�Ō��ׂΗǂ��݂����D
			imshow("UNKO",input_image);
			cv::waitKey(10);

			if(cv::findChessboardCorners( input_image, patternSize, imagePoints[0]) == true){
				std::cout << " - ��_�����ׂČ��o���܂���" << std::endl;
				// ���o�_��`�悷��
			cv::drawChessboardCorners(input_image, patternSize, imagePoints[0], true );
			} else {
				continue;
			}
			cv::imshow( "dist", input_image );
			cv::waitKey(10);
		
				// ���E���W�ɂ�����`�F�b�J�[�p�^�[���̌�_���W������
		for( int k = 0; k < patternSize.area(); k++ ) {
			worldPoints[0].push_back( cv::Point3f(	static_cast<float>( k % patternSize.width * sizeOfSquare ),
													static_cast<float>( k / patternSize.width * sizeOfSquare ),
													0.0 ) );
		}



			cv::calibrateCamera(worldPoints, imagePoints, input_image.size(), cameraMatrix, distCoeffs, rotationVectors, translationVectors );
		
			// ��]�x�N�g���ƕ��s�ړ��x�N�g����4�s4��̊O���p�����[�^�s��ɏ�������
			// �Ȃ��A�����ł�Zhang�̃L�����u���[�V������@�ɂ�����1���ڂ̊O���p�����[�^�s��̂ݏo�͂���
/*			
			// �o�͐�ƂȂ�4�s4��̊O���p�����[�^�s��B�P�ʍs��ŏ�����
			cv::Mat	extrinsic( 4, 4, CV_64F );
			cv::setIdentity( extrinsic );
			// ��]�x�N�g������]�s��ɕϊ����A�O���p�����[�^�s��Ɋi�[
			cv::Rodrigues( rotationVectors[0], extrinsic( cv::Rect( 0, 0, 3, 3 ) ) );
			// ���s�ړ��x�N�g�����O���p�����[�^�s��Ɋi�[
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
			// Zhang�̎�@�ɂ��L�����u���[�V����
	return 0;
}

/*
	cv::Mat									inImg;

	const int								numberOfPoints = 8;				// �`�F�b�J�[�p�^�[�����������邩
	std::vector<cv::Point3f>	objectPoints;		// �`�F�b�J�[��_���W�ƑΉ����鐢�E���W�̒l���i�[����s��
	std::vector<cv::Point2f>	imagePoints;	

	cv::Mat									cameraMatrix(3,3,cv::DataType<float>::type);		// �����p�����[�^�s��A
	cv::Mat									distCoeffs(5,1,cv::DataType<float>::type);			// �����Y�c�ݍs��
	cv::Mat									rotationMatrix(3,3,cv::DataType<float>::type);		// ��]�s��R
	cv::Mat									translationMatrix(3,1,cv::DataType<float>::type);		// ���s�ړ��s��T
	
	std::vector<cv::Mat>					rotationVectors;	// �B�e�摜���Ƃɓ������]�x�N�g��R
	std::vector<cv::Mat>					translationVectors;	// �B�e�摜���Ƃɓ����镽�s�ړ��x�N�g��T

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