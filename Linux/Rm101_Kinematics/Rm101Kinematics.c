#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "Utility.h"
#include "Rm101Control.h"

static RobotPosition prv_RobotPosition;

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

Bool calcShoulderAndElbowRotation(int16_t X, int16_t Z, double_t* aShoulder1, double_t* aElbow1, double_t* aShoulder2, double_t* aElbow2){
   int16_t lcl_PositionVectorAbsolute = (int) sqrt(X * X + Z * Z);
   if (lcl_PositionVectorAbsolute > Elbow_LENGTH + SHOULDER_LENGTH){
      DBP("-----Positionsvektorlänge %d ist größer als Roboterreichweite %d\n", lcl_PositionVectorAbsolute, Elbow_LENGTH + SHOULDER_LENGTH);
      return false;
   }
   DBNP("-----Berechne Winkeltupel für Schulter zum Ellbogen\n");
   DBNP("-----Schulterwinkel = 180° ± ζ   |   Ellbogenwinkel = α ± β\n");
   double_t lcl_Alpha = atan2((double)Z,(double)X) * 180/M_PI;
   double_t lcl_Beta = acos((SHOULDER_LENGTH2 + lcl_PositionVectorAbsolute*lcl_PositionVectorAbsolute - Elbow_LENGTH2) /
                           (double)(2 * SHOULDER_LENGTH * lcl_PositionVectorAbsolute)) * 180/M_PI;
   double_t lcl_Zeta = acos((SHOULDER_LENGTH2 + Elbow_LENGTH2 - lcl_PositionVectorAbsolute*lcl_PositionVectorAbsolute) /
                           (double)(2 * SHOULDER_LENGTH * Elbow_LENGTH)) * 180/M_PI;
   DBP("-----Berechne Hilfswinkel: α = %f, β = %f, ζ = %f\n", lcl_Alpha, lcl_Beta, lcl_Zeta);
   *aShoulder1 = lcl_Alpha - lcl_Beta;
   *aShoulder2 = lcl_Alpha + lcl_Beta;
   *aElbow1 = 180 - lcl_Zeta;
   *aElbow2 = - 180 + lcl_Zeta;
   DBP("-----Berechne Winkeltupel (Schulter, Ellbogen): [%.2f°,%.2f°] und [%.2f°,%.2f°]\n\n", *aShoulder1, *aElbow1, *aShoulder2, *aElbow2);
   return true;
}

Bool ParseError(int16_t aSendError){   
   switch (aSendError) {
      case 1: DBNP("-----Port nicht offen\n");return false;
      case 2: DBNP("-----Ungültiger Port\n");return false;
      case 3: DBNP("-----Fehler bei Datenübertragung\n");return false;
      case 4: DBNP("-----Unmögliche Rotation um Hüftgelenk\n");return false;
      case 5: DBNP("-----Unmögliche Rotation um Schultergelenk\n");return true;
      case 6: DBNP("-----Unmögliche Rotation um Ellbogengelenk\n");return true;
      case 7: DBNP("-----Unmögliche Rotation um Handgelenk\n");return true;
      default: DBNP("-----Unbekannter Fehler\n");return false;
   }
}

