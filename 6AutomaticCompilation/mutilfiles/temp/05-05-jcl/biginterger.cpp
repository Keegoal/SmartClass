#include "biginterger.h"
#include<iostream>
#include<assert.h>
#include<math.h>
# include<stdlib.h>

using namespace std;
/******************************************************
* Des: 从用户输入获得一个大整数结构
* @Argu:字符串 [提示信息]
* Returns:指针  大整数结构
*******************************************************/
BINT_t* BINT_t::Bint_input(const char hint[]) {
	//输入的缓存 
	char input[MAX_BIT_LEN + 1];

	//大数结构数组 
	BINT_t* ret = new BINT_t;
	//交互 
	printf("[%s] 请输入一个大整数(%d位以内)\n", hint, MAX_BIT_LEN);
	std::cin>>input;

	//游标  计算长度 
	char* p = input;

	/*符号判断*/
	switch (*p) {
	case '-':ret->sign = NEG; p++; break;
	case '+':ret->sign = POS; p++; break;
	default: ret->sign = POS;
	}
	/*计算长度并赋值*/
	int len_cnt = 0;
	char begin_flag = 1;
	while (*p != '\0') {
		if (*p > ASCII_NUM_9 || *p < ASCII_NUM_0) {
			printf(":::输入错误 非数字\n");
			free(ret);
			return NULL;
		}
		else if (*p == ASCII_NUM_0 && begin_flag) {
			p++;
			continue;
		}
		else {
			begin_flag = 0;
			ret->numer[len_cnt] = Fix(*p);
			p++;
			len_cnt++;
		}
	}
	//全为0
	if (*p == '\0' && begin_flag == 1) {
		ret->numer[0] = 0;
		ret->len = len_cnt = 1;
	}

	//置len位 
	ret->len = len_cnt;

	//设置剩余的为0x88 -120 	
	memset(&(ret->numer[len_cnt]), NOT_NUM, MAX_BIT_LEN - len_cnt);

#ifdef _DEBUG 
	for (int i = 0; i < ret->len; i++) {
		printf("%d ", ret->numer[i]);
	}
	printf("\nlen:%d", ret->len);
	printf(" sign:%d\n", ret->sign);
#endif

	return ret;
}
/******************************************************
* Des: 把BINT转化为数字形式输出
* @Argu:BINT_t* 要输出的数字
* @Returns:void
*******************************************************/
void BINT_t::Bint_output() {

	if (this->sign == NEG) {
		cout<<"-";
	}
	long long i;
	for (i = 0; i < this->len; i++) {
		cout<<(int)this->numer[i];
	}
	cout << "\n";
}
/******************************************************
* Des: 无符号的加法
*******************************************************/
BINT_t* BINT_t::Bint_Unsigned_ADD(BINT_t* op1, BINT_t* op2) {
	unsigned long long maxLen, rstLen, gap;
	BINT_t* rst, * L, * S;
	bitDec* output;

	assert(op1 != NULL && op2 != NULL);

	//获得最大长度  间距 
	maxLen = (op1->len > op2->len) ? op1->len : op2->len;
	gap = abs(long long(op1->len - op2->len));

	//最长与最短指针 
	L = maxLen == op1->len ? op1 : op2;
	S = maxLen == op1->len ? op2 : op1;

	//结果 结构 
	rst = new BINT_t;


	//结果缓存数组 留一位作为进位预留 
	rstLen = maxLen + 1;
	output = new bitDec(sizeof(bitDec) * rstLen);

	//首位 作为进位预留 
	output[0] = 0;

	//非进位相加 
	long long i;
	for (i = 0; i < gap; i++) {
		output[i + 1] = L->numer[i];
	}
	for (i = gap; i < maxLen; i++) {
		//rst[i]=l[i]+s[i-gap] 
		output[i + 1] = L->numer[i] + S->numer[i - gap];
	}
#ifdef _DEBUG	
	cout<<"output: ";
	for (i = 0; i < maxLen + 1; i++) {
		cout<< (int)output[i];
	}
	cout<<"\n";
#endif

	//循环进位处理 
	for (i = maxLen; i > 0; i--) {
		if (output[i] >= 10) {
			output[i] -= 10;
			output[i - 1] += 1;
		}
	}

	//首位0判断 
	if (output[0] == 0) {
		output++;
		rstLen--;
	}

	//输出赋值 
	memcpy(rst->numer, output, rstLen);
	memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
	rst->sign = POS;//符号位为正 
	rst->len = rstLen;

	//释放局部资源 
	//free(output);

#ifdef _DEBUG
	printf("无符号加法结果：\n");
	for (i = 0; i < rst->len; i++) {
		printf("%d ", rst->numer[i]);
	}
	printf("\nsign:%d len:%d\n", rst->sign, rst->len);
#endif
	return rst;
}

