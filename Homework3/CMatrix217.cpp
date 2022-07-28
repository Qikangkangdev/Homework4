#include "stdafx.h"
#include "cmatrix217.h"
#include "cvector217.h"
#include "ceuler217.h"
#include "cquaternion217.h"
#include "math.h"
#include <string>
#include <sstream>
#define PI acos(-1.0)
using namespace std;

//默认构造函数
CMatrix217::CMatrix217() {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = 0;
	}
}

//带参构造函数
CMatrix217::CMatrix217(float *a) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = *(a + i);
	}
}

//拷贝构造函数
CMatrix217::CMatrix217(CMatrix217 &a) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = a[i];
	}
}

//类型转换
CMatrix217::operator float*() { return &m00; }

//给矩阵16个元素赋值
void CMatrix217::Set(float *val) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = *(val + i);
	}
}


//矩阵赋值
CMatrix217 CMatrix217::operator=(CMatrix217 &p) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] = p[i];
	}
	return *this;
}

//矩阵数乘
CMatrix217 CMatrix217::operator*(float d) {
	for (int i = 0; i < 16; i++) {
		(*this)[i] *= d;
	}
	return *this;
}

//矩阵相乘
CMatrix217 CMatrix217::operator*(CMatrix217 &p) {
	CMatrix217 res;
	float a=0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				a += (*this)[k * 4 + i] * p[j * 4 + k];
			}
			res[4 * j + i] = a;
			a = 0;
		}
	}
	return res;
}

//矩阵与向量相乘
CVector217 CMatrix217::vecMul(CVector217 &p) {
	CVector217 res;
	float a[4] = { p.x, p.y, p.z, 0 };
	float b[4] = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++){
			b[i] += (*this)[i + 4 * j] * a[j];
		}
	}
	res[0] = b[0];
	res[1] = b[1];
	res[2] = b[2];
	return res;
}

//矩阵与位置相乘
CVector217 CMatrix217::posMul(CVector217 &p) {
	CVector217 res;
	float a[4] = { p.x, p.y, p.z, 1 };
	float b[4] = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			b[i] += (*this)[i + 4 * j] * a[j];
		}
	}
	res[0] = b[0];
	res[1] = b[1];
	res[2] = b[2];
	return res;
}

//设置为旋转矩阵
void CMatrix217::SetRotate(float seta, CVector217 axis) {
	axis.Normalize();
	float cs = cos(seta / 180 * PI);
	float si = sin(seta / 180 * PI);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	(*this)[0] = pow(x, 2) * (1 - cs) + cs;
	(*this)[1] = x * y * (1 - cs) + z * si;
	(*this)[2] = x * z * (1 - cs) - y * si;
	(*this)[3] = 0.0;
	(*this)[4] = x * y * (1 - cs) - z * si;
	(*this)[5] = pow(y, 2) * (1 - cs) + cs;
	(*this)[6] = y * z * (1 - cs) + x * si;
	(*this)[7] = 0.0;
	(*this)[8] = x * z * (1 - cs) + y * si;
	(*this)[9] = y * z * (1 - cs) - x * si;
	(*this)[10] = pow(z, 2) * (1 - cs) + cs;
	(*this)[11] = 0.0;
	(*this)[12] = 0.0;
	(*this)[13] = 0.0;
	(*this)[14] = 0.0;
	(*this)[15] = 1.0;
}

//设置为平移矩阵
void CMatrix217::SetTrans(CVector217 trans) {
	(*this)[0] = 1;
	(*this)[1] = 0;
	(*this)[2] = 0;
	(*this)[3] = 0;
	(*this)[4] = 0;
	(*this)[5] = 1;
	(*this)[6] = 0;
	(*this)[7] = 0;
	(*this)[8] = 0;
	(*this)[9] = 0;
	(*this)[10] = 1;
	(*this)[11] = 0;
	(*this)[12] = trans.x;
	(*this)[13] = trans.y;
	(*this)[14] = trans.z;
	(*this)[15] = 1;
}

