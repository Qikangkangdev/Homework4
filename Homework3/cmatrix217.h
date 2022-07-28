#pragma once
class CVector217;
class CEuler217;
class CQuaternion217;

class CMatrix217 {

public:
	CMatrix217();//默认构造函数
	CMatrix217(float *a);//带参数的构造函数
	CMatrix217(CMatrix217 & a);//拷贝构造函数
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;
	void Set(float* val);//给矩阵16个元素赋值
	operator float*();
	CMatrix217 operator=(CMatrix217 &p);//矩阵赋值
	CMatrix217 operator*(float d);//矩阵数乘
	CMatrix217 operator*(CMatrix217 &p);//矩阵相乘
	CVector217 vecMul(CVector217 &p);//矩阵与向量相乘
	CVector217 posMul(CVector217 &p);//矩阵与位置相乘
	void SetRotate(float seta, CVector217 axis);//设置为旋转矩阵
	void SetTrans(CVector217 trans);//设置为平移矩阵
	void SetScale(CVector217 p);//设置为缩放矩阵
	float Inverse();//矩阵求逆，成功返回行列式的值，否则返回0
	CMatrix217 GetInverse();//返回逆矩阵
	CEuler217 ToEuler();//转化为欧拉角
	CQuaternion217 ToQuaternion();//转化为四元数
};
