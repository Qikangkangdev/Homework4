// Homework3.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include<cstdlib>
#include "glut.h"
#include "cvector217.h"
#include "cmatrix217.h"
#include "ceuler217.h"
#include "cquaternion217.h"
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
#define FRAME 25
using namespace std;
#pragma warning(disable : 4996)

GLuint texSky;
GLuint tex1;
GLuint tex2;
GLuint tex3;
GLuint tex4;
GLuint tex5;
GLuint tex6;
GLuint texSkin;
#define BMP_Header_Length 54
GLuint load_texture(const char* file_name);
void reshape(int w, int h);
void myDisplay(void);
float setax = 0.0;
float setay = 0.0;
void myTimerFunc(int val);
float clocation[100][3];
float ccolor[100][3];
float cvector[100][3], cvector1[100][3];
float csize[100];
void myKeyboardUpFunc(unsigned char key, int x, int y);
void myKeyboardFunc(unsigned char key, int x, int y);
void myMouseFunc(int button, int state, int x, int y);
void mySpecialFunc(GLint key, GLint x, GLint y);
void mySpecialUpFunc(GLint key, GLint x, GLint y);
void SetView();
void init();
void RenderWorld();
void KeyboardFunc();
void Font2D(const char * str, double x, double y);
void exit1();
void SpeedNormalize(float *a);
CVector217 GenerateRay(float x, float y);
int key_a, key_d, key_w, key_s, key_q, key_e, key_j, key_l, key_i, key_esc, key_greater, key_less;
int key_k, key_u, key_o, key_lmp, key_rmp, key_lbp, key_rbp, mode = 1, key_1, key_2;
int key_up, key_down, key_left, key_right, key_pup, key_pdown;
CEuler217 Euler, tempe, GodViewModeE, tempe1;
float mspeed = 1, rspeed = 1;
float g_IEyeMat[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
CVector217 rotatev, translatev, tempv, tempv1, GodViewTranslatev, tempv2;
CQuaternion217 temparrq[200];
CMatrix217 EyeMat, IEyeMat(g_IEyeMat), temp, EulerTempM, GodViewModeM, GodViewModeEulerTempM,temp1, tempm1(g_IEyeMat);
CQuaternion217 tempq, tempq1;
float OGLMANAGER_WIDTH, OGLMANAGER_HEIGHT;
CVector217 ray, temp_trans, GodViewModeT;
int choose = -1, bind = -1, choose1 = -1, frame;
int toBind;
float t[FRAME];
int lock;
float speed, ratio;
float SpeedRatio = 0.5, SpeedRatio1 = 1;
int bnum = 100;

int main(int argc, char * argv[])
{
	t[0] = 1.0 / FRAME;
	for (int i = 1; i < FRAME; i++) {
		t[i] = (float)(i + 1.0) * t[0];
	}

	// 进行图形绘制操作
	default_random_engine e, f;
	uniform_real_distribution<float> location(-47, 47);
	uniform_real_distribution<float> size(2, 3);
	uniform_real_distribution<float> color(0, 1);
	uniform_real_distribution<float> vector(0.0, 1.0);
	e.seed(time(0));
	f.seed(time(0) + 1);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 3; j++) {
			clocation[i][j] = location(e);
			ccolor[i][j] = color(e);
			cvector[i][j] = vector(e);
		}
		csize[i] = size(f);
	}
	for (int i = 0; i < 100; i++) {
		SpeedNormalize(cvector[i]);
		for (int j = 0; j < 3; j++) {
			cvector1[i][j] = cvector[i][j];
		}
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
	glEnable(GL_DEPTH_TEST);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("作业四");
	glEnable(GL_TEXTURE_2D);
	texSkin = load_texture("skin.bmp");
	tex1 = load_texture("1.bmp");
	tex2 = load_texture("2.bmp");
	tex3 = load_texture("3.bmp");
	tex4 = load_texture("4.bmp");
	tex5 = load_texture("5.bmp");
	tex6 = load_texture("6.bmp");
	init();
	glutTimerFunc(20, myTimerFunc, 0);
	glutReshapeFunc(&reshape);
	glutKeyboardUpFunc(&myKeyboardUpFunc);
	glutKeyboardFunc(&myKeyboardFunc);
	glutMouseFunc(&myMouseFunc);
	glutSpecialUpFunc(&mySpecialUpFunc);
	glutSpecialFunc(&mySpecialFunc);
	glutDisplayFunc(&myDisplay);
	glutMainLoop();
	system("pause");
	return 0;
}

