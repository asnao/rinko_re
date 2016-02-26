#include "FeatureAR.h"
#include "PoseEstimation.h"

int windowHeight = 480;
int windowWidth = 640;

std::string markerPath = "img/poster_print.bmp";
double markerSize[2] = { 297.0, 209.9 }; // �}�[�J�̑傫��[mm], �����A����
std::string intrinsicPath = "camera.xml";

GLfloat lightPos[] = { 200.0, 1000.0, -500.0, 1.0 };
GLfloat objColor[] = { 0.0, 0.0, 1.0, 1.0 };
GLuint texture[1];

cv::VideoCapture camera(0);	// �������J����ID�ɕύX
//cv::VideoCapture camera(1);	// �������J����ID�ɕύX

cv::Mat frame;

FeatureTracker *tracker;

/* OpenGL�A�C�h���֐� */
void Idle(){
	glutPostRedisplay();	// post display query
}

/* OpenGL�`��֐� */
void Display(void){

	/* �J�����L���v�`�� */
	camera >> frame;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/* �J�����ʒu�E�p������i�����_�̑Ή��t���ƊO���p�����[�^�̐���j */
	tracker->Track(frame);

	/* �`�� */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. �w�i���A�l�p���|���S���Ƀe�N�X�`���}�b�s���O
	// OpenCV��OpenGL��Y���̕������t�ł��邱�Ƃɒ���
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(1, 1);
	glVertex2d(1, -1);
	glTexCoord2d(1, 0);
	glVertex2d(1, 1);
	glTexCoord2d(0, 1);
	glVertex2d(-1, -1);
	glTexCoord2d(0, 0);
	glVertex2d(-1, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glClear(GL_DEPTH_BUFFER_BIT);    

	// 2. �}�[�J���������Ă���Ή��z���̂�`��
	if (tracker->isTracked()){
		
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// OpenCV�J�������W�n: y����������, z�����O����
		// OpenGL�J�������W�n: y���������, z����������
		gluLookAt(0, 0, 0, 0, 0, 1, 0, -1, 0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glPushMatrix();

		glMultMatrixd(tracker->getGLModelView());

		// ���E���W�n: �}�[�J���㌴�_ x�����E����, z����������� 
		glMaterialfv(GL_FRONT, GL_DIFFUSE, objColor);
		glTranslated(markerSize[1] / 2.0, markerSize[0] / 2.0, 0.0); //�@�}�[�J�̒����ɔz�u
		glRotated(90, 1, 0, 0);
		glutSolidTeapot(50.0);//��ӂ̒���

		glPopMatrix();

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
	}

	glutSwapBuffers();
}

/* OpenGL���T�C�Y�֐� */
void Resize(int w, int h){

	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// �����p�����[�^���琶�����ꂽ���e�s����|����
	tracker->getGLProjection(10.0, 100000.0);

	glMatrixMode(GL_MODELVIEW);
}

/* OpenGL�L�[�{�[�h�R�[���o�b�N�֐� */
void Keyboard(unsigned char key, int x, int y){
	switch (key) {
	// q �� ESC �������ƏI��
	case 'q':
	case '\033'://ESC
		exit(0);
		break;
	default:
		break;
	}
}

/* OpenGL�������֐� */
void Init(void){

	// �E�C���h�E�ݒ�
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Feature AR");

	// �R�[���o�b�N�֐��̓o�^
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Idle);

	// �w�i�`��p�e�N�X�`��
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texture[0]);	// �e�N�X�`��1��
	glBindTexture(GL_TEXTURE_2D, texture[0]);  // texture[0] 

	// �E�C���h�E������
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// �B�e�摜�p�o�b�t�@�̍쐬
	frame = cv::Mat(windowHeight, windowWidth, CV_8UC3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
}

/* main�֐� */
int main(int argc, char *argv[]){
	
	// �J�����`�F�b�N
	if (!camera.isOpened()){
		exit(-1);
	}

	// �g���b�J���� 
	tracker = new FeatureTracker();

	// �}�[�J�ǂݍ��� 
	tracker->LoadMarker(markerPath, markerSize[0], markerSize[1]);

	// �����p�����[�^�ǂݍ���
	tracker->LoadCameraIntrinsic(intrinsicPath);
		
	// OpenGL / GLUT �ݒ� 
	glutInit(&argc, argv);
	Init();
	glutMainLoop();

	return 0;
}