//设置为缩放矩阵
void CMatrix217::SetScale(CVector217 p) {
	(*this)[0] = p.x;
	(*this)[1] = 0;
	(*this)[2] = 0;
	(*this)[3] = 0;
	(*this)[4] = 0;
	(*this)[5] = p.y;
	(*this)[6] = 0;
	(*this)[7] = 0;
	(*this)[8] = 0;
	(*this)[9] = 0;
	(*this)[10] = p.z;
	(*this)[11] = 0;
	(*this)[12] = 0;
	(*this)[13] = 0;
	(*this)[14] = 0;
	(*this)[15] = 1;
}

//矩阵求逆，成功返回行列式的值，否则返回0
float CMatrix217::Inverse() {
	CVector217 a, b, c;
	for (int i = 0; i < 3; i++) {
		a[i] = (*this)[i * 4 + 0];
		b[i] = (*this)[i * 4 + 1];
		c[i] = (*this)[i * 4 + 2];
	}
	float det = a.crossMul(b).dotMul(c);
	return det;
}

//返回逆矩阵
CMatrix217 CMatrix217::GetInverse() {
	float det = (*this).Inverse();
	float a[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	if (det == 0) return CMatrix217(a);
	CMatrix217 adj;
	adj[0] = (*this)[5] * (*this)[10] - (*this)[6] * (*this)[9];
	adj[1] = -((*this)[1] * (*this)[10] - (*this)[2] * (*this)[9]);
	adj[2] = (*this)[1] * (*this)[6] - (*this)[2] * (*this)[5];
	adj[3] = 0.0;
	adj[4] = -((*this)[4] * (*this)[10] - (*this)[6] * (*this)[8]);
	adj[5] = (*this)[0] * (*this)[10] - (*this)[2] * (*this)[8];
	adj[6] = -((*this)[0] * (*this)[6] - (*this)[2] * (*this)[4]);
	adj[7] = 0.0;
	adj[8] = (*this)[4] * (*this)[9] - (*this)[8] * (*this)[5];
	adj[9] = -((*this)[0] * (*this)[9] - (*this)[1] * (*this)[8]);
	adj[10] = (*this)[5] * (*this)[0] - (*this)[1] * (*this)[4];
	adj[11] = 0.0;
	adj[12] = -((*this)[12] * (*this)[0] + (*this)[13] * (*this)[4] + (*this)[14] * (*this)[8]) * det;
	adj[13] = -((*this)[12] * (*this)[1] + (*this)[13] * (*this)[5] + (*this)[14] * (*this)[9]) * det;
	adj[14] = -((*this)[12] * (*this)[2] + (*this)[13] * (*this)[6] + (*this)[14] * (*this)[10]) * det;
	adj[15] = det;
	return adj * (1.0 / det);
	}

// 矩阵转化为欧拉角
CEuler217 CMatrix217::ToEuler() {
	float p = asin(-(*this)[9]);
	float b = 0.0, h = 0.0;
	if (p == 90.0 || p == -90.0) {
		h = atan2(-(*this)[2], *(this)[0]);
		return CEuler217(h * 180 / PI, p * 180 / PI, b * 180 / PI);
	}
	else {
		h = atan2((*this)[8], (*this)[10]);
		b = atan2((*this)[1], (*this)[5]);
		return CEuler217(h * 180 / PI, p * 180 / PI, b * 180 / PI);
	}
}

// 矩阵转化为四元数
CQuaternion217 CMatrix217::ToQuaternion() {
	int flag = 1;
	float wz = (*this)[1] - (*this)[4];
	float wy = (*this)[8] - (*this)[2];
	float wx = (*this)[6] - (*this)[9];
	float w = sqrt((*this)[0] + (*this)[5] + (*this)[10] + 1) / 2;
	float x = wx / 4.0 * w;
	float y = wy / 4.0 * w;
	float z = wz / 4.0 * w;
	CQuaternion217 res(x, y, z, w);
	return res;
}