// 初始化
void init() {

	translatev.Set(120, 40, 200);
	rotatev.Set(0, 1, 0);
	glLoadIdentity();

	// 初始化视图矩阵
	temp.SetRotate(30.0, rotatev);
	EyeMat = IEyeMat * temp.GetInverse();
	temp.SetTrans(translatev);
	EyeMat = EyeMat * temp.GetInverse();
	
	// 初始化欧拉角
	Euler = EyeMat.GetInverse().ToEuler();
	Euler.Normal();
}

// 刷新函数
void reshape(int w, int h) {
	OGLMANAGER_WIDTH = w;
	OGLMANAGER_HEIGHT = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, double(w) / double(h), 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// 绘制世界
void RenderWorld() {
	glLineWidth(1.0f);
	CMatrix217 rotateM;
	CVector217 rotateV;
	CMatrix217 transM;
	CVector217 transV;

	// 设置光源颜色 , 黑色 
	float blackColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float whiteColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 设置环境光 
	glLightfv(GL_LIGHT0, GL_AMBIENT, blackColor);

	// 设置漫反射光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteColor);

	// 设置镜面反射光 
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteColor);

	// 设置光源位置 , 最后一位设置成 0 代表该光源无限远
	float lightPosition[] = { 50.0f, 50.0f, 0.0f, 1.0f };

	// 设置光源位置 
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//GL_LIGHT1
	glLightfv(GL_LIGHT1, GL_AMBIENT, blackColor);

	// 设置漫反射光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteColor);

	// 设置镜面反射光 
	glLightfv(GL_LIGHT1, GL_SPECULAR, whiteColor);

	// 设置光源位置 , 最后一位设置成 0 代表该光源无限远
	float lightPosition1[] = { 0.0f, 50.0f, 50.0f, 1.0f };

	// 设置光源位置 
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

	// 设置材质
	float blackMat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float whiteMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float Mat1[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	// 设置环境光反射材质
	glMaterialfv(GL_FRONT, GL_AMBIENT, Mat1);

	// 设置漫反射光反射材质 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Mat1);

	// 设置镜面反射光反射材质 
	glMaterialfv(GL_FRONT, GL_SPECULAR, blackMat);

	// 启用光照
	glEnable(GL_LIGHTING);

	// 设置光源 , 0 号光源使用的是默认材质
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// 画坐标轴
	glPushMatrix();

	// 画直线
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(120.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 120.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 120.0f);
	glEnd();

	// 画X箭头
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	transV.Set(130.0f, 0.0f, 0.0f);
	transM.SetTrans(transV);
	glMultMatrixf(transM);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, 10.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, -10.0f, 0.0f);
	glEnd();
	glPopMatrix();

	// 画Y箭头
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	transV.Set(0.0f, 130.0f, 0.0f);
	transM.SetTrans(transV);
	glMultMatrixf(transM);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, -10.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, -10.0f, 0.0f);
	glEnd();
	glPopMatrix();

	// 画Z箭头
	glPushMatrix();
	transV.Set(0.0f, 0.0f, 130.0f);
	transM.SetTrans(transV);
	glMultMatrixf(transM);
	glLineWidth(2);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, -10.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, 0.0f, -10.0f);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	// 坐标轴绘制完毕

	// 绘制立方体
	glLineWidth(1);
	glPushMatrix();
	transV.Set(-50.0f, -50.0f, -50.0f);
	transM.SetTrans(transV);
	glMultMatrixf(transM);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex2);

	// 绘制顶面
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 100.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 100.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, 0.0f);
	glEnd();

	// 绘制底面
	glBindTexture(GL_TEXTURE_2D, tex1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glEnd();

	// 绘制左面
	glBindTexture(GL_TEXTURE_2D, tex5);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 100.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 100.0f, 100.0f);
	glEnd();

	// 绘制右面
	glBindTexture(GL_TEXTURE_2D, tex4);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(100.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100.0f, 100.0f, 0.0f);
	glEnd();

	// 绘制前面
	glBindTexture(GL_TEXTURE_2D, tex3);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100.0f, 100.0f, 100.0f);
	glEnd();

	// 绘制后面
	glBindTexture(GL_TEXTURE_2D, tex6);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 100.0f, 0.0f);
	glEnd();

	glPopMatrix();
	// 立方体绘制完毕
	glDisable(GL_TEXTURE_2D);

	// 绘制气泡
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	for (int i = 0; i < 100; i++) {
		glPushMatrix();
		transV.Set(clocation[i][0], clocation[i][1], clocation[i][2]);
		transM.SetTrans(transV);
		glMultMatrixf(transM);
		rotatev.Set(cvector[i][0], cvector[i][1], cvector[i][2]);
		tempe = rotatev.ToEuler();
		glMultMatrixf(tempe.ToMatrix());
		GLUquadricObj *quadObj = gluNewQuadric();//创建一个二次曲面物体
		gluQuadricTexture(quadObj, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, texSkin);
		glutSolidSphere(csize[i], 100, 100);
		gluDeleteQuadric(quadObj);
		glPopMatrix();
	}
	// 球体绘制完毕

	// 绘制字体
	stringstream ss1, ss2;
	if (lock != 1) {
		ss1 << "Current bubble's size: " << "Unlock";
	}
	else {
		ss1 << "Current bubble's size: " << csize[choose1];
	}
	ss2 << "Current remaining bubbles' number: " << bnum;
	string str1 = ss1.str();
	string str2 = ss2.str();
	const char * s1 = str1.data();
	const char * s2 = str2.data();
	Font2D(s1, -0.9, 0.9);
	Font2D(s2, -0.9, 0.8);
}

