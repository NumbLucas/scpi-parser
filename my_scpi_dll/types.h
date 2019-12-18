#ifndef __TYPES_H__
#define __TYPES_H__

#define MAX_CMD_LEVEL 10
#define MAX_CMD_LENGTH 15

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#define SCPI_CHOICE_LIST_END   {NULL, -1}
#define SCPI_CMD_LIST_END       {NULL, NULL} 

typedef unsigned char scpi_bool_t;

typedef enum scpi_result_t {
        SCPI_RES_OK = 1,
        SCPI_RES_ERR = -1
}scpi_result_t;


typedef struct _scpi_t scpi_t;
typedef scpi_result_t(*scpi_command_callback_t)(scpi_t *);
typedef int (*scpi_error_callback_t)(scpi_t * context, int error);			
typedef int(*scpi_write_t)(scpi_t * context, const char * data, int len);//回写消息接口

typedef struct scpi_interface_t {
        scpi_error_callback_t error;
        scpi_write_t write;
        //scpi_write_control_t control;
        scpi_command_callback_t flush; //reserved
        scpi_command_callback_t reset; //reserved
}scpi_interface_t;//对外接口，用于错误处理，消息回写

typedef struct scpi_command_t {
        const char * pattern;
        scpi_command_callback_t callback;
}scpi_command_t;//单条命令结构体

struct _scpi_t{
	const char* header;
	const char* param;
	char extend_cmd_param[MAX_CMD_LEVEL][MAX_CMD_LENGTH];
	int  extend_cmd_size;
	int  subCmdParam;
	char cmds[MAX_CMD_LEVEL][MAX_CMD_LENGTH];
	int  cmds_num;
	scpi_interface_t* interface;
};	//单条命令上下文，用于保存相关的信息

typedef  struct scpi_choice_def_t {
	const char * name;
	int tag;
}scpi_choice_def_t; //特定的参数字符串选项，例如ON/OFF,FORWARD/BACKWARD/LEFT/RIGHT等





//reserverd
typedef struct SCPI_CMD_INDEX {
	char longCmd[15];
	char shortCmd[5];
	//bool isEnd;//是否已达命令结尾
	//bool hasParam;
	int loc;//索引表中的位置
	int nextStart;
	int nextEnd;
}SCPI_CMD_INDEX;

typedef struct SCPI_CMD_INDEX_LEVEL_INFO {
	SCPI_CMD_INDEX scpi_cmd_index[MAX_CMD_LEVEL];
	int size;//??2?×ó?üá???êy
}SCPI_CMD_INDEX_LEVEL_INFO;


#endif