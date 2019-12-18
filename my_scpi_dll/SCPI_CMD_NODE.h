#pragma once
#include <vector>
#include "types.h"

using namespace std;

class SCPI_CMD_NODE
{
public:
	SCPI_CMD_NODE(void);
	SCPI_CMD_NODE(string longCmd);
	SCPI_CMD_NODE(string longCmd, bool isEnd);
	virtual ~SCPI_CMD_NODE(void);

	void addCmdSubNode(SCPI_CMD_NODE* node);
	void addCmdPattern(string cmdPattern, scpi_command_callback_t callback);
	
	//bool containsSubNode(SCPI_CMD_NODE* node);
	bool containsSubNode(string longCmd);
	SCPI_CMD_NODE* getNode(string cmd);
	string getLongCmd() { return longCmd;}
	string getShortCmd() { return shortCmd;}
	bool getIsEnd() { return isEnd;}
	bool getHasParam() { return hasParam;}

public:

	vector<SCPI_CMD_NODE*> subCmdList; //������������
	string longCmd;
	string shortCmd;
	bool isEnd;//�Ƿ��Ѵ������β
	bool hasParam;//�������Ƿ��������
	scpi_command_callback_t call_back_func;//ֻ�е�isEnd Ϊ Trueʱ��Ч����ʾ��������Ļص�������
};

void levelOrder(SCPI_CMD_NODE* root);
void parserCmd(SCPI_CMD_NODE* root, string cmd);


