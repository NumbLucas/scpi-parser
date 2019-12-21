#pragma once
#include <vector>
#include <unordered_map>
#include <hash_map>
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

	void levelOrder(SCPI_CMD_NODE* root);
	void parserCmd(SCPI_CMD_NODE* root, string cmd);


	//bool containsSubNode(SCPI_CMD_NODE* node);
	bool containsSubNode(string longCmd);
	SCPI_CMD_NODE* getNode(string cmd, int& index);
	string getLongCmd() { return longCmd;}
	string getShortCmd() { return shortCmd;}
	bool getIsEnd() { return isEnd;}
	bool getIsNum() { return isNum;}
	bool getIsChioce() { return isChioce;}
	scpi_command_callback_t getCallackFunc() { return call_back_func; }
public:
	vector<SCPI_CMD_NODE*> subCmdList; //������������
	//hash_map<string, int> cmdChoice; 
	vector<string> cmdChoice;//ͬһ��ε���������磬[GENA|GENB|GENC]
	string longCmd;
	string shortCmd;
	bool isEnd;//�Ƿ��Ѵ������β
	bool isNum;//�����Ƿ������������
	bool isChioce;//�����Ƿ���ѡ����
	scpi_command_callback_t call_back_func;//ֻ�е�isEnd Ϊ Trueʱ��Ч����ʾ��������Ļص�������
};




