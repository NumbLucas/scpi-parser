#include "handle_multi_cmd.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


/*
��������handleMultiCmd
�� �ܣ�����
�� ��1��
����ֵ��
**/
int handleMultiCmd(char* cmd) {
	//TODO
	return 0;
}


/*
��������splitStr
�� �ܣ����ָ����ָ��ַ��������������vector��
�� ��1��str   �����ַ���ָ��
�� ��2��seperator  �ָ���
�� ��3��rst   �ָ����ַ�������
����ֵ����
**/
void splitStr(const string& str, const char& separator, vector<string>& rst) {
	if(str.length()<1)
		return;
	string subStr;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if(separator == str[i]) {
			if(!subStr.empty())
				rst.push_back(subStr);
			subStr.clear();
		}else {
			subStr.push_back(str[i]);
		}
	}
	if(!subStr.empty())
		rst.push_back(subStr);
}


/*
��������splitCmdAndParm
�� �ܣ��ָ�������ָ��ͷ���������
�� ��1��str   �����ַ���ָ��
�� ��2��head   ����ͷ��
�� ��3��param   �������
����ֵ����
**/
void splitCmdAndParm(const string& str,string& head, string& param) {
	int start = 0;
	string s = str.substr(0, str.length());
	
	if(str.length() < 1)
		return;
	removeHeadTailSpace(s);
	if(s.length()<1)
		return;
	vector<string> v;
	splitStr(s, ' ', v);
	head = v.at(0);
	if(v.size()<2)
		param = "";
	else {
		param = v.at(1);
	}
	//cout<<param<<endl;
}


/*
��������removeHeadTailSpace
�� �ܣ��Ƴ�ͷβ�ո�
�� ��1��str   �����ַ���ָ��	
����ֵ����
**/
void removeHeadTailSpace(string& str) {
	int start = 0;
	int end =str.length();

	for (unsigned int i = 0; i < str.length(); i++)
	{
		if(' ' != str[i]||'\r'!=str[i])
			break;
		start++;
	}
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if(' ' != str[i]||'\r'!=str[i])
			break;
		end--;
	}
	str = str.substr(start, end);
	
}