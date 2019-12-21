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

SCPI_CMD_NODE::SCPI_CMD_NODE(void):isEnd(false),isNum(false),longCmd(""),shortCmd(""){
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
SCPI_CMD_NODE::SCPI_CMD_NODE(string longCmd):isEnd(false),isNum(false),
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
	

	if(index == 0){
		cout<<"only short cmd "<<longCmd<<endl;
	}
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
返回值：0 失败 1成功
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
		for(int j = 0;j < subCmdList.at(i)->cmdChoice.size();j++) {
			if(subCmdList.at(i)->cmdChoice.at(j).compare(cmd) == 0)
				return true;
		}
	}
	return false;
}

/*
函数名：getNode
功 能：返回与cmd对应的下一级命令结点
参 数1：cmd		命令字符串	
返回值：与cmd对应的命令结点，若没有则返回null
**/
SCPI_CMD_NODE* SCPI_CMD_NODE::getNode(string cmd, int& index) {
	index = -1;
	int siez = this->subCmdList.size();
	for (unsigned int i = 0; i < this->subCmdList.size(); i++)
	{
		
		transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);//2019-10-17 partternzhong长短字符均为大写，比较式需将命令转为大写

		string s = this->subCmdList.at(i)->longCmd;
		SCPI_CMD_NODE* next = subCmdList.at(i);
		if(next->longCmd.compare(cmd) == 0||next->shortCmd.compare(cmd) == 0){
			index = -1;
			return next;
		}
		for(int j = 0;j < subCmdList.at(i)->cmdChoice.size();j++) {
			if(next->cmdChoice.at(j).compare(cmd) == 0){
				index = j;
				return next;
			}
		}
	}
	index = -1;
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
	if(cmdPattern.at(0) == '*'){
		v.push_back(cmdPattern.substr(1, cmdPattern.size()-1));
	}
	else
		splitStr(cmdPattern, ':', v);
	SCPI_CMD_NODE* current = this;
	bool hasParamFlag = false;
	regex choice("^\\[[0-9a-zA-Z]+(\\|[0-9a-zA-Z]+)+\\]$");
	for (unsigned int i = 0; i < v.size(); i++){
		if(current == NULL)
			cout<<"error: null pointer"<<endl;

		string s = v.at(i);
		int index = 0;
		SCPI_CMD_NODE* next = NULL;
		if(s.compare("[]") == 0) {//  []为单个子命令有数字参数的保留，不将[]加入命令集
			hasParamFlag = true;
			continue;
		}
		else if(regex_match(s,choice)){//包含类似[GENA|GENB|GENC]此类选项的命令结点,将选项加入cmdParam，且长短命令为第一个选项
			//current->getNode(v.at(i), index)
			vector<string> params;
			splitStr(s.substr(1, s.length()-2), '|', params);
			next = new SCPI_CMD_NODE();
			for(int j = 0;j < params.size();j++){
				next->cmdChoice.push_back(params.at(j));
				
			}
			current->addCmdSubNode(next);
		}else { //普通命令
			int tmp;
			next = current->getNode(s, tmp);
			if(next == NULL) {
				next = new SCPI_CMD_NODE(s);
				current->addCmdSubNode(next);
			}
		}
		
		if(hasParamFlag){
			hasParamFlag = false;
			next->isNum = true;
		}
		if(i == v.size()-1) {
			next->isEnd = true;
			next->call_back_func = call_back_func;
		}
		current = next;
	}
}

/*
函数名：levelOrder
功 能：层次遍历树结构
参 数1：root 命令树根节点	

返回值：
**/
void SCPI_CMD_NODE::levelOrder(SCPI_CMD_NODE* root) {
	if(root == NULL)
		return;
	cout<<"level order start"<<endl;
	queue <SCPI_CMD_NODE*> q;
	q.push(root);
	while(!q.empty()) {
		SCPI_CMD_NODE* temp = q.front();
		q.pop();

		cout<<temp->longCmd;
		cout<<"(";
		for(int i = 0;i< temp->cmdChoice.size();i++)
			cout<<temp->cmdChoice.at(i);
		cout<<")";
		if(temp->isNum) {
			cout<<" hasParam";
		}
			
		cout<<endl;
		for(unsigned int i = 0;i < temp->subCmdList.size();i++) {
			q.push(temp->subCmdList.at(i));
		}
	}
	cout<<"level order end"<<endl;
}