void calcShoulderElbowWristSteps(double_t aShoulderAngle, double_t aElbowAngle, double_t aWristAngle,
                                 int16_t* aShoulderStepsToDo,int16_t* aElbowStepsToDo,int16_t* aWristStepsToDo){
   DBNP("-----Berechne nötige Steps von Homeposition (Ellbogen, Schulter,Handgelenk)\n");
   int16_t lcl_ShoulderStepsFromHome = Degree2Steps(aShoulderAngle, e_joint_Shoulder) - SHOULDER_HOMEPOS;
   int16_t lcl_ElbowStepsFromHome = Degree2Steps(aElbowAngle, e_joint_Elbow) - ELBOW_HOMEPOS;
   DBP("-----Neues Positionstupel (Relativ zu Homeposition): %d(%d);%d(%d);%d\n",
      Degree2Steps(aShoulderAngle, e_joint_Shoulder),lcl_ShoulderStepsFromHome,
      Degree2Steps(aElbowAngle, e_joint_Elbow),lcl_ElbowStepsFromHome, Degree2Steps(aWristAngle, e_joint_WristPitch));
   *aShoulderStepsToDo = lcl_ShoulderStepsFromHome - prv_RobotPosition.Shoulder;
   *aElbowStepsToDo = lcl_ElbowStepsFromHome - prv_RobotPosition.Elbow;
   *aWristStepsToDo = Degree2Steps(aWristAngle, e_joint_WristPitch) - prv_RobotPosition.WristPitch;
   DBP("-----Stepdifferenz für Positionstupel zur aktuellen Position: (%d,%d,%d)\n\n",
      *aShoulderStepsToDo, *aElbowStepsToDo, *aWristStepsToDo);
}

void calcPositionVector(int16_t* X, int16_t* Z, double_t aWristPitch){
   int16_t lcl_DeltaX = (int) (WRIST_LENGTH * cos(aWristPitch* M_PI / 180));
   int16_t lcl_DeltaZ = (int) (WRIST_LENGTH * sin(aWristPitch* M_PI / 180));
   *Z = *Z - WAIST_LENGTH - lcl_DeltaZ;
   *X = *X - lcl_DeltaX;
}

Bool CalcStepsforNewPosition(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll,
   RobotPositionPtr aStepTuple1, RobotPositionPtr aStepTuple2){
   prv_RobotPosition = getPositions();
   DBP("-----Zielkoordinaten: [%d, %d, %d], Greiferwinkel zur Horizotalen: %.2f° \n\n",X,Y,Z,aWristPitch); 
   //Hüfte
   int16_t lcl_NewWaistPosition;
   lcl_NewWaistPosition = CalcWaistRotation(X,Y);
   aStepTuple1->Waist = lcl_NewWaistPosition - prv_RobotPosition.Waist;
   aStepTuple2->Waist = lcl_NewWaistPosition - prv_RobotPosition.Waist;
   DBP("-----Aktuelle Hüftposition: %d, Benötigte Bewegung: %d\n\n",prv_RobotPosition.Waist,aStepTuple1->Waist);

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
   double_t lcl_NewElbowPositionAngle1;
   double_t lcl_NewElbowPositionAngle2;
   if (!calcShoulderAndElbowRotation(lcl_X, lcl_Z, &lcl_NewShoulderPositionAngle1, &lcl_NewElbowPositionAngle1, &lcl_NewShoulderPositionAngle2, &lcl_NewElbowPositionAngle2)){
      DBNP("-----Bewegung fehlgeschlagen-----");
      return false;   
   }
   DBNP("-----Berechne Handgelenkwinkel um Hand korrekt an der Horizontalen auszurichten\n");
   double_t lcl_NewWristPositionAngle1 = aWristPitch - lcl_NewShoulderPositionAngle1 -lcl_NewElbowPositionAngle1;
   double_t lcl_NewWristPositionAngle2 = aWristPitch - lcl_NewShoulderPositionAngle2 -lcl_NewElbowPositionAngle2;
   DBP("-----Berechne Winkeltupel (Schulter, Ellbogen, Handgelenk): [%.2f°,%.2f°,%.2f°] und [%.2f°,%.2f°,%.2f°]\n\n",
         lcl_NewShoulderPositionAngle1, lcl_NewElbowPositionAngle1, lcl_NewWristPositionAngle1,
         lcl_NewShoulderPositionAngle2, lcl_NewElbowPositionAngle2, lcl_NewWristPositionAngle2);

   DBNP("-----Berechne benötigte Steps für erstes Tupel\n");
   calcShoulderElbowWristSteps(lcl_NewShoulderPositionAngle1, lcl_NewElbowPositionAngle1, lcl_NewWristPositionAngle1,
                              &(aStepTuple1->Shoulder), &(aStepTuple1->Elbow), &(aStepTuple1->WristPitch));
   DBNP("-----Berechne benötigte Steps für zweites Tupel\n");
   calcShoulderElbowWristSteps(lcl_NewShoulderPositionAngle2, lcl_NewElbowPositionAngle2, lcl_NewWristPositionAngle2,
                              &(aStepTuple2->Shoulder), &(aStepTuple2->Elbow), &(aStepTuple2->WristPitch));
   aStepTuple1->WristRoll = Degree2Steps(aWristRoll, e_joint_WristRoll);
   aStepTuple2->WristRoll = Degree2Steps(aWristRoll, e_joint_WristRoll);   
   return true;
}

