#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include "Utility.h"

Bool OpenPort(char* aPort);
Bool SendCmd(char* aCmd);
Bool ClosePort(void);

#endif /* FILEHANDLE_H */