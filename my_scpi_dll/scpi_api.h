#include "types.h"

#define SCPI_DLL_API _declspec(dllexport)
//#define SCPI_DLL_API 

#ifdef __cplusplus
extern "C" {
#endif

SCPI_DLL_API int SCPI_INIT(scpi_interface_t* interface);//接口初始化
SCPI_DLL_API int SCPI_REGISTER_PATTERN(char* pattern, scpi_command_callback_t callback);//注册SCPI命令
SCPI_DLL_API int SCPI_INPUT(char* cmd);//解析SCPI命令

SCPI_DLL_API scpi_bool_t SCPI_ParamIsNumber(const char* string);//判断参数是否为数字

SCPI_DLL_API scpi_bool_t SCPI_ParamInt(scpi_t * context, int * value);
SCPI_DLL_API scpi_bool_t SCPI_ParamFloat(scpi_t * context, float * value);
SCPI_DLL_API scpi_bool_t SCPI_ParamDouble(scpi_t * context, double * value);
SCPI_DLL_API scpi_bool_t SCPI_ParamCharacters(scpi_t * context, const char ** value, int * len);
SCPI_DLL_API scpi_bool_t SCPI_ParamSwitch(scpi_t * context, scpi_bool_t* value);
SCPI_DLL_API scpi_bool_t SCPI_ParamChoice(scpi_t * context, const scpi_choice_def_t * options, int* value);

SCPI_DLL_API scpi_bool_t SCPI_ParamArrayInt32(scpi_t * context, int *data, int i_count, int *o_count);
SCPI_DLL_API scpi_bool_t SCPI_ParamArrayFloat(scpi_t * context, float *data, int i_count, int *o_count);
SCPI_DLL_API scpi_bool_t SCPI_ParamArrayDouble(scpi_t * context, double *data, int i_count, int *o_count);
SCPI_DLL_API scpi_bool_t SCPI_ParamArraySwitch(scpi_t * context, scpi_bool_t* value, int i_count, int *o_count);

SCPI_DLL_API scpi_bool_t SCPI_ResultInt(scpi_t * context, int value);
SCPI_DLL_API scpi_bool_t SCPI_ResultFloat(scpi_t * context, float value);
SCPI_DLL_API scpi_bool_t SCPI_ResultDouble(scpi_t * context, double value);
SCPI_DLL_API scpi_bool_t SCPI_ResultCharacters(scpi_t * context, const char* value, int len);
SCPI_DLL_API scpi_bool_t SCPI_ResultSwitch(scpi_t * context, scpi_bool_t value);


#ifdef __cplusplus
}
#endif