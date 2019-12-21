/********************************************************************************* 
  *	Copyright(C),UESTC
  *	FileName: SCPI_CMD_NODE.cpp
  *	Author:  Wei.guo
  *	Version:  1.0.0 
  *	Date:  2019-10-10 
  *	Description:  �����������࣬��Ҫ���ڹ�������������������ӽڵ�Ĺ���
  *	Others:  //��������˵�� 
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
��������SCPI_CMD_NODE
�� �ܣ�SCPI_CMD_NODE���캯������Ҫ������������Ĵ���
�� ��1��node		��������ָ��	
����ֵ��д����Ϣ����
**/
SCPI_CMD_NODE::SCPI_CMD_NODE(string longCmd):isEnd(false),isNum(false),
											longCmd(longCmd),shortCmd("") 
{
	
	regex cmdPattern("^[A-Z]+[A-Z,a-z,0-9]*");
	if(!regex_match(longCmd, cmdPattern)) {
		cout<<"cmd error: "<<longCmd<<endl;
		//return;
	}
	
	
	unsigned int index = 0;//��һ��Сд��ĸ
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
	//���⴦��
	if(0 == strcmp("RGS2000NG", longCmd.c_str())) 
		this->shortCmd = "RGS";

	transform(longCmd.begin(), longCmd.end(), longCmd.begin(), toupper);
	this->longCmd = longCmd;
}

SCPI_CMD_NODE::SCPI_CMD_NODE(string longCmd, bool isEnd) {
}

/*
��������addCmdSubNode
�� �ܣ���ӵ�ǰ���������������
�� ��1��node		��������ָ��	
����ֵ��0 ʧ�� 1�ɹ�
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
��������containsSubNode
�� �ܣ��ж�cmd�Ƿ�Ϊ��ǰ�������һ������
�� ��1��cmd		(��/��)����
����ֵ��0��1��
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
��������getNode
�� �ܣ�������cmd��Ӧ����һ��������
�� ��1��cmd		�����ַ���	
����ֵ����cmd��Ӧ�������㣬��û���򷵻�null
**/
SCPI_CMD_NODE* SCPI_CMD_NODE::getNode(string cmd, int& index) {
	index = -1;
	int siez = this->subCmdList.size();
	for (unsigned int i = 0; i < this->subCmdList.size(); i++)
	{
		
		transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);//2019-10-17 partternzhong�����ַ���Ϊ��д���Ƚ�ʽ�轫����תΪ��д

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
��������addCmdPattern
�� �ܣ����������������ԭ��
�� ��1��cmdPattern		����ԭ��	
�� ��2��call_back_func	������
����ֵ��
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
		if(s.compare("[]") == 0) {//  []Ϊ���������������ֲ����ı���������[]�������
			hasParamFlag = true;
			continue;
		}
		else if(regex_match(s,choice)){//��������[GENA|GENB|GENC]����ѡ���������,��ѡ�����cmdParam���ҳ�������Ϊ��һ��ѡ��
			//current->getNode(v.at(i), index)
			vector<string> params;
			splitStr(s.substr(1, s.length()-2), '|', params);
			next = new SCPI_CMD_NODE();
			for(int j = 0;j < params.size();j++){
				next->cmdChoice.push_back(params.at(j));
				
			}
			current->addCmdSubNode(next);
		}else { //��ͨ����
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
��������levelOrder
�� �ܣ���α������ṹ
�� ��1��root ���������ڵ�	

����ֵ��
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



