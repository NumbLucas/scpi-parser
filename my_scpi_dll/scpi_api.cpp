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
函数名：writeData
功 能：仪器返回信息接口
参 数1：context		该条命令上下文信息	
参 数2：data		    返回字符串
参 数3：len			返回字符串长度
返回值：写入消息长度
**/
static size_t writeData(scpi_t * context, const char * data, int len) {
    if (len > 0) {
        return context->interface->write(context, data, len);
    } else {
        return 0;
    }
}

/*
函数名：SCPI_INIT
功 能：SCPI消息返回等接口的初始化
参 数1：interface		消息返回接口结构体	
返回值：1
**/
SCPI_DLL_API int SCPI_INIT(scpi_interface_t* interface) {
	parser->interface = interface;	
	return 1;
}

/*
函数名：SCPI_REGISTER_PATTERN
功 能：注册命令原型
参 数1：pattern		命令原型
参 数2 ：对应的回调函数
返回值：1
**/
int SCPI_REGISTER_PATTERN(char* pattern, scpi_command_callback_t callback) {
	root->addCmdPattern(pattern, callback);
	return 1;
}

/*
函数名：SCPI_INPUT
功 能：命令解析
参 数1：cmd		需要解析的命令
返回值：解析是否成功，0失败，1成功
**/
int SCPI_INPUT(char* cmd) {
	scpi_t context;

	return parser->parser(&context, cmd, root);
}

/*
函数名：SCPI_ParamIsNumber
功 能：判断字符串是否为数字
参 数1：string		输入字符串
返回值：0失败，1成功
**/
scpi_bool_t SCPI_ParamIsNumber(const char* string) {
	scpi_bool_t rst = FALSE;
	regex science("^((\\d+.?\\d+)[Ee]{1}(-?\\d+))$");//科学计数法
	regex number("^-?\\d+(.\\d+)?$");//数字（包含小数）

	if(regex_match(string, number)||
		regex_match(string, science))
		rst = TRUE;
	return rst;
}

/*
函数名：SCPI_ParamInt
功 能：将字符串解析为Int型数字
参 数1：context		命令上下文
参 数2：value       对应的数字结果
返回值：0失败，1成功
**/
scpi_bool_t SCPI_ParamInt(scpi_t * context, int * value) {
	if(!SCPI_ParamIsNumber(context->param))
		return FALSE;
	*value = atoi(context->param);
	return TRUE;			
}

/*
函数名：SCPI_ParamFloat
功 能：将字符串解析为Float型数字
参 数1：context		命令上下文
参 数2：value       对应的数字结果
返回值：0失败，1成功
**/
scpi_bool_t SCPI_ParamFloat(scpi_t * context, float * value) {
		if(!SCPI_ParamIsNumber(context->param))
		return FALSE;
	*value = atof(context->param);

	return TRUE;	
}
/*
函数名：SCPI_ParamDouble
功 能：将字符串解析为Double型数字
参 数1：context		命令上下文
参 数2：value       对应的数字结果
返回值：0失败，1成功
**/
scpi_bool_t SCPI_ParamDouble(scpi_t * context, double * value) {
	if(!SCPI_ParamIsNumber(context->param))
		return FALSE;
	*value = atof(context->param);
	return TRUE;	
}

/*
函数名：SCPI_ParamCharacters
功 能：解析字符串
参 数1：context		命令上下文
参 数2：value       对应的数字结果
返回值：0失败，1成功
**/
scpi_bool_t SCPI_ParamCharacters(scpi_t * context, const char ** value, int * len) {
	*value = context->param;
	return TRUE;
}

/*
函数名：SCPI_ParamSwitch
功 能：将ON/OFF解析为1/0
参 数1：context		命令上下文
参 数2：value       对应的数字结果
返回值：0失败，1成功
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
函数名：SCPI_ParamChoice
功 能：将字符串解析为选项类型
参 数1：context		命令上下文
参 数2：value       对应的数字结果
返回值：0失败，1成功
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
函数名：
功 能：数组参数解析模板
说明：保留
返回值：
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
函数名：SCPI_ParamInt32
功 能：将字符串解析为Int型数字数组
参 数1：context		命令上下文
参 数2：data       对应的传入数组结果指针
参 数3：i_count	   data数组大小
参 数4：*o_count    结果数组的实际大小
返回值：0失败，1成功
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
函数名：SCPI_ParamFloat
功 能：将字符串解析为Float型数字数组
参 数1：context		命令上下文
参 数2：data       对应的传入数组结果指针
参 数3：i_count	   data数组大小
参 数4：*o_count    结果数组的实际大小
返回值：0失败，1成功
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
函数名：SCPI_ParamDouble
功 能：将字符串解析为Double型数字数组
参 数1：context		命令上下文
参 数2：data       对应的传入数组结果指针
参 数3：i_count	   data数组大小
参 数4：*o_count    结果数组的实际大小
返回值：0失败，1成功
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
函数名：SCPI_ParamArraySwitch
功 能：将字符串解析为bool型数字数组
参 数1：context		命令上下文
参 数2：data       对应的传入数组结果指针
参 数3：i_count	   data数组大小
参 数4：*o_count    结果数组的实际大小
返回值：0失败，1成功
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
函数名：SCPI_ResultInt
功 能：将int型数值转为字符串形式写入返回结果
参 数1：context		命令上下文
参 数2：value       值
返回值：0失败，1成功
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
函数名：SCPI_ResultFloat
功 能：将Float型数值转为字符串形式写入返回结果
参 数1：context		命令上下文
参 数2：value       值
返回值：0失败，1成功
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
函数名：SCPI_ResultDouble
功 能：将Double型数值转为字符串形式写入返回结果
参 数1：context		命令上下文
参 数2：value       值
返回值：0失败，1成功
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
函数名：SCPI_ResultCharacters
功 能：将字符串写入返回结果
参 数1：context		命令上下文
参 数2：value       值
参 数3：字符串长度
返回值：0失败，1成功
**/
SCPI_DLL_API scpi_bool_t SCPI_ResultCharacters(scpi_t * context, const char* value, int len) {
	if(context->interface) {
		return writeData(context, value, len);
	}
	return 0;
}
/*
函数名：SCPI_ResultSwitch
功 能：将bool型数值转为字符串形式写入返回结果
参 数1：context		命令上下文
参 数2：value       值
返回值：0失败，1成功
**/
SCPI_DLL_API scpi_bool_t SCPI_ResultSwitch(scpi_t * context, scpi_bool_t value) {
	if(context->interface) {
		if(value)
			return writeData(context, "ON", 2);
		return writeData(context, "OFF", 3);
	}
	return 0;
}