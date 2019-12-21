#include "types.h"

scpi_result_t SCPI_RGS_FUNC_TEST(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_TEST_CMD_PARAM(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_TEST_PARAM(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_TEST_CHOICE(scpi_t * context);

scpi_result_t SCPI_RGS_FUNC_TEST_INT(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_TEST_DOUBLE(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_TEST_CHARACTER(scpi_t * context);

scpi_result_t SCPI_RGS_FUNC_TEST_ARRAY_INT(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_TEST_ARRAY_DOUBLE(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_TEST_ARRAY_CHARACTER(scpi_t * context);

scpi_result_t SCPI_RGS_FUNC_TEST_SUB_CMD_CHOICE(scpi_t * context);





//先验证部分
//询问信号频率
//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:FREQ | :FREQUENCY}SP<numeric>CR
scpi_result_t SCPI_RGS_FUNC_QUERY_INTERROGATION_FREQ(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_SET_INTERROGATION_FREQ(scpi_t * context);
//打开或关闭询问
//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:RF}SP{ON | OFF}CR
scpi_result_t SCPI_RGS_FUNC_QUERY_INTERROGATION_SWITCH(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_SET_INTERROGATION_SWITCH(scpi_t * context);

//
//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:TYPE}SP<numeric>CR
scpi_result_t SCPI_RGS_FUNC_QUERY_INTERROGATION_TYPE(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_SET_INTERROGATION_TYPE(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:POW | :POWER}SP<numeric>CR
scpi_result_t SCPI_RGS_FUNC_QUERY_TOP_ANTENNA_POWER(scpi_t * context);
scpi_result_t SCPI_RGS_FUNC_SET_TOP_ANTENNA_POWER(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:LOAD}SP<filename>CR
scpi_result_t SCPI_RGS_FUNC_SET_LOAD_TEST_FILE(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:RES | :RESET}CR
scpi_result_t SCPI_RGS_FUNC_SET_RESET_TRANS_TEST(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:SAVE}SP<filename>CR
scpi_result_t SCPI_RGS_FUNC_SET_SAVE_TEST_FILE(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR:SCOPE}SP{INTERR | REPLY}CR
scpi_result_t SCPI_RGS_FUNC_SET_SCOPE_TRIGGER_MODE(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR:SCOPE:INTERR:OFFSET}SP<numeric>CR
scpi_result_t SCPI_RGS_FUNC_SET_SCOPE_TRIGGER_OFFSET(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:STA | :START} CR
scpi_result_t SCPI_RGS_FUNC_SET_START_TRANSMISSION(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:STO | :STOP}CR
scpi_result_t SCPI_RGS_FUNC_SET_STOP_TRANSMISSION(scpi_t * context);


//CMD GROUP
//:RGS2000NG}{:XPDR}{:ANT | :ANTENNA}{:POW | :POWER}SP<numeric>[,<numeric1>]CR
scpi_result_t SCPI_RGS_FUNC_SET_ANTENNA_POWER_DEVIATION(scpi_t * context);

//:RGS2000NG}{:XPDR}{:ANT | :ANTENNA}{:TIM | :TIME}SP<numeric>[,<numeric1>]CR
scpi_result_t SCPI_RGS_FUNC_SET_ANTENNA_TIME_DEVIATION(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:MOD | :MODE}SP<numeric>CR
scpi_result_t SCPI_RGS_FUNC_SET_INTERROGAATION_MODE(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:UF}SP<numeric>CR
scpi_result_t SCPI_RGS_FUNC_SET_INTERROGAATION_MODE_S_MSG_DATA(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:PUL: | :PULSE:}<pulse>SP<value>[,<value1>]CR
scpi_result_t SCPI_RGS_FUNC_SET_PULSE_PARAM(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:PRF }SP<numeric>CR
//Pulse Repetition Frequency (PRF)
scpi_result_t SCPI_RGS_FUNC_SET_PRF(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:PWIDTH | :PW }SP<numeric>CR
scpi_result_t SCPI_RGS_FUNC_SET_PULSE_WIDTH(scpi_t * context);

//Command Syntax: {:RGS | :RGS2000NG}{:XPDR}{:PRFSYNC}SP{ON | OFF}CR
scpi_result_t SCPI_RGS_FUNC_SET_PRF_SYNCHRONIZATION(scpi_t * context);

//{:RGS | :RGS2000NG}{:XPDR}{:SLS}SP{ON | OFF}[,{ON | OFF}]CR
scpi_result_t SCPI_RGS_FUNC_SET_SLS(scpi_t * context);