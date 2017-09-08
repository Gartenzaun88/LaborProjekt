#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "Utility.h"
#include "Rm101Control.h"

int16_t CalcWaistRotation(int16_t X, int16_t Y){
   double_t lcl_Rad;
   double_t lcl_degree;
   int16_t lcl_StepsToDo;

   lcl_Rad = asin(Y/(sqrt(X*X+Y*Y)));
   if (X < 0) 
      lcl_degree = Y < 0 ? -180 - lcl_Rad * 180/M_PI : 180 - lcl_Rad * 180/M_PI;
   else
      lcl_degree = lcl_Rad * 180/M_PI;
      
   DBP("-----Berechne nötige Drehung um Hüftgelenk: %f°\n", lcl_degree);
   lcl_StepsToDo = Degree2Steps(lcl_degree, e_joint_Waist);
   DBP("-----Berechne neue Position von Hüftgelenk: %d\n", lcl_StepsToDo);
   return lcl_StepsToDo;
}

void calcWhoulderAndElbowRotation(int16_t X, int16_t Z, double_t* aShoulder1, double_t* aEllbow1, double_t* aShoulder2, double_t* aEllbow2){
   int16_t lcl_PositionVector = (int) sqrt(X * X + Z * Z);   
   DBNP("-----Berechne Winkeltupel für Schulter zum Ellbogen\n");
   DBNP("-----Ellbogenwinkel = α ± β   |   Schulterwinkel = 180° ± ζ\n");
   double_t lcl_Beta = acos((X*X + lcl_PositionVector*lcl_PositionVector - Y*Y) / (2 * X * lcl_PositionVector));
   double_t lcl_Alpha = atan2(Z,X);
   double_t lcl_Zeta = acos((X*X + Y*Y - lcl_PositionVector*lcl_PositionVector) / (2 * X * Y));

}

void calcPositionVector(int16_t* X, int16_t* Z, double_t aWristPitch){
   int16_t lcl_DeltaX = (int) (WRIST_LENGTH * cos(aWristPitch * M_PI / 180));
   int16_t lcl_DeltaZ = (int) (WRIST_LENGTH * sin(aWristPitch * M_PI / 180));
   *Z = *Z - WAIST_LENGTH + lcl_DeltaZ;
   *X = *X - lcl_DeltaX;
};


//X-Achse zeigt in Richtung Home position
//Z-Achse Richtung der Hüfte nach oben
//Y-achse bildet Vektorprodukt, so dass Rechtssystem entsteht
Bool MoveRoboterXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll, e_Grabber aGrabberStatus) {
   RobotPosition lcl_RobotPosition = getPositions();
    
   //Hüfte
   int16_t lcl_WaistStepsToDo;
   int16_t lcl_NewWaistPosition;
   lcl_NewWaistPosition = CalcWaistRotation(X,Y);
   lcl_WaistStepsToDo = lcl_NewWaistPosition - lcl_RobotPosition.Waist;
   DBP("-----Aktuelle Hüftposition: %d, Benötigte Bewegung: %d\n\n",lcl_RobotPosition.Waist,lcl_WaistStepsToDo);

   //Berechne Roboter Koordinaten X und Z
   int16_t lcl_X = (int) sqrt(X*X+Y*Y);
   int16_t lcl_Z = Z;
   DBP("-----X im Robotereigenen Koordinatensystem K': X' = %d\n\n", lcl_X);

   //Berechne Positionsvektor von Schulter zum Handgelenk
   DBNP("-----Berechne Positionsvektor von Schulter zum Handgelenk in K'\n");
   DBP("-----Positionsvektor von Ursprung zu TCP in K': [%d;0;%d]\n",lcl_X, lcl_Z);
   calcPositionVector(&lcl_X, &lcl_Z, aWristPitch);
   DBP("-----Positionsvektor von Schulter zu Handgelenk in K': [%d;0;%d]\n",lcl_X, lcl_Z);

   calcWhoulderAndElbowRotation(lcl_X, lcl_Z,NULL,NULL,NULL,NULL);
}

void main (){
   MoveRoboterXYZ(300,200,300,30.0,0.0,e_Grabber_close);
   printf("\n\n");
}
