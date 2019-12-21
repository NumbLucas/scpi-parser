/********************************************************************************* 
  *	Copyright(C),UESTC
  *	FileName: SCPI_PARSER.cpp
  *	Author:  Wei.guo
  *	Version:  1.0.0 
  *	Date:  2019-10-10 
  *	Description:  ��������࣬����������������Ԥ����
  *	Others:  //��������˵�� 
  *	History:  
**********************************************************************************/  
#include "SCPI_PARSER.h"
#include "handle_multi_cmd.h"
#include "scpi_def.h"
#include <vector>
#include <iostream>

using namespace std;


/*
��������isAllNum
�� �ܣ��ж��ַ����Ƿ�ȫΪ����
�� ��1��s		�����ַ���	
����ֵ��0��1��
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
	this->interface = &my_scpi_interface;
	//string s;
}

SCPI_PARSER::~SCPI_PARSER(void)
{
}

/*
��������parserCommonCmdHeader
�� �ܣ�����ͨ������
�� ��1��context		��������������
�� ��1��header		���������ͷ��
�� ��1��root		    ������
�� ��1��param		��������Ĳ���
����ֵ��0ʧ�ܣ�1�ɹ�
**/
bool SCPI_PARSER::parserCommonCmdHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param) {
	string s = header.substr(1, header.size()-1);
	int index = 0;
	SCPI_CMD_NODE* temp = root->getNode(s, index);

	if(temp == NULL)
		return false;
	if(temp->getIsEnd()) {
			//TODO ��ǰ�������
		temp->getCallackFunc()(context);
		return true;
	}
	return false;
}

/*
��������parserRgsCmdHeader
�� �ܣ������豸ר������
�� ��1��context		��������������
�� ��1��header		���������ͷ��
�� ��1��root		    ������
�� ��1��param		��������Ĳ���
����ֵ��0ʧ�ܣ�1�ɹ�
**/
bool SCPI_PARSER::parserRgsCmdHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param) {
	vector<string> v;
	SCPI_CMD_NODE* current = root;
	string s;

	splitStr(header, ':', v);
	int subChoiceLevel = 0;

	for(int i = 0;i < v.size();i++) {
		s = v.at(i);
		
		memcpy_s(context->cmds[context->cmds_num++], MAX_CMD_LENGTH-1, s.c_str(), s.length());
		//strspn
		if(isAllNum(s)) {
			context->sub_cmd_num = atoi(s.c_str());
			continue;
		}
		//[GENA|GENB|GENC|GEND] index��ʾѡ���Ӧ������
		int index = -1;
		SCPI_CMD_NODE* temp = current->getNode(s, index);
		if(temp == NULL) {
			if(this->interface&&this->interface->error)
				this->interface->error(context, -1);
			return false;
		}
		if(index != -1) {//������ѡ��������
			if(subChoiceLevel<MAX_SUB_CMD_CHOICE_LEVEL)
				context->cmd_chioce_index[subChoiceLevel++] = index;
			else 
				printf("unsupported subChoiceLevel size larger than %d",MAX_SUB_CMD_CHOICE_LEVEL);
		}

		//context->subCmdChioce[subChoiceLevel] = 
		
		if(temp->getIsEnd()&&i == v.size()-1) {
			//TODO ��ǰ�������
			processContext(context, header, param);
			temp->getCallackFunc()(context);
			return true;
		}
			
		current = temp;
	}
	if(this->interface&&this->interface->error)
		this->interface->error(context, -2);
	return false;
}

/*
��������parserHeader
�� �ܣ����ݵ�һ���ַ�������������*Ϊͨ�������Ϊ�豸ר������
�� ��1��context		��������������
�� ��1��header		���������ͷ��
�� ��1��root		    ������
�� ��1��param		��������Ĳ���
����ֵ��0ʧ�ܣ�1�ɹ�
**/
bool SCPI_PARSER::parserHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param) {

	if(header.at(0) == '*')
		return parserCommonCmdHeader(context, header, root, param);
	else if(header.at(0) == ':')
		return parserRgsCmdHeader(context, header, root, param);

	return false;
}

/*
��������parser
�� �ܣ����������Ԥ�����ָ�����ͷ����������Ƴ��ո��
�� ��1��context		��������������
�� ��1��cmd			��������
�� ��1��root		    ������
����ֵ��0ʧ�ܣ�1�ɹ�
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
��������processContext
�� �ܣ���������Ԥ����
�� ��1��context		��������������
�� ��1��header			��������ͷ��
�� ��1��param		    �����������
����ֵ��0ʧ�ܣ�1�ɹ�
**/
void SCPI_PARSER::processContext(scpi_t* context, string header, string param) {
	//cout<<header<<endl;
	//cout<<param<<endl;
}