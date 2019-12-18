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
typedef int(*scpi_write_t)(scpi_t * context, const char * data, int len);//��д��Ϣ�ӿ�

typedef struct scpi_interface_t {
        scpi_error_callback_t error;
        scpi_write_t write;
        //scpi_write_control_t control;
        scpi_command_callback_t flush; //reserved
        scpi_command_callback_t reset; //reserved
}scpi_interface_t;//����ӿڣ����ڴ�������Ϣ��д

typedef struct scpi_command_t {
        const char * pattern;
        scpi_command_callback_t callback;
}scpi_command_t;//��������ṹ��

struct _scpi_t{
	const char* header;
	const char* param;
	char extend_cmd_param[MAX_CMD_LEVEL][MAX_CMD_LENGTH];
	int  extend_cmd_size;
	int  subCmdParam;
	char cmds[MAX_CMD_LEVEL][MAX_CMD_LENGTH];
	int  cmds_num;
	scpi_interface_t* interface;
};	//�������������ģ����ڱ�����ص���Ϣ

typedef  struct scpi_choice_def_t {
	const char * name;
	int tag;
}scpi_choice_def_t; //�ض��Ĳ����ַ���ѡ�����ON/OFF,FORWARD/BACKWARD/LEFT/RIGHT��





//reserverd
typedef struct SCPI_CMD_INDEX {
	char longCmd[15];
	char shortCmd[5];
	//bool isEnd;//�Ƿ��Ѵ������β
	//bool hasParam;
	int loc;//�������е�λ��
	int nextStart;
	int nextEnd;
}SCPI_CMD_INDEX;

typedef struct SCPI_CMD_INDEX_LEVEL_INFO {
	SCPI_CMD_INDEX scpi_cmd_index[MAX_CMD_LEVEL];
	int size;//??2?����?����???��y
}SCPI_CMD_INDEX_LEVEL_INFO;


#endif