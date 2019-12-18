/********************************************************************************* 
  *	Copyright(C),UESTC
  *	FileName: SCPI_CMD_NODE.cpp
  *	Author:  Wei.guo
  *	Version:  1.0.0 
  *	Date:  2019-10-10 
  *	Description:  单个命令结点类，主要用于构建命令树，具有添加子节点的功能
  *	Others:  //其他内容说明 
  *	History:  
**********************************************************************************/  
#include "SCPI_CMD_NODE.h"
#include "handle_multi_cmd.h"
#include <iostream>
#include <string>
#include <queue>
#include <regex>
#include <algorithm>

using namespace std;

SCPI_CMD_NODE::SCPI_CMD_NODE(void):isEnd(false),hasParam(false),longCmd(""),shortCmd(""){
}

SCPI_CMD_NODE::~SCPI_CMD_NODE(void)
{
}

/*
函数名：SCPI_CMD_NODE
功 能：SCPI_CMD_NODE构造函数，主要包括长短命令的处理
参 数1：node		子命令结点指针	
返回值：写入消息长度
**/
SCPI_CMD_NODE::SCPI_CMD_NODE(string longCmd):isEnd(false),hasParam(false),
											longCmd(longCmd),shortCmd("") 
{
	
	regex cmdPattern("^[A-Z]+[A-Z,a-z,0-9]*");
	if(!regex_match(longCmd, cmdPattern)) {
		cout<<"cmd error: "<<longCmd<<endl;
		//return;
	}
	
	
	unsigned int index = 0;//第一个小写字母
	for (; index < longCmd.length(); index++)
	{
		if(longCmd.at(index)>='a'&&longCmd.at(index)<='z') {
			break;
		}
	}
	

	if(index == 0)
		cout<<"cmd error: "<<longCmd<<endl;
	else{
		string s = longCmd.substr(0, index);
		if(longCmd.at(longCmd.size()-1) == '?')
			s = s + "?";
		this->shortCmd = s;
	}
	//特殊处理
	if(0 == strcmp("RGS2000NG", longCmd.c_str())) 
		this->shortCmd = "RGS";

	transform(longCmd.begin(), longCmd.end(), longCmd.begin(), toupper);
	this->longCmd = longCmd;
}

SCPI_CMD_NODE::SCPI_CMD_NODE(string longCmd, bool isEnd) {
}

/*
函数名：addCmdSubNode
功 能：添加当前命令结点的子命令结点
参 数1：node		子命令结点指针	
返回值：写入消息长度
**/
void SCPI_CMD_NODE::addCmdSubNode(SCPI_CMD_NODE* node) {
	//if(containsSubNode(node))
		//return;
	subCmdList.push_back(node);
}

/*
bool SCPI_CMD_NODE::containsSubNode(SCPI_CMD_NODE* node) {
	for (unsigned int i = 0; i < subCmdList.size(); i++)
	{
		if(subCmdList.at(i)->longCmd.compare(node->longCmd) == 0)
			return true;
	}
	return false;
}
*/

/*
函数名：containsSubNode
功 能：判断cmd是否为当前命令的下一级命令
参 数1：cmd		(长/短)命令
返回值：0否，1是
**/
bool SCPI_CMD_NODE::containsSubNode(string cmd) {
	for (unsigned int i = 0; i < subCmdList.size(); i++)
	{
		if(subCmdList.at(i)->longCmd.compare(cmd) == 0||
			subCmdList.at(i)->shortCmd.compare(cmd) == 0)
			return true;
	}
	return false;
}

/*
函数名：getNode
功 能：返回与cmd对应的下一级命令结点
参 数1：cmd		命令字符串	
返回值：与cmd对应的命令结点，若没有则返回null
**/
SCPI_CMD_NODE* SCPI_CMD_NODE::getNode(string cmd) {
	for (unsigned int i = 0; i < subCmdList.size(); i++)
	{
		transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);//2019-10-17 partternzhong长短字符均为大写，比较式需将命令转为大写

		string s = subCmdList.at(i)->longCmd;
		if(s.compare(cmd) == 0||
			subCmdList.at(i)->shortCmd.compare(cmd) == 0)
			return subCmdList.at(i);
	}
	return NULL;
}

/*
函数名：addCmdPattern
功 能：往命令树添加命令原型
参 数1：cmdPattern		命令原型	
参 数2：call_back_func	处理函数
返回值：
**/
void SCPI_CMD_NODE::addCmdPattern(string cmdPattern, scpi_command_callback_t call_back_func) {
	vector<string> v;
	if(cmdPattern.size() <1)
		return;
	if(cmdPattern.at(0) == '*')
		v.push_back(cmdPattern.substr(1, cmdPattern.size()-1));
	else
		splitStr(cmdPattern, ':', v);
	SCPI_CMD_NODE* current = this;
	bool hasParamFlag = false;
	for (unsigned int i = 0; i < v.size(); i++){
		if(current == NULL)
			cout<<"null pointer"<<endl;


		if(v.at(i).compare("[]") == 0) {//  []为单个子命令有参数的保留，不将[]加入命令集
			hasParamFlag = true;
			continue;
		}

		SCPI_CMD_NODE* temp = current->getNode(v.at(i));
		if(temp == NULL) {
			temp = new SCPI_CMD_NODE(v.at(i));
			current->addCmdSubNode(temp);
		}
		if(hasParamFlag){
			hasParamFlag = false;
			temp->hasParam = true;
		}
		if(i == v.size()-1) {
			temp->isEnd = true;
			//scpi_t t;
			//call_back_func(&t);
			temp->call_back_func = call_back_func;
		}
		current = temp;
	}
}

/*
函数名：levelOrder
功 能：层次遍历树结构
参 数1：root 命令树根节点	

返回值：
**/
void levelOrder(SCPI_CMD_NODE* root) {
	if(root == NULL)
		return;
	cout<<"level order start"<<endl;
	queue <SCPI_CMD_NODE*> q;
	q.push(root);
	while(!q.empty()) {
		SCPI_CMD_NODE* temp = q.front();
		q.pop();

		cout<<temp->longCmd;
		if(temp->hasParam) {
			cout<<" hasParam";
		}
			
		cout<<endl;
		for(unsigned int i = 0;i < temp->subCmdList.size();i++) {
			q.push(temp->subCmdList.at(i));
		}
	}
	cout<<"level order end"<<endl;
}



