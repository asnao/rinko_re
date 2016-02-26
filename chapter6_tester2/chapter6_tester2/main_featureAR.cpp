#include "FeatureAR.h"
#include "PoseEstimation.h"

int windowHeight = 480;
int windowWidth = 640;

std::string markerPath = "img/poster_print.bmp";
double markerSize[2] = { 297.0, 209.9 }; // マーカの大きさ[mm], 高さ、横幅
std::string intrinsicPath = "camera.xml";

GLfloat lightPos[] = { 200.0, 1000.0, -500.0, 1.0 };
GLfloat objColor[] = { 0.0, 0.0, 1.0, 1.0 };
GLuint texture[1];

cv::VideoCapture camera(0);	// 引数をカメラIDに変更
//cv::VideoCapture camera(1);	// 引数をカメラIDに変更

cv::Mat frame;

FeatureTracker *tracker;

/* OpenGLアイドル関数 */
void Idle(){
	glutPostRedisplay();	// post display query
}

/* OpenGL描画関数 */
void Display(void){

	/* カメラキャプチャ */
	camera >> frame;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/* カメラ位置・姿勢推定（特徴点の対応付けと外部パラメータの推定） */
	tracker->Track(frame);

	/* 描画 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. 背景を、四角いポリゴンにテクスチャマッピング
	// OpenCVとOpenGLはY軸の方向が逆であることに注意
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

	// 2. マーカが見つかっていれば仮想物体を描画
	if (tracker->isTracked()){
		
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// OpenCVカメラ座標系: y軸が下向き, z軸が前向き
		// OpenGLカメラ座標系: y軸が上向き, z軸が後ろ向き
		gluLookAt(0, 0, 0, 0, 0, 1, 0, -1, 0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glPushMatrix();

		glMultMatrixd(tracker->getGLModelView());

		// 世界座標系: マーカ左上原点 x軸が右方向, z軸が鉛直上方 
		glMaterialfv(GL_FRONT, GL_DIFFUSE, objColor);
		glTranslated(markerSize[1] / 2.0, markerSize[0] / 2.0, 0.0); //　マーカの中央に配置
		glRotated(90, 1, 0, 0);
		glutSolidTeapot(50.0);//一辺の長さ

		glPopMatrix();

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
	}

	glutSwapBuffers();
}

/* OpenGLリサイズ関数 */
void Resize(int w, int h){

	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 内部パラメータから生成された投影行列を掛ける
	tracker->getGLProjection(10.0, 100000.0);

	glMatrixMode(GL_MODELVIEW);
}

/* OpenGLキーボードコールバック関数 */
void Keyboard(unsigned char key, int x, int y){
	switch (key) {
	// q か ESC を押すと終了
	case 'q':
	case '\033'://ESC
		exit(0);
		break;
	default:
		break;
	}
}

/* OpenGL初期化関数 */
void Init(void){

	// ウインドウ設定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Feature AR");

	// コールバック関数の登録
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Idle);

	// 背景描画用テクスチャ
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texture[0]);	// テクスチャ1枚
	glBindTexture(GL_TEXTURE_2D, texture[0]);  // texture[0] 

	// ウインドウ初期化
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// 撮影画像用バッファの作成
	frame = cv::Mat(windowHeight, windowWidth, CV_8UC3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, frame.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
}

/* main関数 */
int main(int argc, char *argv[]){
	
	// カメラチェック
	if (!camera.isOpened()){
		exit(-1);
	}

	// トラッカ生成 
	tracker = new FeatureTracker();

	// マーカ読み込み 
	tracker->LoadMarker(markerPath, markerSize[0], markerSize[1]);

	// 内部パラメータ読み込み
	tracker->LoadCameraIntrinsic(intrinsicPath);
		
	// OpenGL / GLUT 設定 
	glutInit(&argc, argv);
	Init();
	glutMainLoop();

	return 0;
}