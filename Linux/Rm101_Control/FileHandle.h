#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include "Utility.h"

Bool OpenPort(char* aPort);
Bool ClosePort(void);
Bool SendCmd(char* aCmd);

#endif /* FILEHANDLE_H */