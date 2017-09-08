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
      
   DBP("-----Berechne nötige Drehung um Hüftgelenk: %.2f°\n", lcl_degree);
   lcl_StepsToDo = Degree2Steps(lcl_degree, e_joint_Waist);
   DBP("-----Berechne neue Position von Hüftgelenk: %d\n", lcl_StepsToDo);
   return lcl_StepsToDo;
}

void calcWhoulderAndElbowRotation(int16_t X, int16_t Z, double_t* aShoulder1, double_t* aEllbow1, double_t* aShoulder2, double_t* aEllbow2){
   int16_t lcl_PositionVectorAbsolute = (int) sqrt(X * X + Z * Z);   
   DBNP("-----Berechne Winkeltupel für Schulter zum Ellbogen\n");
   DBNP("-----Schulterwinkel = 180° ± ζ   |   Ellbogenwinkel = α ± β\n");
   double_t lcl_Alpha = atan2((double)Z,(double)X) * 180/M_PI;
   double_t lcl_Beta = acos((SHOULDER_LENGTH2 + lcl_PositionVectorAbsolute*lcl_PositionVectorAbsolute - ELLBOW_LENGTH2) /
                           (double)(2 * SHOULDER_LENGTH * lcl_PositionVectorAbsolute)) * 180/M_PI;
   double_t lcl_Zeta = acos((SHOULDER_LENGTH2 + ELLBOW_LENGTH2 - lcl_PositionVectorAbsolute*lcl_PositionVectorAbsolute) /
                           (double)(2 * SHOULDER_LENGTH * ELLBOW_LENGTH)) * 180/M_PI;
   DBP("-----Berechne Hilfswinkel: α = %f, β = %f, ζ = %f\n", lcl_Alpha, lcl_Beta, lcl_Zeta);
   *aShoulder1 = lcl_Alpha - lcl_Beta;
   *aShoulder2 = lcl_Alpha + lcl_Beta;
   *aEllbow1 = 180 - lcl_Zeta;
   *aEllbow2 = - 180 + lcl_Zeta;
   DBP("-----Berechne Winkeltupel (Schulter, Ellbogen): [%.2f°,%.2f°] und [%.2f°,%.2f°]\n\n", *aShoulder1, *aEllbow1, *aShoulder2, *aEllbow2);

}

void calcPositionVector(int16_t* X, int16_t* Z, double_t aWristPitch){
   int16_t lcl_DeltaX = (int) (WRIST_LENGTH * cos(aWristPitch* M_PI / 180));
   int16_t lcl_DeltaZ = (int) (WRIST_LENGTH * sin(aWristPitch* M_PI / 180));
   *Z = *Z - WAIST_LENGTH - lcl_DeltaZ;
   *X = *X - lcl_DeltaX;
};


//X-Achse zeigt in Richtung Home position
//Z-Achse Richtung der Hüfte nach oben
//Y-achse bildet Vektorprodukt, so dass Rechtssystem entsteht
Bool MoveRoboterXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll, e_Grabber aGrabberStatus) {
   RobotPosition lcl_RobotPosition = getPositions();
   DBP("-----Zielkoordinaten: [%d, %d, %d], Greiferwinkel zur Horizotalen: %.2f° \n\n",X,Y,Z,aWristPitch); 
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
   DBP("-----Positionsvektor von Schulter zu Handgelenk in K': [%d;0;%d]\n\n",lcl_X, lcl_Z);

   double_t lcl_NewShoulderPositionAngle1;
   double_t lcl_NewShoulderPositionAngle2;
   double_t lcl_NewEllbowPositionAngle1;
   double_t lcl_NewEllbowPositionAngle2;
   calcWhoulderAndElbowRotation(lcl_X, lcl_Z, &lcl_NewShoulderPositionAngle1, &lcl_NewEllbowPositionAngle1, &lcl_NewShoulderPositionAngle2, &lcl_NewEllbowPositionAngle2);
   DBNP("-----Berechne Handgelenkwinkel um Hand korrekt an der Horizontalen auszurichten\n");
   double_t lcl_NewWristPositionAngle1 = aWristPitch - lcl_NewShoulderPositionAngle1 -lcl_NewEllbowPositionAngle1;
   double_t lcl_NewWristPositionAngle2 = aWristPitch - lcl_NewShoulderPositionAngle2 -lcl_NewEllbowPositionAngle2;
   DBP("-----Berechne Winkeltupel (Schulter, Ellbogen, Handgelenk): [%.2f°,%.2f°,%.2f°] und [%.2f°,%.2f°,%.2f°]\n\n",
         lcl_NewShoulderPositionAngle1, lcl_NewEllbowPositionAngle1, lcl_NewWristPositionAngle1,
         lcl_NewShoulderPositionAngle2, lcl_NewEllbowPositionAngle2, lcl_NewWristPositionAngle2);
}

void main (){
   MoveRoboterXYZ(200,200,150,-30.0,0.0,e_Grabber_close);
   printf("\n\n");
}
