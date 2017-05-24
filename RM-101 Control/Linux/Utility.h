#ifndef UTILITY_H
#define UTILITY_H

#include <sys/types.h>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

//Debug Ausgabe mit Parameter
#define DBP(fmt, ...)                                  \
   do {                                                 \
      if (DEBUG_TEST) fprintf(stderr, fmt, __VA_ARGS__); \
   } while (0)
//Debug Ausgabe ohne Parameter  
#define DBNP(fmt)                         \
   do {                                    \
      if (DEBUG_TEST) fprintf(stderr, fmt); \
   } while (0)

#define Bool u_int8_t
#define true 1
#define false 0

//Fehlercodes für Systemfehler siehe variable errno
#define ERROR_COULDNOTTOPENPORT 1 //Systemaufruf fehlgeschlagen
#define ERROR_INVALIDPORTHANDLE 2 //Ungültiger/nicht existierender Port
#define ERROR_SENDINGDATA 3 //Keine oder nicht alle Daten konnten gesendet werden

void setError(u_int8_t aError);
u_int8_t getError(void);

#endif /* UTILITY_H */