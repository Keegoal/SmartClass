#include<fstream>
#include<iostream>
#include<string>
using namespace std;
int main()
{
	string t,ans,ans2;
	ofstream ofs;//���Լ�¼�ɼ�
	int g=100;//ÿ��ɼ�����
	int i;
	//freopen("out.txt","r",stdin);//��out1�ļ�
	freopen("./temp/out.txt","r",stdin);//��out1�ļ�
	char c;
	while(scanf("%c",&c)!=EOF) 
	if(c!='\n')
		ans+=c;//��¼�ļ��г��˻س�֮����ַ�
	fclose(stdin);
	freopen("./temp/ans.txt","r",stdin);
	while(scanf("%c",&c)!=EOF) 
	if(c!='\n')
			ans2+=c;//��¼���ļ��г��˻س�֮����ַ�
	fclose(stdin);
	
	ofs.open("./temp/grade1.txt",ios::out | ios::trunc);
	if(ans.size()!=ans2.size())
		{
			ofs<<"�ļ������ʽ����ȷ"<<endl;
			return 0;
		}//�𰸴�С��ͬ��ֱ���д�
	for(i=0;i<ans.size();i++)
	{	
		if(ans[i]!=ans2[i])
			{
				ofs<<"�ɼ�Ϊ0"<<endl;
				return 0;
			}//�ж������Ƿ�һ��
	}
	ofs<<"�ɼ�Ϊ"<<g<<endl;
	return 0;
}