/******************************************************
* Des: 无符号 减法
*******************************************************/
BINT_t* BINT_t::Bint_Unsigned_SUB(BINT_t* op1, BINT_t* op2) {
	unsigned long long maxLen = 0, gap, Zero_cnt = 0;
	BINT_t* rst, * L, * S;
	bitDec* output;

	assert(op1 != NULL && op2 != NULL);

	//获得最大长度  间距 
	maxLen = (op1->len > op2->len) ? op1->len : op2->len;
	gap = abs(long long(op1->len - op2->len));

	//最长与最短指针 
	L = maxLen == op1->len ? op1 : op2;
	S = maxLen == op1->len ? op2 : op1;

	//结果 结构 
	rst = (BINT_t*)malloc(sizeof(BINT_t));

	//结果缓存数组 
	output = (bitDec*)malloc(sizeof(bitDec) * (maxLen));

	long long i, j;
	for (i = 0; i < gap; i++) {
		output[i] = L->numer[i];
	}
	for (i = gap; i < maxLen; i++) {
		//rst[i]=l[i]-s[i-gap] 
		output[i] = L->numer[i] - S->numer[i - gap];
	}

	//循环进位处理 
	for (i = maxLen - 1; i > 0; i--) {
		if (output[i] < 0) {
			output[i] += 10;
			output[i - 1] -= 1;
		}
	}


	//首位0统计 
	for (i = 0; output[i] == 0 && i < maxLen; i++) {
		Zero_cnt++;
	}
	//全为0 
	if (Zero_cnt == maxLen) {
		//输出0 
		rst->numer[0] = 0;
		memset(&(rst->numer[1]), NOT_NUM, MAX_BIT_LEN - 1);
		rst->len = 1;
	}
	else {
		for (i = Zero_cnt, j = 0; i < maxLen; i++, j++) {
			rst->numer[j] = output[i];
		}
		//memcpy(rst->numer,&output[Zero_cnt],maxLen-Zero_cnt);
		memset(&(rst->numer[j]), NOT_NUM, MAX_BIT_LEN - maxLen + Zero_cnt);
		rst->len = maxLen - Zero_cnt;
	}

	//符号位赋值 
	rst->sign = L == op1 ? POS : NEG;

	//释放局部资源
	if (output != nullptr)
	{
		delete[] output;
	}

#ifdef _DEBUG	
	printf("无符号减法结果: \n");
	for (i = 0; i < rst->len; i++) {
		printf("%d ", rst->numer[i]);
	}
	printf("\nsign:%d len:%d \n", rst->sign, rst->len);
#endif

	return rst;
}

BINT_t BINT_t::operator+(BINT_t op2)
{
	BINT_t* rst=nullptr;

	assert(this != NULL && &op2 != NULL);

	if (this->sign == POS && op2.sign == POS) {
		rst = Bint_Unsigned_ADD(this, &op2);
	}
	else if (this->sign == POS && op2.sign == NEG) {
		rst = Bint_Unsigned_SUB(this, &op2);
	}
	else if (this->sign == NEG && op2.sign == POS) {
		rst = Bint_Unsigned_SUB(&op2, this);
	}
	else if (this->sign == NEG && op2.sign == NEG) {
		rst = Bint_Unsigned_ADD(this, &op2);
		rst->sign = NEG;
	}

#ifdef _DEBUG	
	printf("有符号加法结果: \n");
	for (int i = 0; i < rst->len; i++) {
		printf("%d ", rst->numer[i]);
	}
	printf("\nsign:%d len:%d \n", rst->sign, rst->len);
#endif

	return *rst;
}