// 设置矩阵
void SetView() {
	if (toBind == 1 && frame < FRAME) {
		translatev = translatev + tempv1;
		if (mode == 2) {
			temp.SetTrans(translatev);
			temp = temp.GetInverse();
			EyeMat = temparrq[frame].ToMatrix().GetInverse() * temp;
		}
		else {
			Euler = temparrq[frame].ToEuler();
		}
		frame++;
		if (frame == FRAME) {
			toBind = 0;
			frame = 0;
		}
	}

	if (bind != -1 && toBind == 0) {
		tempv.Set(cvector[bind][0], cvector[bind][1], cvector[bind][2]);
		temp.SetTrans(translatev);
		tempe = tempv.ToEuler();
		temp = temp.GetInverse();
		if (mode == 2) {
			EyeMat = tempe.ToMatrix().GetInverse() * temp;
			EyeMat = tempm1 * EyeMat;
		}
		else {
			Euler = (tempe.ToMatrix() * tempe1.ToMatrix()).ToEuler();
		}
	}

	if (mode == 2) {
		glLoadMatrixf(EyeMat);
	}
	else {
		EulerTempM.SetTrans(translatev);
		EulerTempM = EulerTempM.GetInverse();
		glMultMatrixf(Euler.ToMatrix().GetInverse() * EulerTempM);
	}
}

// 绘制函数
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	SetView();
	RenderWorld();
	glPopMatrix();
	glFlush();
}

// 计时器函数
void myTimerFunc(int val) {
	exit1();
	if (fabs(cvector[0][0] - cvector1[0][0] * SpeedRatio) >= 1e-5 || cvector[0][0] == 0.0) {
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				cvector[i][j] = cvector1[i][j] * SpeedRatio;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		cvector[choose1][i] = cvector1[choose1][i] * SpeedRatio * SpeedRatio1;
	}
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 3; j++) {
			clocation[i][j] += cvector[i][j];
			if (clocation[i][j] + csize[i] > 50.0) {
				clocation[i][j] = 50.0 - csize[i];
				cvector1[i][j] *= -1.0;
				cvector[i][j] *= -1.0;
			}
			else if (clocation[i][j] - csize[i] < -50.0) {
				clocation[i][j] = -50.0 + csize[i];
				cvector1[i][j] *= -1.0;
				cvector[i][j] *= -1.0;
			}
		}
	}
	if (bind != -1) {
		translatev[0] += cvector[bind][0];
		translatev[1] += cvector[bind][1];
		translatev[2] += cvector[bind][2];
	}
	KeyboardFunc();
	myDisplay();
	glutTimerFunc(20, myTimerFunc, 0);
}

