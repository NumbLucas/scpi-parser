#include <iostream>
#include "handle_multi_cmd.h"
#include "SCPI_CMD_NODE.h"
#include "SCPI_PARSER.h"
#include "rgs_cmd_func.h"
#include <sys/timeb.h>
#include "scpi_api.h"
#include <windows.h>
#include "TestParamParser.h"

using namespace std;
static int cmd_num = 0;

int printCurrentTime()     
{    
	SYSTEMTIME sys;   
	GetLocalTime( &sys );
	printf("%ds:%dms \n", sys.wSecond, sys.wMilliseconds);
	return 0;
}

void dfs(char* (*data)[5], int level, bool  (*b)[5], char*cmd, SCPI_CMD_NODE* root) {
	if(level == 5) {
		//cout<<cmd<<endl;
		cmd_num++;
		root->addCmdPattern(cmd, SCPI_RGS_FUNC_TEST);
		memset(cmd, '\0', 100);
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		if(b[level][i])
			continue;
		b[level][i] = true;
		
		char temp[100];
		strcpy(temp, cmd);
		strcat(temp, ":");
		strcat(temp, data[level][i]);
		dfs(data, level+1, b, temp, root);
		b[level][i] = false;
	}
}
int main() {
	/*
	cout<<"hello world"<<endl;
	vector<string> v;
	splitStr("this is a test str", ' ', v);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		cout<<v.at(i)<<endl;
	}
	string s = " wer asd fe  ";
	string rst;
	removeHeadTailSpace(s);
	cout<<s<<endl;
	string cmd;
	string param;
	splitCmdAndParm("  rngs:21:re:wq   32,43,12   ", cmd, param);
	cout<<"cmd: "<<cmd<<endl;
	cout<<"param: "<< param<<endl;
	

	SCPI_CMD_NODE root;
	root.addCmdSubNode(new SCPI_CMD_NODE("xpdr"));
	root.addCmdSubNode(new SCPI_CMD_NODE("measure"));
	root.addCmdSubNode(new SCPI_CMD_NODE("test"));
	cout<<"fs"<<endl;
	
	SCPI_CMD_NODE* xpdr = root.getNode("xpdr");
	SCPI_CMD_NODE* measure = root.getNode("measure");
	SCPI_CMD_NODE* test = root.getNode("test");

	cout<<xpdr->longCmd<<endl;
	cout<<measure->longCmd<<endl;
	cout<<test->longCmd<<endl;
	*/

/*
	SCPI_CMD_NODE root;
	vector<string> v;
	splitStr("rgs:xpdr:freq:on", ':', v);
	SCPI_CMD_NODE* current = &root;

	for (int i = 0; i < v.size(); i++){
		if(current == NULL)
			cout<<"null pointer"<<endl;
		SCPI_CMD_NODE* temp = current->getNode(v.at(i));
		if(temp == NULL) {
			temp = new SCPI_CMD_NODE(v.at(i));
			current->addCmdSubNode(temp);
		}
		current = temp;

	}
	*/
	
	/*
	char* pattern[5][5]={{"RGS2000NG","RGS3000NG","RGS4000NG","RGS5000NG","RGS6000NG"},
					 {"SCE","XPDR","SETTING","M429","MEAsure"},
					 {"FREQ","Test","STA","PULSE","VDB"},
					 {"TYPE","MODE","POW","GEN","WQWE"},
					{"ANT","PRF","WIDTH","TIME","HEAD"}};
	
	char cmd[100];
	memset(cmd, '\0', 100);
	int n = 5;
	bool  b[5][5];
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			b[i][j] = false;

	dfs(pattern, 0, b, cmd, &root);
	*/
	SCPI_CMD_NODE root;
	root.addCmdPattern("*IDN?", SCPI_RGS_FUNC_TEST);
	root.addCmdPattern(":RGS2000NG:XPDR:[]:FREQuency:On", SCPI_RGS_FUNC_TEST);
	root.addCmdPattern(":RGS2000NG:XPDR:Test:OFf", SCPI_RGS_FUNC_TEST);
	root.addCmdPattern(":RGS2000NG:SCE:sta:VDB",SCPI_RGS_FUNC_TEST);
	root.addCmdPattern(":RGS2000NG:MEAsure:PULse:FREQuency",SCPI_RGS_FUNC_TEST);

	root.addCmdPattern(":RGS2000NG:TEST:PARAM:INT",SCPI_RGS_FUNC_TEST_INT);
	root.addCmdPattern(":RGS2000NG:TEST:PARAM:DOUBLE",SCPI_RGS_FUNC_TEST_DOUBLE);
	root.addCmdPattern(":RGS2000NG:TEST:PARAM:ARRAY:INT",SCPI_RGS_FUNC_TEST_ARRAY_INT);
	root.addCmdPattern(":RGS2000NG:TEST:PARAM:ARRAY:DOUBLE",SCPI_RGS_FUNC_TEST_ARRAY_DOUBLE);
	root.addCmdPattern(":RGS2000NG:TEST:PARAM:[P1|P2|P3|P4]:[A1|A2|A3|A4]:INT",SCPI_RGS_FUNC_TEST_SUB_CMD_CHOICE);
	
	root.levelOrder(&root);
	//testParamParser(&root);
	testParmSubCmdChoice(&root);
	//scpi_t context;
	//bool rst = parser.parser(&context, ":RGS2000NG:MEAsure:PULse:FREQuency OFF", &root);
	
	system("pause");
	return 0;
}