BINT_t BINT_t::operator-(BINT_t op2)
{
	BINT_t* rst = nullptr;

	assert(this != NULL && &op2 != NULL);

	if (this->sign == POS && op2.sign == POS) {
		rst = Bint_Unsigned_SUB(this, &op2);
	}
	else if (this->sign == POS && op2.sign == NEG) {
		rst = Bint_Unsigned_ADD(this, &op2);
	}
	else if (this->sign == NEG && op2.sign == POS) {
		rst = Bint_Unsigned_ADD(this, &op2);
		rst->sign = NEG;
	}
	else if (this->sign == NEG && op2.sign == NEG) {
		rst = Bint_Unsigned_SUB(&op2, this);
	
	}
#ifdef _DEBUG	
	cout << "有符号减法结果:" << endl;
	for (int i = 0; i < rst->len; i++) {
		cout << (int)rst->numer[i];
		//printf("%d ", rst->numer[i]);
	}
	cout<<"\nsign:"<<(int)rst->sign<<"len:"<<rst->len<<"\n";
#endif
	return *rst;
}

BINT_t BINT_t::operator*(BINT_t op2)
{
	unsigned long long maxLen, minLen, rstLen;
	BINT_t* rst, * L, * S;
	bitDec* output;
	long long i, j;

	assert(this != NULL && &op2 != NULL);

	//获得最长 最短长度 
	maxLen = (this->len > op2.len) ? this->len : op2.len;
	minLen = (this->len == maxLen) ? op2.len : this->len;

	//结果长度 预留一位进位 
	rstLen = maxLen + minLen;

#ifdef _DEBUG
	cout<<"maxlen:"<< maxLen<<"\t";
	cout<<"minlen:"<< minLen <<"\t";
	cout<<"rstlen:"<< rstLen <<"\n";
#endif

	//最长与最短指针 
	L = maxLen == this->len ? this : &op2;
	S = maxLen == this->len ? &op2 : this;

	//结果 结构 
	rst = (BINT_t*)malloc(sizeof(BINT_t));

	//结果缓存数组 
	output = (bitDec*)malloc(sizeof(bitDec) * (rstLen));
	memset(output, 0, rstLen);

#ifdef _DEBUG
	cout<<"output初始化为:";
	for (int k = 0; k < rstLen; k++) printf("%d ", output[k]);
	printf("\n");
#endif 

	for (i = minLen - 1; i >= 0; i--) {
		//非进制 
		for (j = maxLen - 1; j >= 0; j--) {
			output[i + j + 1] += L->numer[j] * S->numer[i];
		}
#ifdef _DEBUG
		cout<<"["<<i<<"]:";
		for (int k = 0; k < rstLen; k++) printf("%2d ", output[k]);
		cout << "\n";
#endif 
		// 进位
		for (j = maxLen + i; j > i; j--) {
			output[j - 1] += output[j] / 10;
			output[j] = output[j] % 10;
		}
#ifdef _DEBUG
		printf("[F]:", i);
		for (int k = 0; k < rstLen; k++) printf("%2d ", output[k]);
		printf("\n\n");
#endif 
	}

	//预留首进位判断 
	auto p = output;
	if (output[0] == 0) {
		rstLen--;
		output++;
	}

	//赋值给rst 
	memcpy(rst->numer, output, rstLen);
	memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
	rst->sign = (this->sign == op2.sign) ? POS : NEG;
	rst->len = rstLen;

	//释放局部资源 
	delete[] p;

#ifdef _DEBUG	
	printf("乘法结果: \n");
	for (int k = 0; k < rst->len; k++) {
		printf("%d ", rst->numer[k]);
	}
	printf("\nsign:%d len:%d \n", rst->sign, rst->len);
#endif

	return *rst;
}

