#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "Rm101Control.h"

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

int16_t CalcWaistRotation(int16_t X, int16_t Y){
   double_t lcl_Rad;
   lcl_Rad = asin(Y/(sqrt(X*X+Y*Y)));
   double_t lcl_degree = X < 0 ? 180 - lcl_Rad * 180/M_PI : lcl_Rad * 180/M_PI;
   DBP("-----Berechne Drehung um Hüftgelenk: %f°\n", lcl_degree);
}


//X-Achse zeigt in Richtung Home position
//Z-Achse Richtung der Hüfte nach oben
//Y-achse bildet Vektorprodukt, so dass Rechtssystem entsteht
Bool MoveRoboterXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll) {
   RobotPosition lcl_RobotPosition = getPositions();
   //Hüfte
      //Berechne Hüftrotation
      CalcWaistRotation(X,Y);

      //Berechne Steps to do
      //Berechne Stepdifferenz neu - alt
}

void main (){
   MoveRoboterXYZ(1,2,3,0.0,0.0);

   MoveRoboterXYZ(-1,2,3,0.0,0.0);
};
