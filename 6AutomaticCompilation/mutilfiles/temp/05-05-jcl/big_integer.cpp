/*
* ��Ŀ���ƣ�����������������أ���Ԫ���֣�
* ��    �룺��������
* ��    ��������������������������
* ʱ    �䣺2022��5��12��
*/
#include"biginterger.h"
#include <cstddef>
#include<iostream>
/*************************************
 ������Զ����Լ����λ��
 #define MAX_BIT_LEN  XXX
 ************************************/
using namespace std;
int main() {
	BINT_t* op1=nullptr;
	BINT_t* op2=nullptr;
	BINT_t rst;
	
	while ((op1 = op1->Bint_input("��һ��������")) == NULL);
	while ((op2 = op2->Bint_input("��һ��������")) == NULL);
	rst = *op1 + *op2;
	/*rst.Bint_output();*/
	rst = *op1 * *op2;
	rst = *op1 - *op2;
	rst = *op1 / *op2;
	cout << "----------�Ƚϣ���ȣ�------"<<endl;
	if (*op1==*op2)
	{
		std::cout << "Equal";
	}
	else
	{
		cout << "Unequal";
	}
	return 0;
}