BINT_t BINT_t::operator/(BINT_t op2)
{
	unsigned long long rstLen, len1, len2;
	BINT_t* rst;
	bitDec* output, * subc;
	long long i, j, k;
	char s0_flag = 0;

	assert(this != NULL && &op2 != NULL);

	if (this->len < op2.len) {
		printf(":::提示，被除数小于除数\n");

		//返回0 
		rst = (BINT_t*)malloc(sizeof(BINT_t));
		rst->numer[0] = 0;
		memset(&(rst->numer[1]), NOT_NUM, MAX_BIT_LEN - 1);
		rst->sign = POS;
		rst->len = 1;

#ifdef _DEBUG
		printf("模余结果：\n");
		for (i = 0; i < rst->len; i++) {
			printf("%d ", rst->numer[i]);
		}
		printf("\nsign:%d len:%d\n", rst->sign, rst->len);
#endif

		return *rst;
	}

	//结果长度 同时预留一个进位 
	rstLen = this->len - op2.len + 1;
	len1 = this->len;
	len2 = op2.len;

	//结果 结构 
	rst = (BINT_t*)malloc(sizeof(BINT_t));

	//结果缓存数组 
	output = (bitDec*)malloc(sizeof(bitDec) * (rstLen));
	memset(output, 0, rstLen);

	//相减缓存 
	subc = (bitDec*)malloc(sizeof(bitDec) * (len2 + 1));
	memset(subc, 0, len2 + 1);


	for (i = 0; i < rstLen; i++) {
		if (i == 0) {
			//第一次取前从op1取len2个数 subc首位为0 
			for (j = 0; j < len2; j++) {
				subc[j + 1] = this->numer[j];
			}
		}
		else {
			//第二次开始
			//前len2个移位并加回 
			for (j = 0; j < len2; j++) {
				subc[j] = subc[j + 1] + op2.numer[j];
			}
			//补充一个进来 
			subc[len2] = this->numer[i + len2 - 1];
		}
#ifdef _DEBUG
		printf("[SUBC]:");
		for (int m = 0; m < len2 + 1; m++)printf("%d ", subc[m]);
		printf("\n");
#endif

		//1-10试减 
		for (j = 1; j <= 10; j++) {
			//减法 
			for (k = 0; k < len2; k++) {
				subc[k + 1] = subc[k + 1] - op2.numer[k];
			}

			//小于0判断
			s0_flag = 0; //是否小于0 
			for (k = 0; k < len2 + 1; k++) {
				//第一个非零数为负数 则一定为负数 
				if (subc[k] < 0) {
					s0_flag = 1;
					break;
				}
				//第一个非零数为正数 则一定为正数 
				else if (subc[k] > 0) {
					break;
				}
			}
#ifdef _DEBUG
			printf("[%d]:", j);
			for (int m = 0; m < len2 + 1; m++)printf("%d ", subc[m]);
			printf("\n");
#endif
			//小于0 写output 
			if (s0_flag) {
				output[i] = j - 1;
				break;
			}
			//否则 负数进制
			for (k = len2; k > 0; k--) {
				if (subc[k] < 0) {
					subc[k] += 10;
					subc[k - 1]--;
				}
			}
		}
#ifdef _DEBUG
		printf("[output]:");
		for (int m = 0; m < rstLen; m++)printf("%d ", output[m]);
		printf("\n");
#endif		 
	}
	auto p1 = output;
	auto p2 = subc;
	//结果首位判断 
	if (output[0] == 0) {
		output++;
		rstLen--;
	}

	//写出
	memcpy(rst->numer, output, rstLen);
	memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
	rst->sign = (this->sign == op2.sign) ? POS : NEG;
	rst->len = rstLen;

	//释放局部资源 
	free(p1);
	free(p2);

#ifdef _DEBUG
	printf("整除结果：\n");
	for (i = 0; i < rst->len; i++) {
		printf("%d ", rst->numer[i]);
	}
	printf("\nsign:%d len:%d\n", rst->sign, rst->len);
#endif

	return *rst;
}
BINT_t* BINT_t::Bint_MOD(BINT_t* op1, BINT_t* op2) {
	unsigned long long rstLen, len1, len2, rLen;
	BINT_t* rst;
	bitDec* subc, * p;
	long long i, j, k;
	char s0_flag = 0;

	assert(op1 != NULL && op2 != NULL);

	if (op1->len < op2->len) {
		printf(":::提示，被除数小于除数\n");

		//返回op1的赋值 
		rst = (BINT_t*)malloc(sizeof(BINT_t));
		memcpy(rst->numer, op1->numer, MAX_BIT_LEN);
		rst->sign = op1->sign;
		rst->len = op1->len;

#ifdef _DEBUG
		printf("模余结果：\n");
		for (i = 0; i < rst->len; i++) {
			printf("%d ", rst->numer[i]);
		}
		printf("\nsign:%d len:%d\n", rst->sign, rst->len);
#endif

		return rst;
	}

	//除数的结果长度 预留一个进位 同时也是循环的长度 
	rLen = op1->len - op2->len + 1;
	len1 = op1->len;
	len2 = op2->len;

	//余数长度 
	rstLen = len2 + 1;

	//结果 结构 
	rst = (BINT_t*)malloc(sizeof(BINT_t));

	//相减缓存 即模余缓存  
	subc = (bitDec*)malloc(sizeof(bitDec) * rstLen);
	memset(subc, 0, rstLen);


	for (i = 0; i < rLen; i++) {
		if (i == 0) {
			//第一次取前从op1取len2个数 subc首位为0 
			for (j = 0; j < len2; j++) {
				subc[j + 1] = op1->numer[j];
			}
		}
		else {
			//第二次开始
			//前len2个移位并加回 
			for (j = 0; j < len2; j++) {
				subc[j] = subc[j + 1] + op2->numer[j];
			}
			//补充一个进来 
			subc[len2] = op1->numer[i + len2 - 1];
		}
#ifdef _DEBUG
		printf("[SUBC]:");
		for (int m = 0; m < len2 + 1; m++)printf("%d ", subc[m]);
		printf("\n");
#endif

		//1-10试减 
		for (j = 1; j <= 10; j++) {
			//减法 
			for (k = 0; k < len2; k++) {
				subc[k + 1] = subc[k + 1] - op2->numer[k];
			}

			//小于0判断
			s0_flag = 0; //是否小于0 
			for (k = 0; k < rstLen; k++) {
				//第一个非零数为负数 则一定为负数 
				if (subc[k] < 0) {
					s0_flag = 1;
					break;
				}
				//第一个非零数为正数 则一定为正数 
				else if (subc[k] > 0) {
					break;
				}
			}
#ifdef _DEBUG
			printf("[%d]:", j);
			for (int m = 0; m < rstLen; m++)printf("%d ", subc[m]);
			printf("\n");
#endif
			//小于0 写output 
			if (s0_flag) {
				break;
			}
			//否则 负数进制
			for (k = len2; k > 0; k--) {
				if (subc[k] < 0) {
					subc[k] += 10;
					subc[k - 1]--;
				}
			}
		}
	}

	//重加回 
	for (i = 0; i < len2; i++) {
		subc[i + 1] += op2->numer[i];
	}

#ifdef _DEBUG
	printf("[subc]:");
	for (int m = 0; m < rstLen; m++)printf("%d ", subc[m]);
	printf("\n");
#endif	

	//模余首位0判断 
	p = subc;
	while (*p == 0 && rstLen > 0) {
		p++;
		rstLen--;
	}
	//余数为0 
	if (rstLen == 0) {
		//写出0 
		rst->numer[0] = 0;
		memset(&(rst->numer[1]), NOT_NUM, MAX_BIT_LEN - 1);
		rst->sign = POS;
		rst->len = 1;
	}
	else {
		//正常写出
		memcpy(rst->numer, p, rstLen);
		memset(&(rst->numer[rstLen]), NOT_NUM, MAX_BIT_LEN - rstLen);
		rst->sign = (op1->sign == op2->sign) ? POS : NEG;
		rst->len = rstLen;
	}



	//释放局部资源 
	free(subc);

#ifdef _DEBUG
	printf("模余结果：\n");
	for (i = 0; i < rst->len; i++) {
		printf("%d ", rst->numer[i]);
	}
	printf("\nsign:%d len:%d\n", rst->sign, rst->len);
#endif

	return rst;
}

BINT_t& BINT_t::operator=(const BINT_t other)
{
	this->len = other.len;
	this->sign = other.sign;
	memcpy(this->numer, other.numer, MAX_BIT_LEN);
	return *this;
}

bool BINT_t::operator==(BINT_t& other)
{	
	if ((int)( * this - other).numer[0] == 0)
	{
		return true;
	}
	return false;
}

bool BINT_t::operator>(BINT_t& other)
{
	if ((*this - other).sign == 1 && !(*this==other))
	{
		return true;
	}
	return false;
}

bool BINT_t::operator<(BINT_t& other)
{
	if ((*this - other).sign != 1 && !(*this == other))
	{
		return true;
	}
	return false;
}