// 键盘抬起函数
void myKeyboardUpFunc(unsigned char key, int x, int y) {\
	cout << key << " Up" << endl;
	switch (key) {
	case 'a':
		key_a = 0;
		break;
	case 'd':
		key_d = 0;
		break;
	case 'w':
		key_w = 0;
		break;
	case 's':
		key_s = 0;
		break;
	case 'q':
		key_q = 0;
		break;
	case 'e':
		key_e = 0;
		break;
	case 'j':
		key_j = 0;
		break;
	case 'l':
		key_l = 0;
		break;
	case 'i':
		key_i = 0;
		break;
	case 'k':
		key_k = 0;
		break;
	case 'u':
		key_u = 0;
		break;
	case 'o':
		key_o = 0;
		break;
	case '[':
		key_lmp = 0;
		break;
	case ']':
		key_rmp = 0;
		break;
	case '{':
		key_lbp = 0;
		break;
	case '}':
		key_rbp = 0;
		break;
	case '1':
		key_1 = 0;
		break;
	case '2':
		key_2 = 0;
		break;
	case 27:
		key_esc = 0;
		break;
	case '>':
		key_greater = 0;
		break;
	case '<':
		key_less = 0;
		break;
	}
}

void mySpecialUpFunc(GLint key, GLint x, GLint y) {
	cout << key << " Up" << endl;
	if (key == GLUT_KEY_UP)
	{
		key_up = 0;
	}
	if (key == GLUT_KEY_DOWN) {
		key_down = 0;
	}
	if (key == GLUT_KEY_LEFT)
	{
		key_left = 0;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		key_right = 0;
	}
	if (key == GLUT_KEY_PAGE_UP)
	{
		key_pup = 0;
	}
	if (key == GLUT_KEY_PAGE_DOWN)
	{
		key_pdown = 0;
	}
}

// 键盘按下函数
void myKeyboardFunc(unsigned char key, int x, int y) {
	cout << key << " Down" << endl;
	switch (key) {
	case 'a':
		key_a = 1;
		break;
	case 'd':
		key_d = 1;
		break;
	case 'w':
		key_w = 1;
		break;
	case 's':
		key_s = 1;
		break;
	case 'q':
		key_q = 1;
		break;
	case 'e':
		key_e = 1;
		break;
	case 'j':
		key_j = 1;
		break;
	case 'l':
		key_l = 1;
		break;
	case 'i':
		key_i = 1;
		break;
	case 'k':
		key_k = 1;
		break;
	case 'u':
		key_u = 1;
		break;
	case 'o':
		key_o = 1;
		break;
	case '[':
		key_lmp = 1;
		break;
	case ']':
		key_rmp = 1;
		break;
	case '{':
		key_lbp = 1;
		break;
	case '}':
		key_rbp = 1;
		break;
	case '1':
		if (mode == 2 && bind == -1) {
			Euler = EyeMat.GetInverse().ToEuler();
		}
		if (mode == 2 && bind != -1 && toBind == 0) {
			tempe1 = tempm1.GetInverse().ToEuler();
		}
		mode = 1;
		break;
	case '2':
		if (mode == 1 && bind == -1) {
			temp = Euler.ToMatrix().GetInverse();
			EyeMat = temp;
			temp.SetTrans(translatev);
			temp = temp.GetInverse();
			EyeMat = EyeMat * temp;
		}
		if (mode == 1 && bind != -1 && toBind == 0) {
			tempm1 = tempe1.ToMatrix().GetInverse();
		}
		mode = 2;
		break;
	case 27:
		key_esc = 1;
		break;
	case '>':
		key_greater = 1;
		break;
	case '<':
		key_less = 1;
		break;
	}
}

