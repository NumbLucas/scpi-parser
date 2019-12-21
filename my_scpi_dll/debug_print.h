#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

#include <stdarg.h>

#define DEBUG_LEVEL 0
#define INFO_OUTPUT      3
#define WARNING_OUTPUT   2
#define DEBUG_OUTPUT     1
#define ERROR_OUTPUT     0

//#define DEBUG_LEVEL      DEBUG_OUTPUT
//需在使用以下打印宏的.c文件中定义“DEBUG_LEVEL”变量
#define INFO_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=INFO_OUTPUT){\
        printf("Info %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)

#define WARNING_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=WARNING_OUTPUT){\
        printf("Warning %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)

#define DEBUG_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=DEBUG_OUTPUT){\
        printf("Debug %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)

#define ERROR_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=ERROR_OUTPUT){\
        printf("Error %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)

#endif // !_DEBUG_PRINT_H_