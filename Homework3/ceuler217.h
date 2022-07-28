#pragma once
class CVector217;
class CMatrix217;
class CQuaternion217;

class CEuler217 {

public:
	CEuler217(); //默认构造函数
	CEuler217(float h, float p, float b); //带参构造函数
	CEuler217(CEuler217 &a); //拷贝构造函数
	float h, p, b;
	void Set(float fh, float fp, float fb); //为成员变量赋值
	operator float*(); //重载取址操作符
	CVector217 ToVector(); //欧拉角转化为向量
	CMatrix217 ToMatrix(); //欧拉角转化为矩阵
	CQuaternion217 ToQuaternion(); //欧拉角转化为四元数
	void Normal();//欧拉角规范化
};