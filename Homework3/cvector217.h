#pragma once
class CEuler217;

class CVector217 {
public:
	float x;
	float y;
	float z;
	CVector217();//�޲ι��캯��
	CVector217(float *a);//�вι��캯��
	CVector217(CVector217 &a);//�������캯��
	void Set(float tx, float ty, float tz);//��ֵ����
	CVector217 operator+(CVector217 &p);//���ؼӺ�
	CVector217 operator-(CVector217 &p);//���ؼ���
	CVector217 operator=(CVector217 &p);//���ظ�ֵ��
	int operator==(CVector217 &p);//������ȷ���
	int operator!=(CVector217 &p);//���ز��ȷ���
	operator float*();//����ת��
	CVector217 operator*(float n);//����
	float dotMul(CVector217 &n);//���
	CVector217 crossMul(CVector217 &n);//���
	void Normalize();//��λ��
	float len();//��ģ
	CVector217 project(CVector217 &n);//ͶӰ
	CEuler217 ToEuler();//ת��Ϊŷ����
};

