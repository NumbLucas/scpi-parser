#pragma once
#include <string>
#include "SCPI_CMD_NODE.h"
#include "scpi_def.h"
using namespace std;

class SCPI_PARSER
{
public:
	SCPI_PARSER(void);
	SCPI_PARSER(scpi_interface_t*);
	~SCPI_PARSER(void);
	bool parserHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param="");
	bool parserCommonCmdHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param="");
	bool parserRgsCmdHeader(scpi_t* context, string header, SCPI_CMD_NODE* root, string param="");
	bool parser(scpi_t* context, string cmd, SCPI_CMD_NODE* root);
	void processContext(scpi_t* context, string header, string param);
public:
	scpi_interface_t* interface;
};

