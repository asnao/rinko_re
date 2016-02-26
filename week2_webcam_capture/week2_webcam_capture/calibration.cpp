#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

int main( int argc, char** argv )
{
	// �E�B���h�E����t�@�C�����Ɋւ���p�����[�^
	const std::string						windowNameSrc = "Source";
	const std::string						windowNameUnd = "Undistorted Image";
	const std::string						fileNameCameraParameters = "cam_param.xml";

	// �`�F�b�J�[�p�^�[���Ɋւ���ϐ��ƃp�����[�^
	std::vector<cv::Mat>					checkerImgs;								// �`�F�b�J�[�p�^�[���摜
	const int								numberOfCheckerPatterns = 1;				// �`�F�b�J�[�p�^�[�����������邩
	const cv::Size							patternSize( 9, 6 );						// �`�F�b�J�[�p�^�[���̌�_�̐�
	float									sizeOfSquare = 10.0;						// �`�F�b�J�[�p�^�[���̃}�X�ڂ̃T�C�Y [mm]

	// ���W�Ɋւ���ϐ�
	std::vector<std::vector<cv::Point3f> >	worldPoints( numberOfCheckerPatterns );		// �`�F�b�J�[��_���W�ƑΉ����鐢�E���W�̒l���i�[����s��
	std::vector<std::vector<cv::Point2f> >	imagePoints( numberOfCheckerPatterns );		// �`�F�b�J�[��_���W���i�[����s��


	// �J�����L�����u���[�V�����̃p�����[�^
	cv::TermCriteria						criteria( cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.001 );

	// �J�����p�����[�^�s��
	cv::Mat									cameraMatrix;		// �����p�����[�^�s��
	cv::Mat									distCoeffs;			// �����Y�c�ݍs��
	std::vector<cv::Mat>					rotationVectors;	// �B�e�摜���Ƃɓ������]�x�N�g��
	std::vector<cv::Mat>					translationVectors;	// �B�e�摜���Ƃɓ����镽�s�ړ��x�N�g��


	cv::VideoCapture cap(0); // �f�t�H���g�J�������I�[�v��
    if(!cap.isOpened())  // �����������ǂ������`�F�b�N
        return -1;

    cv::Mat input_image;

    for(int j = 0;; j++)
    {
		for(int i = 0; i < numberOfCheckerPatterns; i++ ) {
		    cv::Mat frame;
		    cap >> frame; // �J��������V�����t���[�����擾
			input_image = frame;      //mat�̃R�s�[�͕��ʂ�=�Ō��ׂΗǂ��݂����D
	
	
			if(cv::findChessboardCorners( input_image, patternSize, imagePoints[i]) == true){
				std::cout << " - ��_�����ׂČ��o���܂���" << std::endl;
				// ���o�_��`�悷��
			cv::drawChessboardCorners(input_image, patternSize, imagePoints[i], true );
			}
			cv::imshow( "dist", input_image );
			cv::waitKey(10);
		
			// ���E���W�ɂ�����`�F�b�J�[�p�^�[���̌�_���W������
				for( int k = 0; k < patternSize.area(); k++ ) {
					worldPoints[i].push_back( cv::Point3f(	static_cast<float>( k % patternSize.width * sizeOfSquare ),
															static_cast<float>( k / patternSize.width * sizeOfSquare ),
															0.0 ) );
				}
			

		}
			// Zhang�̎�@�ɂ��L�����u���[�V����
			cv::calibrateCamera(worldPoints, imagePoints, input_image.size(), cameraMatrix, distCoeffs, rotationVectors, translationVectors );
		
			// ��]�x�N�g���ƕ��s�ړ��x�N�g����4�s4��̊O���p�����[�^�s��ɏ�������
			// �Ȃ��A�����ł�Zhang�̃L�����u���[�V������@�ɂ�����1���ڂ̊O���p�����[�^�s��̂ݏo�͂���
		
			// �o�͐�ƂȂ�4�s4��̊O���p�����[�^�s��B�P�ʍs��ŏ�����
			cv::Mat	extrinsic( 4, 4, CV_64F );
			cv::setIdentity( extrinsic );
			// ��]�x�N�g������]�s��ɕϊ����A�O���p�����[�^�s��Ɋi�[
			cv::Rodrigues( rotationVectors[0], extrinsic( cv::Rect( 0, 0, 3, 3 ) ) );
			// ���s�ړ��x�N�g�����O���p�����[�^�s��Ɋi�[
			translationVectors[j].copyTo( extrinsic( cv::Rect( 3, 0, 1, 3 ) ) );
			
			std::cout << "extrinsic = " << extrinsic << std::endl;
			std::cout << "intrinsic = " << cameraMatrix << std::endl;
			std::cout << "distcoeffs = " <<distCoeffs << std::endl;
			/*
			// �J�����p�����[�^���t�@�C���ɏ����o��
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
			
			// �c�ݕ␳�����摜��\��
			cv::Mat	undistorted;
			for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
				// �����Y�c�݌W����p���ĉ摜�̘c�݂�␳
				cv::undistort( checkerImgs[i], undistorted, cameraMatrix, distCoeffs );
				// �c�ݕ␳�O�ƕ␳��̉摜��\��
				cv::imshow( windowNameUnd, undistorted );
				cv::imshow( windowNameSrc, checkerImgs[i] );
				cv::waitKey( 0 );
*/
  
    }
    // VideoCapture �f�X�g���N�^�ɂ��C�J�����͎����I�ɏI����������܂�
    return 0;


/*
	// �`�F�b�J�[�p�^�[���摜���t�@�C������ǂݍ���
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		std::stringstream		stream;
		// �ʂ��ԍ�����t�@�C�����𐶐�
		stream << i + 1 << ".jpg";
		std::string	fileName = stream.str();
		// �ǂݍ��񂾉摜���x�N�g���ɕۑ�
		checkerImgs.push_back( cv::imread( fileName ) );
		std::cout << "�`�F�b�J�[�p�^�[���ǂݍ���: " << fileName << std::endl;
	}

	// �`�F�b�N�p�^�[���̌�_���W�����߁CimagePoints�Ɋi�[����
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		std::cout << i + 1 << "�Ԗڂ̉摜����`�F�b�J�[�p�^�[���̌�_�����o���܂�";
		if( cv::findChessboardCorners( checkerImgs[i], patternSize, imagePoints[i] ) ) {
			std::cout << " - ��_�����ׂČ��o���܂���" << std::endl;
			// ���o�_��`�悷��
			cv::drawChessboardCorners( checkerImgs[i], patternSize, imagePoints[i], true );
			cv::imshow( windowNameSrc, checkerImgs[i] );
			cv::waitKey( 500 );
			// �Q�l: �T�u�s�N�Z���P�ʂ܂ŃR�[�i�[���o���x���グ��ꍇ�͉��L�̃R�����g����������
			//cv::Mat	gray;
			//cv::cvtColor( checkerImgs[i], gray, cv::COLOR_BGR2GRAY );
			//cv::cornerSubPix( gray, imagePoints[i], cv::Size( 11, 11 ), cv::Size( -1, -1 ), criteria );
		} else {
			std::cout << " - ���o����Ȃ�������_������܂�" << std::endl;
			cv::waitKey( 0 );
			return -1;
		}
	}

	// ���E���W�ɂ�����`�F�b�J�[�p�^�[���̌�_���W������
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		for( int j = 0; j < patternSize.area(); j++ ) {
			worldPoints[i].push_back( cv::Point3f(	static_cast<float>( j % patternSize.width * sizeOfSquare ),
													static_cast<float>( j / patternSize.width * sizeOfSquare ),
													0.0 ) );
		}
	}

	// Zhang�̎�@�ɂ��L�����u���[�V����
	cv::calibrateCamera( worldPoints, imagePoints, checkerImgs[0].size(), cameraMatrix, distCoeffs, rotationVectors, translationVectors );

	// ��]�x�N�g���ƕ��s�ړ��x�N�g����4�s4��̊O���p�����[�^�s��ɏ�������
	// �Ȃ��A�����ł�Zhang�̃L�����u���[�V������@�ɂ�����1���ڂ̊O���p�����[�^�s��̂ݏo�͂���

	// �o�͐�ƂȂ�4�s4��̊O���p�����[�^�s��B�P�ʍs��ŏ�����
	cv::Mat	extrinsic( 4, 4, CV_64F );
	cv::setIdentity( extrinsic );
	// ��]�x�N�g������]�s��ɕϊ����A�O���p�����[�^�s��Ɋi�[
	cv::Rodrigues( rotationVectors[0], extrinsic( cv::Rect( 0, 0, 3, 3 ) ) );
	// ���s�ړ��x�N�g�����O���p�����[�^�s��Ɋi�[
	translationVectors[0].copyTo( extrinsic( cv::Rect( 3, 0, 1, 3 ) ) );

	// �J�����p�����[�^���t�@�C���ɏ����o��
	cv::FileStorage			fswrite( fileNameCameraParameters, cv::FileStorage::WRITE );
	if( fswrite.isOpened() ) {
		cv::write( fswrite, "extrinsic", extrinsic );
		cv::write( fswrite, "intrinsic", cameraMatrix );
		cv::write( fswrite, "distortion", distCoeffs );
	}

	// �c�ݕ␳�����摜��\��
	cv::Mat	undistorted;
	for( int i = 0; i < numberOfCheckerPatterns; i++ ) {
		// �����Y�c�݌W����p���ĉ摜�̘c�݂�␳
		cv::undistort( checkerImgs[i], undistorted, cameraMatrix, distCoeffs );
		// �c�ݕ␳�O�ƕ␳��̉摜��\��
		cv::imshow( windowNameUnd, undistorted );
		cv::imshow( windowNameSrc, checkerImgs[i] );
		cv::waitKey( 0 );
	}

	return 0;
	*/
	}