#pragma once
class CMatrix217;
class CVector217;
class CEuler217;

class CQuaternion217 {
public:
	float x, y, z, w;
	CQuaternion217();//Ĭ�Ϲ��캯��
	CQuaternion217(float x, float y, float z, float w);//���ι��캯��
	CQuaternion217(CQuaternion217 &a);//�������캯��
	operator float*();//����ת��
	void Set(float fx, float fy, float fz, float fw);//��ֵ����
	CQuaternion217& operator = (const CQuaternion217& p);//���ظ�ֵ
	CQuaternion217 operator + (const CQuaternion217 &p);//����+
	CQuaternion217 operator* (float data);//��������
	CQuaternion217 operator*(const CQuaternion217 &p);//��Ԫ���˷�
	float dotMul(const CQuaternion217&p);//���
	float len();//��ģ
	void Normalize(); //���׼��
	CQuaternion217& Inverse();//������Ԫ������ı�����
	CQuaternion217 GetInverse();//������Ԫ��,����ı����������µ���Ԫ��
	CQuaternion217 Div(const CQuaternion217& b);//��� ��ǰΪa,��c = a-b
	void GetAngle(float& angle, CVector217& axis);//����ת��ͽǶ�
	CQuaternion217 Slerp(const CQuaternion217& Vend, float t);//��ֵ���ӵ�ǰ��Ԫ��
	//��ֵ��Vend��Ԫ����t�ǲ���[0,1]
	void Slerp(const CQuaternion217& Vend, int n, float* t, CQuaternion217* Result);//��ֵ
	//һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result�С�
	CEuler217 ToEuler();//ת��Ϊŷ����
	CMatrix217 ToMatrix();//ת��Ϊ����
};