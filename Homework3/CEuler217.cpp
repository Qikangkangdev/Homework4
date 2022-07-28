#include "stdafx.h"
#include "ceuler217.h"
#include "cmatrix217.h"
#include "cvector217.h"
#include "cquaternion217.h"
#include "math.h"
#define PI acos(-1.0)

using namespace std;

// 默认构造函数
CEuler217::CEuler217() {
	for (int i = 0; i < 3; i++) {
		(*this)[i] = 0;
	}
}

// 带参构造函数
CEuler217::CEuler217(float h, float p, float b) {
	(*this)[0] = h;
	(*this)[1] = p;
	(*this)[2] = b;
}

// 拷贝构造函数
CEuler217::CEuler217(CEuler217 &a) {
	for (int i = 0; i < 3; i++) {
		(*this)[i] = a[i];
	}
}

// 类型转换
CEuler217::operator float *() { return &h; };

// 给欧拉角赋值
void CEuler217::Set(float fh, float fp, float fb) {
	(*this)[0] = fh;
	(*this)[1] = fp;
	(*this)[2] = fb;
}

// 欧拉角转化为向量
CVector217 CEuler217::ToVector() {
	CMatrix217 m = (*this).ToMatrix();
	CVector217 res;
	float x = -sin(PI / 180 * h) * cos(PI / 180 * p);
	float z = -cos(PI / 180 * h) * cos(PI / 180 * p);
	float y = sin(PI / 180 * p);
	res.Set(x, y, z);
	return res;
}

// 欧拉角转化为矩阵
CMatrix217 CEuler217::ToMatrix() {
	CMatrix217 resx;
	CMatrix217 resy;
	CMatrix217 resz;
	CVector217 x;
	x.Set(1.0, 0.0, 0.0);
	CVector217 y;
	y.Set(0.0, 1.0, 0.0);
	CVector217 z;
	z.Set(0.0, 0.0, 1.0);
	resy.SetRotate(h, y);
	resx.SetRotate(p, x);
	resz.SetRotate(b, z);
	CMatrix217 res = (resy * resx) * resz;
	return res;
}

// 欧拉角转化为四元数
CQuaternion217 CEuler217::ToQuaternion() {
	CQuaternion217 h1, p1, b1;
	h1.Set(0.0, sin(h * PI / 180.0 / 2.0), 0.0, cos(h * PI / 180.0 / 2.0));
	p1.Set(sin(p * PI / 180.0 / 2.0), 0.0, 0.0, cos(p * PI / 180.0 / 2.0));
	b1.Set(0.0, 0.0, sin(b * PI / 180.0 / 2.0), cos(b * PI / 180.0 / 2.0));
	CQuaternion217 res = b1 * p1 * h1;
	return res;
}

// 欧拉角规范化
void CEuler217::Normal() {
	if (p > 90) {
		do {
			p -= 180;
			h += 180;
		}while(p > 90);
	}
	if (p < -90) {
		do {
			p += 180;
			h += 180;
		} while (p < -90);
	}
	if (h > 180) {
		do {
			h -= 360;
		} while (h > 180);
	}
	if (h < -180) {
		do {
			h += 360;
		} while (h < -180);
	}
	if (b > 180) {
		do {
			b -= 360;
		} while (b > 180);
	}
	if (b < -180) {
		do {
			b += 360;
		} while (b < -180);
	}
}