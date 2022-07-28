#include "stdafx.h"
#include<math.h>
#include "cvector217.h"
#include "ceuler217.h"
#include <string>
#include <sstream>
#define PI acos(-1.0);
using namespace std;

// �޲ι��캯��
CVector217::CVector217() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

// �вι��캯��
CVector217::CVector217(float *a) {
	for (int i = 0; i < 3; i++) {
		(*this)[i] = a[i];
	}
}

// �������캯��
CVector217::CVector217(CVector217 &a) {
	for (int i = 0; i < 3; i++) {
		(*this)[i] = a[i];
	}
}

// ��ֵ����
void CVector217::Set(float tx, float ty, float tz) {
	x = tx;
	y = ty;
	z = tz;
}

// ���ؼӺ�
CVector217 CVector217::operator+(CVector217 &p) {
	CVector217 res;
	res.x = x + p.x;
	res.y = y + p.y;
	res.z = z + p.z;
	return res;
}

// ���ؼ���
CVector217 CVector217::operator-(CVector217 &p) {
	CVector217 res;
	res.x = x - p.x;
	res.y = y - p.y;
	res.z = z - p.z;
	return res;
}

// ���ظ�ֵ��
CVector217 CVector217::operator=(CVector217 &p) {

	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
}

// ������Ⱥ�
int CVector217::operator==(CVector217 &p) {
	int isEqual = 1;
	if (x != p.x) { isEqual = 0; };
	if (y != p.y) { isEqual = 0; };
	if (z != p.z) { isEqual = 0; };
	return isEqual;
}

// ���ز��Ⱥ�
int CVector217::operator!=(CVector217 &p) {
	int isNotEqual = 1;
	if (x == p.x&&y == p.y&&z == p.z) { isNotEqual = 0; };
	return isNotEqual;
}

// ����ת��
CVector217::operator float*() { return &x; }

// ����
CVector217 CVector217::operator*(float n) {
	CVector217 res;
	res.x = x*n;
	res.y = y*n;
	res.z = z*n;
	return res;
}

// ���
float CVector217::dotMul(CVector217 &n) {
	return x*n.x + y*n.y + z*n.z;
}

// ���
CVector217 CVector217::crossMul(CVector217 &n) {
	CVector217 res;
	res.x = y*n.z - z*n.y;
	res.y = z*n.x - x*n.z;
	res.z = x*n.y - y*n.x;
	return res;
}

// ��λ��
void CVector217::Normalize() {
	float w = sqrt(x*x + y*y + z*z);
	if (w != 0) {
		x = x / w;
		y = y / w;
		z = z / w;
	}
}

// ��ģ
float CVector217::len() {
	return sqrt(x*x + y*y + z*z);
}

// ͶӰ
CVector217 CVector217::project(CVector217 &n) {
	n.Normalize();
	return n * dotMul(n);
}

// ת��Ϊŷ����
CEuler217 CVector217::ToEuler() {
	CEuler217 res;
	if (z != 0.0) {
		res[0] = atan(x / z) * 180.0 / PI;
	}
	else {
		res[0] = x < 0.0 ? 90.0 : -90.0;
	}
	if (z > 0) {
		res[0] *= -1.0;
		res[0] = x < 0.0 ? 180.0 - res[0] : -180.0 - res[0];
	}
	res[1] = atan2(y, sqrt(pow(x, 2) + pow(z, 2))) * 180.0 / PI;
	res[2] = 0;
	res.Normal();
	return res;
}
