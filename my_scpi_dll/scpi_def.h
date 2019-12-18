#include "types.h"

extern scpi_t scpi_context;
extern scpi_interface_t my_scpi_interface;
int SCPI_Write(scpi_t * context, const char * data, int len);
int SCPI_Error(scpi_t * context, int err);
scpi_result_t SCPI_Reset(scpi_t * context);
scpi_result_t SCPI_Flush(scpi_t * context);