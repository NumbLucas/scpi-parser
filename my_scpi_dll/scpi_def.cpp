#include "scpi_def.h"

scpi_t scpi_context;
scpi_interface_t my_scpi_interface = {    
	SCPI_Error,
    SCPI_Write,
	nullptr,
	nullptr
    //SCPI_Control,
    //SCPI_Flush,
    //SCPI_Reset,
};
int SCPI_Write(scpi_t * context, const char * data, int len) {
    return len;
}

scpi_result_t SCPI_Flush(scpi_t * context) {
    return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, int err) {
    return 0;
}

scpi_result_t SCPI_Reset(scpi_t * context) {
    return SCPI_RES_OK;
}