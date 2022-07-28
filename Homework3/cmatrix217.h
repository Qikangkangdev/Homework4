#pragma once
class CVector217;
class CEuler217;
class CQuaternion217;

class CMatrix217 {

public:
	CMatrix217();//Ĭ�Ϲ��캯��
	CMatrix217(float *a);//�������Ĺ��캯��
	CMatrix217(CMatrix217 & a);//�������캯��
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;
	void Set(float* val);//������16��Ԫ�ظ�ֵ
	operator float*();
	CMatrix217 operator=(CMatrix217 &p);//����ֵ
	CMatrix217 operator*(float d);//��������
	CMatrix217 operator*(CMatrix217 &p);//�������
	CVector217 vecMul(CVector217 &p);//�������������
	CVector217 posMul(CVector217 &p);//������λ�����
	void SetRotate(float seta, CVector217 axis);//����Ϊ��ת����
	void SetTrans(CVector217 trans);//����Ϊƽ�ƾ���
	void SetScale(CVector217 p);//����Ϊ���ž���
	float Inverse();//�������棬�ɹ���������ʽ��ֵ�����򷵻�0
	CMatrix217 GetInverse();//���������
	CEuler217 ToEuler();//ת��Ϊŷ����
	CQuaternion217 ToQuaternion();//ת��Ϊ��Ԫ��
};
