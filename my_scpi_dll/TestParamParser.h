#pragma once
#include "types.h"
#include "SCPI_CMD_NODE.h"

void testParamParser(SCPI_CMD_NODE* root);
void testParamNumberInt(SCPI_CMD_NODE* root);
void testParamNumberDouble(SCPI_CMD_NODE* root);
void testParamCharacter(SCPI_CMD_NODE* root);

void testParamArrayNumberInt(SCPI_CMD_NODE* root);
void testParamArrayNumberDouble(SCPI_CMD_NODE* root);
void testParamArrayCharacter(SCPI_CMD_NODE* root);

void testParmSubCmdChoice(SCPI_CMD_NODE* root);

