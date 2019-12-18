#include <regex>
#include <iostream>
#include "scpi_api.h"
#include "SCPI_CMD_NODE.h"
#include "SCPI_PARSER.h"
#include "handle_multi_cmd.h"

using namespace std;

SCPI_CMD_NODE* root = new SCPI_CMD_NODE();
SCPI_PARSER* parser = new SCPI_PARSER(&my_scpi_interface);

/*
��������writeData
�� �ܣ�����������Ϣ�ӿ�
�� ��1��context		����������������Ϣ	
�� ��2��data		    �����ַ���
�� ��3��len			�����ַ�������
����ֵ��д����Ϣ����
**/
static size_t writeData(scpi_t * context, const char * data, int len) {
    if (len > 0) {
        return context->interface->write(context, data, len);
    } else {
        return 0;
    }
}

/*
��������SCPI_INIT
�� �ܣ�SCPI��Ϣ���صȽӿڵĳ�ʼ��
�� ��1��interface		��Ϣ���ؽӿڽṹ��	
����ֵ��1
**/
SCPI_DLL_API int SCPI_INIT(scpi_interface_t* interface) {
	parser->interface = interface;	
	return 1;
}

/*
��������SCPI_REGISTER_PATTERN
�� �ܣ�ע������ԭ��
�� ��1��pattern		����ԭ��
�� ��2 ����Ӧ�Ļص�����
����ֵ��1
**/
int SCPI_REGISTER_PATTERN(char* pattern, scpi_command_callback_t callback) {
	root->addCmdPattern(pattern, callback);
	return 1;
}

/*
��������SCPI_INPUT
�� �ܣ��������
�� ��1��cmd		��Ҫ����������
����ֵ�������Ƿ�ɹ���0ʧ�ܣ�1�ɹ�
**/
int SCPI_INPUT(char* cmd) {
	scpi_t context;

	return parser->parser(&context, cmd, root);
}

/*
��������SCPI_ParamIsNumber
�� �ܣ��ж��ַ����Ƿ�Ϊ����
�� ��1��string		�����ַ���
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamIsNumber(const char* string) {
	scpi_bool_t rst = FALSE;
	regex science("^((\\d+.?\\d+)[Ee]{1}(-?\\d+))$");//��ѧ������
	regex number("^-?\\d+(.\\d+)?$");//���֣�����С����

	if(regex_match(string, number)||
		regex_match(string, science))
		rst = TRUE;
	return rst;
}

/*
��������SCPI_ParamInt
�� �ܣ����ַ�������ΪInt������
�� ��1��context		����������
�� ��2��value       ��Ӧ�����ֽ��
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamInt(scpi_t * context, int * value) {
	if(!SCPI_ParamIsNumber(context->param))
		return FALSE;
	*value = atoi(context->param);
	return TRUE;			
}

/*
��������SCPI_ParamFloat
�� �ܣ����ַ�������ΪFloat������
�� ��1��context		����������
�� ��2��value       ��Ӧ�����ֽ��
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamFloat(scpi_t * context, float * value) {
		if(!SCPI_ParamIsNumber(context->param))
		return FALSE;
	*value = atof(context->param);

	return TRUE;	
}
/*
��������SCPI_ParamDouble
�� �ܣ����ַ�������ΪDouble������
�� ��1��context		����������
�� ��2��value       ��Ӧ�����ֽ��
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamDouble(scpi_t * context, double * value) {
	if(!SCPI_ParamIsNumber(context->param))
		return FALSE;
	*value = atof(context->param);
	return TRUE;	
}

/*
��������SCPI_ParamCharacters
�� �ܣ������ַ���
�� ��1��context		����������
�� ��2��value       ��Ӧ�����ֽ��
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamCharacters(scpi_t * context, const char ** value, int * len) {
	*value = context->param;
	return TRUE;
}

/*
��������SCPI_ParamSwitch
�� �ܣ���ON/OFF����Ϊ1/0
�� ��1��context		����������
�� ��2��value       ��Ӧ�����ֽ��
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ParamSwitch(scpi_t * context, scpi_bool_t* value) {
	if(0 == strcmp(context->param, "ON")) {
		*value = TRUE;
		return TRUE;
	}else if(0 == strcmp(context->param, "OFF")) {
		*value = FALSE;
		return TRUE;
	}
	return FALSE;
}

/*
��������SCPI_ParamChoice
�� �ܣ����ַ�������Ϊѡ������
�� ��1��context		����������
�� ��2��value       ��Ӧ�����ֽ��
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ParamChoice(scpi_t * context, const scpi_choice_def_t * options, int* value) {
	int index = 0;
	if(!value||!options)
		return FALSE;
	for(;options[index].name;index++) {
		if(0 == strcmp(options[index].name, context->param)) {
			*value = options[index].tag;
			return TRUE;
		}
	}
	return FALSE;
}
/*
��������
�� �ܣ������������ģ��
˵��������
����ֵ��
**/
#define PARAM_ARRAY_TEMPLATE(func) do{\
    for (*o_count = 0; *o_count < i_count; (*o_count)++) {\
        if (!func(context, &data[*o_count])) {\
			return FALSE;\
        }\
    }\
    return TRUE;\
}while(0)

