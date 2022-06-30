#pragma once
#include<string.h>
/******************************************************************
 ���� ʹ��char��ʾһ��ʮ�������йص�һЩ�궨��
*******************************************************************/
#define ASCII_NUM_9 57
#define ASCII_NUM_0 48
#define POS  1
#define NEG  0
#define  Fix(n) ((n)-ASCII_NUM_0)
#define NOT_NUM 0x88

/******************************************************************
 һ�����������λ��
*******************************************************************/
#ifndef MAX_BIT_LEN
#define MAX_BIT_LEN 500
#endif
//��ʹ�ö��� 
//#define BintGet(c)  (Bint_input((c))) 
//#define BintShow(b)  (Bint_output((b)))
typedef char bitDec;
class BINT_t
{
public:
	bitDec  numer[MAX_BIT_LEN];//ʮ�������� 
	char  sign;//����λ 
	unsigned long long len; //���� 
	//����  
	BINT_t* Bint_input(const char hint[]);
	void Bint_output();
	//�޷��żӼ�
	BINT_t* Bint_Unsigned_ADD(BINT_t* op1, BINT_t* op2);
	BINT_t* Bint_Unsigned_SUB(BINT_t* op1, BINT_t* op2);
	//�з��żӼ� 
	BINT_t operator+(BINT_t op2);
	BINT_t operator-(BINT_t op2);
	//�˷� ���� 
	BINT_t operator*(BINT_t op2);
	BINT_t operator/(BINT_t op2);
	//ģ��
	BINT_t* Bint_MOD(BINT_t* op1, BINT_t* op2);

	BINT_t& operator=(const BINT_t other);
	bool operator==(BINT_t& other);
	bool operator>(BINT_t& other);
	bool operator<(BINT_t& other);
};

