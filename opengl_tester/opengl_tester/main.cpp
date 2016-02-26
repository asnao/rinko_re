#include <stdlib.h>
#include "GL/glut.h"
#include <iostream>

using namespace std;

void kadai(int argc, char *argv[]);
void init();
void SetLight();
void display();

//光源
void SetLight(){
	GLfloat light_ambient[] = {0.5f, 0.5f, 1.0f, 1.0f};
	GLfloat light_diffuse[] = {0.5f, 0.5f, 0.9f, 1.0f};
	GLfloat light_specular[] = {0.5f, 0.5f, 0.9f, 1.0f};
	GLfloat light0_position[] = {-100.0, 100.0, 100.0, 1.0};
	GLfloat spot0_direction[] = {0.0, -1.0, 0.0};
	GLfloat light1_position[] = {10.0, -10.0, -10.0, -1.0};
	GLfloat spot1_direction[] = {0.0, 1.0, 0.0};

	//Light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot0_direction);

	//Light1
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1_direction);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

//描画
void display(){

	//立方体の頂点座標を入力
	//立方体は1辺の長さが1.0f
	//原点は0.0
	static GLfloat vert[8][3]=
	{
		{ -1,-1,1}, //A
		{ 1,-1,1 }, //B
		{ 1,1,1 }, //C
		{ -1,1,1}, //D
		{ 1, -1, -1}, //E
		{ -1, -1, -1}, //F
		{ -1, 1, -1}, //G
		{ 1, 1, -1}  //H
	};

	glEnable(GL_LIGHTING);
	SetLight();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluPerspective(30.0,1.0,1.0,100.0);
	gluLookAt(3.0,4.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	glColor3d(0.0,1.0,0.0);
	glBegin(GL_LINE_LOOP);

	//下面の頂点の指定
	glVertex3fv(vert[1]);
	glVertex3fv(vert[0]);
	glVertex3fv(vert[5]);
	glVertex3fv(vert[4]);
	glEnd();

	//上面の頂点の指定
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vert[3]);
	glVertex3fv(vert[2]);
	glVertex3fv(vert[7]);
	glVertex3fv(vert[6]);
	glEnd();
	
	//奥面の頂点の指定
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vert[4]);
	glVertex3fv(vert[5]);
	glVertex3fv(vert[6]);
	glVertex3fv(vert[7]);
	glEnd();

	//手前面の頂点の指定
	glBegin(GL_LINE_LOOP);
	glVertex3fv(vert[0]);
	glVertex3fv(vert[1]);
	glVertex3fv(vert[2]);
	glVertex3fv(vert[3]);
	glEnd();

	//線の描画
	glFlush();
}

void kadai(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512,512);
	glutCreateWindow("graphic");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}

void init(){
	glClearColor(0.0,0.0,0.0,0.0);
}

int main(int argc, char *argv[]){
	kadai(argc, argv);	
}