void mySpecialFunc(GLint key, GLint x, GLint y) {
	cout << key << " Down" << endl;
	if (key == GLUT_KEY_UP)
	{
		key_up = 1;
	}
	if (key == GLUT_KEY_DOWN)
	{
		key_down = 1;
	}
	if (key == GLUT_KEY_LEFT)
	{
		key_left = 1;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		key_right = 1;
	}
	if (key == GLUT_KEY_PAGE_UP)
	{
		key_pup = 1;
	}
	if (key == GLUT_KEY_PAGE_DOWN)
	{
		key_pdown = 1;
	}
}

// 响应按键事件
void KeyboardFunc() {
	if (key_esc == 1 && bind != -1) {
		bind = -1;
		toBind = 1;
		tempv = GodViewTranslatev;
		tempv1 = tempv - translatev;
		tempv1 = tempv1 * (1.0 / FRAME);
		if (mode == 2) {
			temp = GodViewModeM.GetInverse();
			temp1 = EyeMat.GetInverse();
		}
		else {
			temp = GodViewModeE.ToMatrix();
			temp1 = Euler.ToMatrix();
		}
		tempv.Set(-temp[8], -temp[9], -temp[10]);
		tempv2.Set(-temp1[8], -temp1[9], -temp1[10]);
		tempq = tempv.ToEuler().ToQuaternion(); // 目标
		tempq1 = tempv2.ToEuler().ToQuaternion(); // 当前
		tempq.Normalize();
		tempq1.Normalize();
		tempq1.Slerp(tempq, FRAME, t, temparrq);
	}
	if (mode == 1) {
		if (key_a == 1) {
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixf(Euler.ToMatrix());
			glGetFloatv(GL_MODELVIEW_MATRIX, temp);
			glPopMatrix();
			tempv.Set(temp[0], temp[1], temp[2]);
			tempv = tempv * -mspeed;
			translatev = translatev + tempv;
		}
		if (key_d == 1) {
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixf(Euler.ToMatrix());
			glGetFloatv(GL_MODELVIEW_MATRIX, temp);
			glPopMatrix();
			tempv.Set(temp[0], temp[1], temp[2]);
			tempv = tempv * mspeed;
			translatev = translatev + tempv;
		}
		if (key_w == 1) {
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixf(Euler.ToMatrix());
			glGetFloatv(GL_MODELVIEW_MATRIX, temp);
			glPopMatrix();
			tempv.Set(temp[4], temp[5], temp[6]);
			tempv = tempv * mspeed;
			translatev = translatev + tempv;
		}
		if (key_s == 1) {
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixf(Euler.ToMatrix());
			glGetFloatv(GL_MODELVIEW_MATRIX, temp);
			glPopMatrix();
			tempv.Set(temp[4], temp[5], temp[6]);
			tempv = tempv * -mspeed;
			translatev = translatev + tempv;
		}
		if (key_q == 1) {
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixf(Euler.ToMatrix());
			glGetFloatv(GL_MODELVIEW_MATRIX, temp);
			glPopMatrix();
			tempv.Set(temp[8], temp[9], temp[10]);
			tempv = tempv * -mspeed;
			translatev = translatev + tempv;
		}
		if (key_e == 1) {
			glPushMatrix();
			glLoadIdentity();
			glMultMatrixf(Euler.ToMatrix());
			glGetFloatv(GL_MODELVIEW_MATRIX, temp);
			glPopMatrix();
			tempv.Set(temp[8], temp[9], temp[10]);
			tempv = tempv * mspeed;
			translatev = translatev + tempv;
		}
	}
	else {
		if (key_a == 1) {
			temp = EyeMat.GetInverse();
			tempv1.Set(temp[0], temp[1], temp[2]);
			tempv1 = tempv1 * -mspeed;
			translatev = translatev + tempv1;
			tempv.Set(-mspeed, 0, 0);
			temp.SetTrans(tempv);
			temp = temp.GetInverse();
			EyeMat = temp * EyeMat;
		}
		if (key_d == 1) {
			temp = EyeMat.GetInverse();
			tempv1.Set(temp[0], temp[1], temp[2]);
			tempv1 = tempv1 * mspeed;
			translatev = translatev + tempv1;
			tempv.Set(mspeed, 0, 0);
			temp.SetTrans(tempv);
			temp = temp.GetInverse();
			EyeMat = temp * EyeMat;
		}
		if (key_w == 1) {
			temp = EyeMat.GetInverse();
			tempv1.Set(temp[4], temp[5], temp[6]);
			tempv1 = tempv1 * mspeed;
			translatev = translatev + tempv1;
			tempv.Set(0, mspeed, 0);
			temp.SetTrans(tempv);
			temp = temp.GetInverse();
			EyeMat = temp * EyeMat;
		}
		if (key_s == 1) {
			temp = EyeMat.GetInverse();
			tempv1.Set(temp[4], temp[5], temp[6]);
			tempv1 = tempv1 * -mspeed;
			translatev = translatev + tempv1;
			tempv.Set(0, -mspeed, 0);
			temp.SetTrans(tempv);
			temp = temp.GetInverse();
			EyeMat = temp * EyeMat;
		}
		if (key_q == 1) {
			temp = EyeMat.GetInverse();
			tempv1.Set(temp[8], temp[9], temp[10]);
			tempv1 = tempv1 * -mspeed;
			translatev = translatev + tempv1;
			tempv.Set(0, 0, -mspeed);
			temp.SetTrans(tempv);
			temp = temp.GetInverse();
			EyeMat = temp * EyeMat;
		}
		if (key_e == 1) {
			temp = EyeMat.GetInverse();
			tempv1.Set(temp[8], temp[9], temp[10]);
			tempv1 = tempv1 * mspeed;
			translatev = translatev + tempv1;
			tempv.Set(0, 0, mspeed);
			temp.SetTrans(tempv);
			temp = temp.GetInverse();
			EyeMat = temp * EyeMat;
		}
	}
	if (bind == -1) {
		if (mode == 1) {
			if (key_j == 1) {
				Euler.h += rspeed;
			}
			if (key_l == 1) {
				Euler.h -= rspeed;

			}
			if (key_i == 1) {
				Euler.p += rspeed;

			}
			if (key_k == 1) {
				Euler.p -= rspeed;
			}
			if (key_u == 1) {
				Euler.b += rspeed;

			}
			if (key_o == 1) {
				Euler.b -= rspeed;

			}
		}
		if (mode == 2) {
			if (key_j == 1) {
				rotatev.Set(0, 1, 0);
				temp.SetRotate(rspeed, rotatev);
				temp = temp.GetInverse();
				EyeMat = temp * EyeMat;
			}
			if (key_l == 1) {
				rotatev.Set(0, 1, 0);
				temp.SetRotate(-rspeed, rotatev);
				temp = temp.GetInverse();
				EyeMat = temp * EyeMat;
			}
			if (key_i == 1) {
				rotatev.Set(1, 0, 0);
				temp.SetRotate(rspeed, rotatev);
				temp = temp.GetInverse();
				EyeMat = temp * EyeMat;
			}
			if (key_k == 1) {
				rotatev.Set(1, 0, 0);
				temp.SetRotate(-rspeed, rotatev);
				temp = temp.GetInverse();
				EyeMat = temp * EyeMat;
			}
			if (key_u == 1) {
				rotatev.Set(0, 0, 1);
				temp.SetRotate(-rspeed, rotatev);
				temp = temp.GetInverse();
				EyeMat = temp * EyeMat;
			}
			if (key_o == 1) {
				rotatev.Set(0, 0, 1);
				temp.SetRotate(rspeed, rotatev);
				temp = temp.GetInverse();
				EyeMat = temp * EyeMat;
			}
		}
	}
	if (bind != -1 && toBind == 0) {
		if (mode == 1) {
			if (key_j == 1) {
				tempe1.h += rspeed;
			}
			if (key_l == 1) {
				tempe1.h -= rspeed;

			}
			if (key_i == 1) {
				tempe1.p += rspeed;

			}
			if (key_k == 1) {
				tempe1.p -= rspeed;
			}
			if (key_u == 1) {
				tempe1.b += rspeed;

			}
			if (key_o == 1) {
				tempe1.b -= rspeed;
			}
		}
		if (mode == 2) {
			if (key_j == 1) {
				rotatev.Set(0, 1, 0);
				temp.SetRotate(rspeed, rotatev);
				tempm1 = temp.GetInverse() * tempm1;
			}
			if (key_l == 1) {
				rotatev.Set(0, 1, 0);
				temp.SetRotate(-rspeed, rotatev);
				tempm1 = temp.GetInverse() * tempm1;
			}
			if (key_i == 1) {
				rotatev.Set(1, 0, 0);
				temp.SetRotate(rspeed, rotatev);
				tempm1 = temp.GetInverse() * tempm1;
			}
			if (key_k == 1) {
				rotatev.Set(1, 0, 0);
				temp.SetRotate(-rspeed, rotatev);
				tempm1 = temp.GetInverse() * tempm1;
			}
			if (key_u == 1) {
				rotatev.Set(0, 0, 1);
				temp.SetRotate(-rspeed, rotatev);
				tempm1 = temp.GetInverse() * tempm1;
			}
			if (key_o == 1) {
				rotatev.Set(0, 0, 1);
				temp.SetRotate(rspeed, rotatev);
				tempm1 = temp.GetInverse() * tempm1;
			}
		}
	}
	if (key_lmp == 1) {
		mspeed = mspeed >= 0.4 ? mspeed - 0.4 : mspeed;
	}
	if (key_rmp == 1) {
		mspeed += 0.4;
	}
	if (key_lbp == 1) {
		rspeed = rspeed >= 0.2 ? rspeed - 0.2 : rspeed;
	}
	if (key_rbp == 1) {
		rspeed += 0.2;
	}
	if (key_greater == 1) {
		SpeedRatio += 0.05;
		SpeedRatio = SpeedRatio > 1.0 ? 1.0 : SpeedRatio;
	}
	if (key_less == 1) {
		SpeedRatio -= 0.05;
		SpeedRatio = SpeedRatio < 0.0 ? 0.0 : SpeedRatio;
	}
	if (lock == 1) {
		tempv.Set(cvector[choose1][0], cvector[choose1][1], cvector[choose1][2]);
		temp = tempv.ToEuler().ToMatrix();
		speed = pow(cvector1[choose1][2], 2) + pow(cvector1[choose1][1], 2) + pow(cvector1[choose1][0], 2);
		if (key_up == 1) {
			tempv.Set(temp[4], temp[5], temp[6]);
		}
		if (key_down == 1) {
			tempv.Set(-temp[4], -temp[5], -temp[6]);
		}
		if (key_right == 1) {
			tempv.Set(temp[0], temp[1], temp[2]);
		}
		if (key_left == 1) {
			tempv.Set(-temp[0], -temp[1], -temp[2]);
		}
		for (int i = 0; i < 3; i++) {
			cvector1[choose1][i] += tempv[i] * 0.1f;
		}
		SpeedNormalize(cvector1[choose1]);
	}
	if (key_pdown == 1) {
		float speed1 = 0.0;
		for (int i = 0; i < 3; i++) {
			speed1 += pow(cvector[choose1][i], 2);
		}
		if (speed1 >= 0.1) {
			SpeedRatio1 -= 0.1;
		}		
	}
	if (key_pup == 1) {
		float speed1 = 0.0;
		for (int i = 0; i < 3; i++) {
			speed1 += pow(cvector[choose1][i], 2);
		}
		if (speed1 <= 4.0) {
			SpeedRatio1 += 0.1;
		}	
	}
}

