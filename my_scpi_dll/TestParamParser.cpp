#include "TestParamParser.h"
#include "SCPI_PARSER.h"

void testParamParser(SCPI_CMD_NODE* root) {
	testParamNumberInt(root);
	testParamNumberDouble(root);
	testParamArrayNumberInt(root);
	testParamArrayNumberDouble(root);
}

void testParamNumberInt(SCPI_CMD_NODE* root) {
	SCPI_PARSER parser;
	scpi_t context;
	bool rst = parser.parser(&context, ":RGS2000NG:TEST:PARAM:INT 1231", root);	
}

void testParamNumberDouble(SCPI_CMD_NODE* root) {
	SCPI_PARSER parser;
	scpi_t context;
	bool rst = parser.parser(&context, ":RGS2000NG:TEST:PARAM:DOUBLE 2.432", root);
}

void testParamCharacter(SCPI_CMD_NODE* root);

void testParamArrayNumberInt(SCPI_CMD_NODE* root) {
	SCPI_PARSER parser;
	scpi_t context;
	bool rst = parser.parser(&context, ":RGS2000NG:TEST:PARAM:ARRAY:INT 1,3,4,5", root);	
}
void testParamArrayNumberDouble(SCPI_CMD_NODE* root) {
	SCPI_PARSER parser;
	scpi_t context;
	bool rst = parser.parser(&context, ":RGS2000NG:TEST:PARAM:ARRAY:DOUBLE 2.4,4.87,32.543", root);	
}
void testParamArrayCharacter(SCPI_CMD_NODE* root);

void testParmSubCmdChoice(SCPI_CMD_NODE* root){
	SCPI_PARSER parser;
	scpi_t context;
	for(int i = 0;i< MAX_SUB_CMD_CHOICE_LEVEL;i++) {
		context.cmd_chioce_index[i] = -1;
	}
	context.cmds_num = 0;
	bool rst = parser.parser(&context, ":RGS2000NG:TEST:PARAM:P2:A3:INT 32", root);
}