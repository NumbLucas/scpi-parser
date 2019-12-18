#include "handle_multi_cmd.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


/*
函数名：handleMultiCmd
功 能：保留
参 数1：
返回值：
**/
int handleMultiCmd(char* cmd) {
	//TODO
	return 0;
}


/*
函数名：splitStr
功 能：按分隔符分割字符串，结果保存在vector中
参 数1：str   输入字符串指针
参 数2：seperator  分隔符
参 数3：rst   分割后的字符串数组
返回值：无
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
函数名：splitCmdAndParm
功 能：分割单条命令的指令头与参数部分
参 数1：str   输入字符串指针
参 数2：head   命令头部
参 数3：param   命令参数
返回值：无
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
函数名：removeHeadTailSpace
功 能：移除头尾空格
参 数1：str   输入字符串指针	
返回值：无
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