// 响应鼠标回调函数
void myMouseFunc(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			temp_trans = translatev;
			ray = GenerateRay(x, y);
			CVector217 ray1;
			ray1 = ray - temp_trans;
			int find = 0;
			float minl = 0xffffff, minh = 0xffffff;
			for (int i = 0; i < 100; i++) {
				tempv.Set(clocation[i][0], clocation[i][1], clocation[i][2]);
				tempv = tempv - temp_trans;
				float proj = tempv.project(ray1).len();
				if (proj >= 0) {
					float l = tempv.len();
					float h = sqrt(pow(l, 2) - pow(proj, 2));
					if (h < csize[i] && i != bind) {
						if (l < minl) {
							minl = l;
							choose = i;
							find = 1;
							cout << i << endl;
						}
					}
				}
				else continue;
			}
			if (find) {
				lock = 1;
				choose1 = choose;
				tempm1.Set(g_IEyeMat);
				tempe1.Set(0, 0, 0);
				if (bind == -1) {
					if (mode == 2) {
						GodViewModeM = EyeMat;
					}
					else {
						GodViewModeE = Euler;
					}
					GodViewTranslatev = translatev;
				}
				bind = choose;
				choose = -1;
				tempv.Set(clocation[bind][0], clocation[bind][1], clocation[bind][2]);
				tempv = tempv - temp_trans;
				tempv1 = tempv * (1.0 / FRAME);
				toBind = 1;
				find = 0;
				if (mode == 2) {
					temp = EyeMat.GetInverse();
				}
				else {
					temp = Euler.ToMatrix();
				}
				tempv.Set(cvector[bind][0], cvector[bind][1], cvector[bind][2]);
				tempv2.Set(-temp[8], -temp[9], -temp[10]);
				tempq = tempv.ToEuler().ToQuaternion();// 目标
				tempq1 = tempv2.ToEuler().ToQuaternion();// 当前
				tempq.Normalize();
				tempq1.Normalize();
				tempq1.Slerp(tempq, FRAME, t, temparrq);
			}
		}
	}
}

