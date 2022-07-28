#include "stdafx.h"
#include "ceuler217.h"
#include "cmatrix217.h"
#include "cvector217.h"
#include "cquaternion217.h"
#include "math.h"
#define PI acos(-1.0);

using namespace std;

// 默认构造函数
CQuaternion217::CQuaternion217() {
	for (int i = 0; i < 4; i++) {
		(*this)[0] = 0.0;
	}
}

// 带参构造函数
CQuaternion217::CQuaternion217(float x, float y, float z, float w) {
	(*this)[0] = x;
	(*this)[1] = y;
	(*this)[2] = z;
	(*this)[3] = w;
}

// 拷贝构造函数
CQuaternion217::CQuaternion217(CQuaternion217& a) {
	for (int i = 0; i < 4; i++) {
		(*this)[i] = a[i];
	}
}

// 赋值函数
void CQuaternion217::Set(float fx, float fy, float fz, float fw) {
	(*this)[0] = fx;
	(*this)[1] = fy;
	(*this)[2] = fz;
	(*this)[3] = fw;
}

// 类型转换
CQuaternion217::operator float*() {
	return &x;
}

// 重载赋值符号
CQuaternion217& CQuaternion217::operator=(const CQuaternion217& p) {
	(*this).x = p.x;
	(*this).y = p.y;
	(*this).z = p.z;
	(*this).w = p.w;
	return *this;
}

// 重载加号
CQuaternion217 CQuaternion217::operator+(const CQuaternion217& p) {
	CQuaternion217 res;
	CQuaternion217 res1;
	if (p.w * (*this).w < 0.0) {
		res1 = (*this) * -1.0;
	}
	res.x = (*this).x + p.x;
	res.y = (*this).y + p.y;
	res.z = (*this).z + p.z;
	res.w = (*this).w + p.w;
	res = res.w * (*this).w < 0.0 ? res * -1.0 : res;
	return res;
}

// 重载数乘
CQuaternion217 CQuaternion217::operator*(float data) {
	CQuaternion217 res;
	for (int i = 0; i < 4; i++) {
		res[i] = data * (*this)[i];
	}
	return res;
}

// 四元数乘法
CQuaternion217 CQuaternion217::operator*(const CQuaternion217 &p) {
	CQuaternion217 res;
	float x1 = (*this).x; float x2 = p.x;
	float y1 = (*this).y; float y2 = p.y;
	float z1 = (*this).z; float z2 = p.z;
	float w1 = (*this).w; float w2 = p.w;
	res[0] = w1 * x2 + x1 * w2 + z1 * y2 - y1 * z2;
	res[1] = w1 * y2 + y1 * w2 + x1 * z2 - z1 * x2;
	res[2] = w1 * z2 + z1 * w2 + y1 * x2 - x1 * y2;
	res[3] = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
	return res;
}

// 点乘
float CQuaternion217::dotMul(const CQuaternion217& p) {
	return (*this).x * p.x + (*this).y * p.y + (*this).z * p.z + (*this).w * p.w;
}

// 求模
float CQuaternion217::len() {
	float res = 0.0;
	for (int i = 0; i < 4; i++) {
		res += pow((*this)[i], 2);
	}
	return sqrt(res);
}

// 求标准化
void CQuaternion217::Normalize() {
	float res = 1.0 / (*this).len();
	(*this) = (*this) * res;
}

// 求逆四元数，会改变自身
CQuaternion217& CQuaternion217::Inverse() {
	(*this) = (*this) * -1.0;
	(*this)[3] *= -1.0;
	(*this) = (*this) * (1.0 / pow((*this).len(), 2));
	return (*this);
}

// 求逆四元数,不会改变自身，生成新的四元数
CQuaternion217 CQuaternion217::GetInverse() {
	CQuaternion217 res;
	res = (*this) * -1.0;
	res[3] *= -1.0;
	return res * (1.0 / pow(res.len(), 2));
}

// 求差 当前为a,求c = a-b
CQuaternion217 CQuaternion217::Div(const CQuaternion217& b) {
	CQuaternion217 res = (*this).GetInverse();
	return res * b;
}

// 求旋转轴和角度
void CQuaternion217::GetAngle(float& angle, CVector217& axis) {
	(*this).Normalize();
	angle = acos((*this)[3]) * 2 * 180 / PI;
	float a = sin(angle / 2);
	if (a != 0.0) {
		for (int i = 0; i < 3; i++) {
			axis[i] = (*this)[i] / a;
		}
	}
	else {
		axis.Set(0.0, 0.0, 0.0);
	}
}

// 插值。从当前四元数插值到Vend四元数，t是参数[0, 1]
CQuaternion217 CQuaternion217::Slerp(const CQuaternion217& Vend, float t) {
	float w = Vend.w - (*this).w;
	float x = Vend.x - (*this).x;
	float y = Vend.y - (*this).y;
	float z = Vend.z - (*this).z;
	CQuaternion217 res((*this).x + t * x, (*this).y + t * y, (*this).z + t * z, (*this).w + t * w);
	res.Normalize();
	return res;
}

// 插值一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中。
void CQuaternion217::Slerp(const CQuaternion217& Vend, int n, float* t, CQuaternion217* Result) {
	float w = Vend.w - (*this).w;
	float x = Vend.x - (*this).x;
	float y = Vend.y - (*this).y;
	float z = Vend.z - (*this).z;
	CQuaternion217 q;
	for (int i = 0; i < n; i++) {
		q = (*this).Slerp(Vend, t[i]);
		Result[i] = q;
	}
}

// 转化为欧拉角
CEuler217 CQuaternion217::ToEuler() {
	CEuler217 res;
	res[1] = asin(2 * w * x - 2 * y * z) * 180 / PI;
	if (!(res[1] == 90.0 || res[1] == -90.0)) {
		res[0] = atan2(2 * z * x + 2 * w * y, 1 - 2 * x * x - 2 * y * y) * 180 / PI;
		res[2] = atan2(2 * x * y + 2 * w * z, 1 - 2 * z * z - 2 * x * x) * 180 / PI;
	}
	else {
		res[0] = atan2(2 * w * y - 2 * z * x, 1 - 2 * y * y - 2 * z * z) * 180 / PI;
		res[2] = 0.0;
	}
	return res;
}

// 转化为矩阵
CMatrix217 CQuaternion217::ToMatrix() {
	CMatrix217 res;
	res[0] = 1 - 2 * y * y - 2 * z * z;
	res[1] = 2 * x * y + 2 * w * z;
	res[2] = 2 * x * z - 2 * w * y;
	res[3] = 0.0;
	res[4] = 2 * x * y - 2 * w * z;
	res[5] = 1 - 2 * x * x - 2 * z * z;
	res[6] = 2 * y * z + 2 * w * x;
	res[7] = 0.0;
	res[8] = 2 * x * z + 2 * w * y;
	res[9] = 2 * y * z - 2 * w * x;
	res[10] = 1 - 2 * x * x - 2 * y * y;
	res[11] - 0.0;
	res[12] = 0.0;
	res[13] = 0.0;
	res[14] = 0.0;
	res[15] = 1.0;
	return res;
}