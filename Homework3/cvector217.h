#pragma once
class CEuler217;

class CVector217 {
public:
	float x;
	float y;
	float z;
	CVector217();//无参构造函数
	CVector217(float *a);//有参构造函数
	CVector217(CVector217 &a);//拷贝构造函数
	void Set(float tx, float ty, float tz);//赋值函数
	CVector217 operator+(CVector217 &p);//重载加号
	CVector217 operator-(CVector217 &p);//重载减号
	CVector217 operator=(CVector217 &p);//重载赋值号
	int operator==(CVector217 &p);//重载相等符号
	int operator!=(CVector217 &p);//重载不等符号
	operator float*();//类型转换
	CVector217 operator*(float n);//数乘
	float dotMul(CVector217 &n);//点乘
	CVector217 crossMul(CVector217 &n);//叉乘
	void Normalize();//单位化
	float len();//求模
	CVector217 project(CVector217 &n);//投影
	CEuler217 ToEuler();//转化为欧拉角
};

