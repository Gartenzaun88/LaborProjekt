#include "Utility.h"

static u_int8_t prv_LastError = 0;

void setError(u_int8_t aError){
prv_LastError = aError;   
}

u_int8_t getError(void){
return prv_LastError;   
}