// 生成射线,返回鼠标点击点的世界坐标
CVector217 GenerateRay(float x, float y) {
	CVector217 ray;
	double projection[16], modelview[16];
	int viewport[4];
	glPushMatrix();
	glLoadIdentity();
	if (mode == 2) {
		glMultMatrixf(EyeMat);
	}
	else {
		glMultMatrixf(Euler.ToMatrix().GetInverse() * EulerTempM);
	}
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glPopMatrix();
	double objx, objy, objz;
	gluUnProject(x, viewport[3] - y, 1, modelview, projection, viewport, &objx, &objy, &objz); 
	ray[0] = objx;
	ray[1] = objy;
	ray[2] = objz;
	return ray;
}

//气泡吞噬退出函数
void exit1() {
	float size1;
	if (lock == 1) {
		for (int i = 0; i < 100; i++) {
			if (i != choose1 && csize[i] <= csize[choose1] && csize[i] != 0) {
				size1 = pow((clocation[choose1][0] - clocation[i][0]), 2) + pow((clocation[choose1][1] - clocation[i][1]), 2) + pow((clocation[choose1][2] - clocation[i][2]), 2);
				if (pow((csize[choose1] + csize[i]), 2)>size1) {
					csize[choose1] = pow((pow(csize[i], 3) + pow(csize[choose1], 3)), (1.0 / 3.0));
					csize[i] = 0;
					bnum -= 1;
				}
			}
		}
	}
	if (bnum == 1) {
		exit(0);
	}
}

