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
	vector<SCPI_CMD_NODE*> subCmdList; //包含的子命令
	//hash_map<string, int> cmdChoice; 
	vector<string> cmdChoice;//同一层次的命令参数如，[GENA|GENB|GENC]
	string longCmd;
	string shortCmd;
	bool isEnd;//是否已达命令结尾
	bool isNum;//命令是否是数字类参数
	bool isChioce;//命令是否是选项类
	scpi_command_callback_t call_back_func;//只有当isEnd 为 True时有效，表示整条命令的回调处理函数
};




