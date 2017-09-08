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

#define WAIST_LENGTH 160 //in mm
#define SHOULDER_LENGTH 200 //in mm
#define ELLBOW_LENGTH 150 //in mm
#define WRIST_LENGTH 88 //in mm
#endif /* UTILITY_H */