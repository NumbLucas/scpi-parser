/********************************************************************************* 
  *	Copyright(C),UESTC
  *	FileName: SCPI_PARSER.cpp
  *	Author:  Wei.guo
  *	Version:  1.0.0 
  *	Date:  2019-10-10 
  *	Description:  命令解析类，用于命令解析与参数预处理
  *	Others:  //其他内容说明 
  *	History:  
**********************************************************************************/  
#include "SCPI_PARSER.h"
#include "handle_multi_cmd.h"
#include <vector>
#include <iostream>

using namespace std;


/*
函数名：isAllNum
功 能：判断字符串是否全为数字
参 数1：s		输入字符串	
返回值：0否，1是
**/
bool isAllNum(string s) {
	for(unsigned int i = 0;i < s.length();i++) {
		if(s.at(i)<'0'||s.at(i)>'9')
			return false;
	}
	return true;

}

SCPI_PARSER::SCPI_PARSER(scpi_interface_t* interface)
{
	this->interface = interface;
}

SCPI_PARSER::SCPI_PARSER(void)
{
	string s;
}

SCPI_PARSER::~SCPI_PARSER(void)
{
}

/*
函数名：parserCommonCmdHeader
功 能：解析通用命令
参 数1：context		该条命令上下文
参 数1：header		输入命令的头部
参 数1：root		    命令树
参 数1：param		输入命令的参数
返回值：0失败，1成功
**/
bool SCPI_PARSER::parserCommonCmdHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param) {
	string s = header.substr(1, header.size()-1);
	SCPI_CMD_NODE* temp = root->getNode(s);

	if(temp == NULL)
		return false;
	if(temp->isEnd) {
			//TODO 提前处理参数
		temp->call_back_func(context);
		return true;
	}
	return false;
}

/*
函数名：parserRgsCmdHeader
功 能：解析设备专有命令
参 数1：context		该条命令上下文
参 数1：header		输入命令的头部
参 数1：root		    命令树
参 数1：param		输入命令的参数
返回值：0失败，1成功
**/
bool SCPI_PARSER::parserRgsCmdHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param) {
	vector<string> v;
	SCPI_CMD_NODE* current = root;
	string s;

	splitStr(header, ':', v);


	for(int i = 0;i < v.size();i++) {
		s = v.at(i);
		
		memcpy_s(context->cmds[context->cmds_num++], MAX_CMD_LENGTH-1, s.c_str(), s.length());
		//strspn
		if(isAllNum(s)) {
			context->subCmdParam = atoi(s.c_str());
			continue;
		}

		SCPI_CMD_NODE* temp = current->getNode(s);
		if(temp == NULL) {
			this->interface->error(context, 0);
			return false;
		}
		
		if(temp->isEnd&&i == v.size()-1) {
			//TODO 提前处理参数
			processContext(context, header, param);
			temp->call_back_func(context);
			return true;
		}
			
		current = temp;
	}
	this->interface->error(context, 0);
	return false;
}

/*
函数名：parserHeader
功 能：根据第一个字符来分类解析命令，*为通用命令，：为设备专有命令
参 数1：context		该条命令上下文
参 数1：header		输入命令的头部
参 数1：root		    命令树
参 数1：param		输入命令的参数
返回值：0失败，1成功
**/
bool SCPI_PARSER::parserHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param) {

	if(header.at(0) == '*')
		return parserCommonCmdHeader(context, header, root, param);
	else if(header.at(0) == ':')
		return parserRgsCmdHeader(context, header, root, param);

	return false;
}

/*
函数名：parser
功 能：命令解析，预处理，分割命令头部与参数，移除空格等
参 数1：context		该条命令上下文
参 数1：cmd			输入命令
参 数1：root		    命令树
返回值：0失败，1成功
**/
bool SCPI_PARSER::parser(scpi_t* context, string cmd, SCPI_CMD_NODE* root) {
	string header,param;
	removeHeadTailSpace(cmd);
	splitCmdAndParm(cmd, header, param);
	context->header = header.c_str();
	context->param = param.c_str();
	
	return parserHeader(context, header, root, param);
}
/*
函数名：processContext
功 能：参数解析预处理
参 数1：context		该条命令上下文
参 数1：header			输入命令头部
参 数1：param		    命令参数部分
返回值：0失败，1成功
**/
void SCPI_PARSER::processContext(scpi_t* context, string header, string param) {
	//cout<<header<<endl;
	//cout<<param<<endl;
}