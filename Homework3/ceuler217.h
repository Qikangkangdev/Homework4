#pragma once
class CVector217;
class CMatrix217;
class CQuaternion217;

class CEuler217 {

public:
	CEuler217(); //Ĭ�Ϲ��캯��
	CEuler217(float h, float p, float b); //���ι��캯��
	CEuler217(CEuler217 &a); //�������캯��
	float h, p, b;
	void Set(float fh, float fp, float fb); //Ϊ��Ա������ֵ
	operator float*(); //����ȡַ������
	CVector217 ToVector(); //ŷ����ת��Ϊ����
	CMatrix217 ToMatrix(); //ŷ����ת��Ϊ����
	CQuaternion217 ToQuaternion(); //ŷ����ת��Ϊ��Ԫ��
	void Normal();//ŷ���ǹ淶��
};