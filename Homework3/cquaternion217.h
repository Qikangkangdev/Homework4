#pragma once
class CMatrix217;
class CVector217;
class CEuler217;

class CQuaternion217 {
public:
	float x, y, z, w;
	CQuaternion217();//默认构造函数
	CQuaternion217(float x, float y, float z, float w);//带参构造函数
	CQuaternion217(CQuaternion217 &a);//拷贝构造函数
	operator float*();//类型转化
	void Set(float fx, float fy, float fz, float fw);//赋值函数
	CQuaternion217& operator = (const CQuaternion217& p);//重载赋值
	CQuaternion217 operator + (const CQuaternion217 &p);//重载+
	CQuaternion217 operator* (float data);//重载数乘
	CQuaternion217 operator*(const CQuaternion217 &p);//四元数乘法
	float dotMul(const CQuaternion217&p);//点乘
	float len();//求模
	void Normalize(); //求标准化
	CQuaternion217& Inverse();//求逆四元数，会改变自身
	CQuaternion217 GetInverse();//求逆四元数,不会改变自身，生成新的四元数
	CQuaternion217 Div(const CQuaternion217& b);//求差 当前为a,求c = a-b
	void GetAngle(float& angle, CVector217& axis);//求旋转轴和角度
	CQuaternion217 Slerp(const CQuaternion217& Vend, float t);//插值。从当前四元数
	//插值到Vend四元数，t是参数[0,1]
	void Slerp(const CQuaternion217& Vend, int n, float* t, CQuaternion217* Result);//插值
	//一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中。
	CEuler217 ToEuler();//转化为欧拉角
	CMatrix217 ToMatrix();//转化为矩阵
};