/*
��������SCPI_ParamInt32
�� �ܣ����ַ�������ΪInt����������
�� ��1��context		����������
�� ��2��data       ��Ӧ�Ĵ���������ָ��
�� ��3��i_count	   data�����С
�� ��4��*o_count    ��������ʵ�ʴ�С
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamArrayInt32(scpi_t * context, int *data, int i_count, int *o_count) {
	vector<string> v;
	splitStr(context->param, ',', v); 
	for (int i = 0; i < i_count&&i<v.size(); i++)
	{
		if(!SCPI_ParamIsNumber(v.at(i).c_str()))
			return FALSE;
		data[i] = atoi(v.at(i).c_str());
		(*o_count)++;
	}
	return TRUE;
}

/*
��������SCPI_ParamFloat
�� �ܣ����ַ�������ΪFloat����������
�� ��1��context		����������
�� ��2��data       ��Ӧ�Ĵ���������ָ��
�� ��3��i_count	   data�����С
�� ��4��*o_count    ��������ʵ�ʴ�С
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamArrayFloat(scpi_t * context, float *data, int i_count, int *o_count) {
	vector<string> v;
	splitStr(context->param, ',', v); 
	for (unsigned int i = 0; i < i_count&&i<v.size(); i++)
	{
		if(!SCPI_ParamIsNumber(v.at(i).c_str()))
			return FALSE;
		data[i] = atof(v.at(i).c_str());
		(*o_count)++;
	}
	return TRUE;
}

/*
��������SCPI_ParamDouble
�� �ܣ����ַ�������ΪDouble����������
�� ��1��context		����������
�� ��2��data       ��Ӧ�Ĵ���������ָ��
�� ��3��i_count	   data�����С
�� ��4��*o_count    ��������ʵ�ʴ�С
����ֵ��0ʧ�ܣ�1�ɹ�
**/
scpi_bool_t SCPI_ParamArrayDouble(scpi_t * context, double *data, int i_count, int *o_count) {
	vector<string> v;
	splitStr(context->param, ',', v); 
	for (unsigned int i = 0; i < i_count&&i<v.size(); i++)
	{
		if(!SCPI_ParamIsNumber(v.at(i).c_str()))
			return FALSE;
		data[i] = atof(v.at(i).c_str());
		(*o_count)++;
	}
	return TRUE;
}

/*
��������SCPI_ParamArraySwitch
�� �ܣ����ַ�������Ϊbool����������
�� ��1��context		����������
�� ��2��data       ��Ӧ�Ĵ���������ָ��
�� ��3��i_count	   data�����С
�� ��4��*o_count    ��������ʵ�ʴ�С
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ParamArraySwitch(scpi_t * context, scpi_bool_t* value, int i_count, int *o_count){
	vector<string> v;
	splitStr(context->param, ',', v); 
	for (unsigned int i = 0; i < i_count&&i<v.size(); i++)
	{
		if(!SCPI_ParamSwitch(context, &(value[i])))
			return FALSE;
		//value[i] = atof(v.at(i).c_str());
		(*o_count)++;
	}
	return TRUE;
}

/*
��������SCPI_ResultInt
�� �ܣ���int����ֵתΪ�ַ�����ʽд�뷵�ؽ��
�� ��1��context		����������
�� ��2��value       ֵ
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ResultInt(scpi_t * context, int value) {
	if(context->interface) {
		char data[64];
		sprintf_s(data, sizeof(data), "%d", value);
		//sprintf(data, "%d", value);
		return writeData(context,data, 64);
	}
	return 0;
}
/*
��������SCPI_ResultFloat
�� �ܣ���Float����ֵתΪ�ַ�����ʽд�뷵�ؽ��
�� ��1��context		����������
�� ��2��value       ֵ
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ResultFloat(scpi_t * context, float value) {
	if(context->interface) {
		char data[64];
		sprintf_s(data, sizeof(data), "%f", value);
		//sprintf(data, "%f", value);
		return writeData(context,data, 64);
	}
	return 0;
}
/*
��������SCPI_ResultDouble
�� �ܣ���Double����ֵתΪ�ַ�����ʽд�뷵�ؽ��
�� ��1��context		����������
�� ��2��value       ֵ
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ResultDouble(scpi_t * context, double value) {
	if(context->interface) {
		char data[64];
		sprintf_s(data, sizeof(data), "%f", value);
		//sprintf(data, "%f", value);
		return writeData(context,data, 64);
	}
	return 0;
}
/*
��������SCPI_ResultCharacters
�� �ܣ����ַ���д�뷵�ؽ��
�� ��1��context		����������
�� ��2��value       ֵ
�� ��3���ַ�������
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ResultCharacters(scpi_t * context, const char* value, int len) {
	if(context->interface) {
		return writeData(context, value, len);
	}
	return 0;
}
/*
��������SCPI_ResultSwitch
�� �ܣ���bool����ֵתΪ�ַ�����ʽд�뷵�ؽ��
�� ��1��context		����������
�� ��2��value       ֵ
����ֵ��0ʧ�ܣ�1�ɹ�
**/
SCPI_DLL_API scpi_bool_t SCPI_ResultSwitch(scpi_t * context, scpi_bool_t value) {
	if(context->interface) {
		if(value)
			return writeData(context, "ON", 2);
		return writeData(context, "OFF", 3);
	}
	return 0;
}