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
#include <string>
#include <ctime>
#define FRAME 25
using namespace std;

void reshape(int w, int h);
void myDisplay(void);
float setax = 0.0;
float setay = 0.0;
void myTimerFunc(int val);
float clocation[100][3];
float ccolor[100][3];
float cvector[100][3];
float csize[100];
void myKeyboardUpFunc(unsigned char key, int x, int y);
void myKeyboardFunc(unsigned char key, int x, int y);
void myMouseFunc(int button, int state, int x, int y);
void SetView();
void init();
void RenderWorld();
void KeyboardFunc();
CVector217 GenerateRay(float x, float y);
int key_a, key_d, key_w, key_s, key_q, key_e, key_j, key_l, key_i, key_esc;
int key_k, key_u, key_o, key_lmp, key_rmp, key_lbp, key_rbp, mode = 1, key_1, key_2;
CEuler217 Euler, tempe, GodViewModeE, tempe1;
float mspeed = 1, rspeed = 1;
float g_IEyeMat[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
CVector217 rotatev, translatev, tempv, tempv1, GodViewTranslatev, tempv2;
CQuaternion217 temparrq[200];
CMatrix217 EyeMat, IEyeMat(g_IEyeMat), temp, EulerTempM, GodViewModeM, GodViewModeEulerTempM,temp1, tempm1(g_IEyeMat);
CQuaternion217 tempq, tempq1;
float OGLMANAGER_WIDTH, OGLMANAGER_HEIGHT;
CVector217 ray, temp_trans, GodViewModeT;
int choose = -1, bind = -1, frame;
int toBind;
float t[FRAME];

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
	uniform_real_distribution<float> vector(0.2, 0.4);
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
	glEnable(GL_DEPTH_TEST);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("作业四");
	init();
	glutTimerFunc(20, myTimerFunc, 0);
	glutReshapeFunc(&reshape);
	glutKeyboardUpFunc(&myKeyboardUpFunc);
	glutKeyboardFunc(&myKeyboardFunc);
	glutMouseFunc(&myMouseFunc);
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

	// 绘制顶面
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 100.0f);
	glVertex3f(100.0f, 100.0f, 100.0f);
	glVertex3f(100.0f, 100.0f, 0.0f);
	glEnd();

	// 绘制底面
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);

	// 绘制左面
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glVertex3f(0.0f, 100.0f, 100.0f);
	glEnd();

	// 绘制右面
	glBegin(GL_QUADS);
	glVertex3f(100.0f, 100.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 100.0f, 0.0f);
	glEnd();

	// 绘制前面
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 100.0f, 100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 100.0f, 100.0f);
	glEnd();

	// 绘制后面
	glBegin(GL_QUADS);
	glVertex3f(100.0f, 100.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glEnd();

	glPopMatrix();
	// 立方体绘制完毕

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
		glColor4f(ccolor[i][0], ccolor[i][1], ccolor[i][2], 0.0f);
		glutSolidSphere(csize[i], 100, 100);
		glPopMatrix();
	}
	// 球体绘制完毕
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
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 3; j++) {
			clocation[i][j] += cvector[i][j];
			if (clocation[i][j] + csize[i] > 50) {
				clocation[i][j] = 50.0 - csize[i];
				cvector[i][j] *= -1.0;
			}
			else if (clocation[i][j] - csize[i] < -50) {
				clocation[i][j] = -50.0 + csize[i];
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
