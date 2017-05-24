#ifndef UTILITY_H
#define UTILITY_H

#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

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

typedef enum {false = 0, true = 1} Bool;

//Fehlercodes für Systemfehler siehe SystemVariable errno(Linux) oder getlasterror(Windows)
#define ERROR_COULDNOTTOPENPORT 1 //Port konnte nicht geöffnet werden
#define ERROR_INVALIDPORTHANDLE 2 //Ungültiger/nicht existierender Port
#define ERROR_SENDINGDATA 3 //Keine oder nicht alle Daten konnten gesendet werden
#define ERROR_WAISTED 4 //Unmögliche Rotation um Hüft-Gelenk
#define ERROR_SHOULDERED 5 //Unmögliche Rotation um Schulter-Gelenk
#define ERROR_ELBOWED 6 // Unmögliche Rotation um Ellbogen-Gelenk
#define ERROR_WRISTED 7 //Unmögliche Rotation um Hand-Gelenk

Bool setError(u_int8_t aError);
u_int8_t getError(void);

#endif /* UTILITY_H */