// 显示字体
void Font2D(const char * str, double x, double y) {
	// 设置投影方式：平行投影
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// 输出字符串
	int len = strlen(str);
	glRasterPos2f(x, y);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
	// 恢复投影方式
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// 将速度归一化为1
void SpeedNormalize(float *a) {
	float len = 0.0;
	for (int i = 0; i < 3; i++) {
		len += pow(a[i], 2);
	}
	len = sqrt(len);
	if (len != 0.0) {
		for (int i = 0; i < 3; i++) {
			a[i] /= len;
		}
	}
}

// 绑定纹理函数
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
	{
		printf("Wrong!!!!\n");
		return 0;
	}

	// 读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	GLint line_bytes = width * 3;
	while (line_bytes % 4 != 0)
		++line_bytes;
	total_bytes = line_bytes * height;

	// 根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// 读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}


	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);

	glBindTexture(GL_TEXTURE_2D, texture_ID);   //绑定纹理

												//设置4个常用的纹理参数。。如何把纹理像素映射成像素
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //指定纹理贴图与材质的混合模式

																//参数： 目标纹理，多重细节层次（不考虑多重纹理为0），RGB数据存储格式，二维纹理像素宽高，纹理边框大小
																//       纹理像素数据的格式，数据保存形式（像素数据类型，字节），内存中指向纹理图像的指针（纹理图像的内参快地址）
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels); //为纹理对象指定一个纹理

	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定

	free(pixels);
	return texture_ID;
}