Bool CheckRobotMovementAll(RobotPosition aRobotPosition){
   if (!CheckRobotMovement(aRobotPosition.Waist, e_joint_Waist))
      return false;
   if (!CheckRobotMovement(aRobotPosition.Shoulder, e_joint_Shoulder))
      return false;
   if (!CheckRobotMovement(aRobotPosition.Elbow, e_joint_Elbow))
      return false;
   if (!CheckRobotMovement(aRobotPosition.WristPitch, e_joint_WristPitch))
      return false;
   return true;
}

//X-Achse zeigt in Richtung Home position
//Z-Achse Richtung der Hüfte nach oben
//Y-achse bildet Vektorprodukt, so dass Rechtssystem entsteht
Bool MoveRoboterXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll) {
   RobotPosition lcl_StepTuple1, lcl_StepTuple2;

   if (!CalcStepsforNewPosition(X, Y, Z, aWristPitch, aWristRoll, &lcl_StepTuple1, &lcl_StepTuple2))
      return false;
   DBNP("-----Sende erstes Gelenktupel an den Roboter\n");
   if (MoveRoboterSteps(lcl_StepTuple1.Waist, lcl_StepTuple1.Shoulder, lcl_StepTuple1.Elbow,
      lcl_StepTuple1.WristPitch, lcl_StepTuple1.WristRoll)){
      DBNP("-----Bewegung Erfolgreich-----");  
      return true;   
   }
   if (!ParseError(getError())){
      DBNP("-----Bewegung fehlgeschlagen-----");
      return false;
   }
   DBNP("-----Erstes Gelenktupel gescheitert, Versuche zweites Gelenktupel\n");
   if (MoveRoboterSteps(lcl_StepTuple2.Waist, lcl_StepTuple2.Shoulder, lcl_StepTuple2.Elbow,
      lcl_StepTuple2.WristPitch, lcl_StepTuple2.WristRoll)){
      DBNP("-----Bewegung Erfolgreich-----");
      return true;
   } else {
      ParseError(getError());
      DBNP("-----Bewegung fehlgeschlagen-----");
      return false;
   }
}

Bool ValidateXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch) {
   RobotPosition lcl_StepTuple1, lcl_StepTuple2;

   if (!CalcStepsforNewPosition(X, Y, Z, aWristPitch, 0, &lcl_StepTuple1, &lcl_StepTuple2))
      return false;
   DBNP("-----Kontrolliere Positionen der Gelenke für das erste Tupel\n");   
   Bool lcl_TestTuple1 = CheckRobotMovementAll(lcl_StepTuple1); 
   DBNP("-----Kontrolliere Positionen der Gelenke für das erste Tupel\n");   
   Bool lcl_TestTuple2 = CheckRobotMovementAll(lcl_StepTuple2); 
   if (lcl_TestTuple1 || lcl_TestTuple2){
      DBNP("-----Koordinate durch mindestens ein Gelenktupel erreichbar");
      return true;
   }
   DBNP("-----Koordinate nicht erreichbar");
   return false;
}

int main (){
   MoveRoboterXYZ(437,0,160,0.0,0.0);
   if(ValidateXYZ(335,200,200,0.0))
      DBNP("\nWUHU\n");
   if(ValidateXYZ(200,-300,360,-10.0))
      DBNP("\nWUHU\n");

   printf("\n\n");
}
