#include "Rm101Control.h"
#include "FileHandle.h"
#include <string.h>
#include <unistd.h>
#include <math.h>

static RobotPosition prv_ActualPosition = {0,0,0,0,0,e_Grabber_close};
extern Bool SendCmd(char* aCmd);

RobotPosition getPositions(){
   return prv_ActualPosition;
}

static void AddPositions(int16_t aWaist, int16_t aShoulder, int16_t aElbow, int16_t aWristPitch,int16_t aWristRoll){
   prv_ActualPosition.Waist += aWaist;
   prv_ActualPosition.Shoulder += aShoulder;
   prv_ActualPosition.Elbow += aElbow;
   prv_ActualPosition.WristPitch += aWristPitch;
   prv_ActualPosition.WristRoll += aWristRoll;
}

static void OpenOrCloseGrabber(e_Grabber aPosition){
   prv_ActualPosition.Wrist_Grabber = aPosition;
}

Bool CheckRobotMovement(int16_t aValue, e_Joint aJoint)
{
   int16_t lcl_Value = 0;
   int16_t lcl_Low = 0;
   int16_t lcl_High = 0;
   char lcl_Joint[11] = {0};
   switch (aJoint){
      case e_joint_Waist: 
         lcl_Value = prv_ActualPosition.Waist;
         lcl_Low = WAIST_LOW;
         lcl_High = WAIST_HIGH;
         strcpy(lcl_Joint, "Waist");
         break;
      case e_joint_Shoulder: 
         lcl_Value = prv_ActualPosition.Shoulder;
         lcl_Low = SHOULDER_LOW;
         lcl_High = SHOULDER_HIGH;
         strcpy(lcl_Joint, "Shoulder");
         break;
      case e_joint_Elbow: 
         lcl_Value = prv_ActualPosition.Elbow;
         lcl_Low = ELBOW_LOW;
         lcl_High = ELBOW_HIGH;
         strcpy(lcl_Joint, "Elbow");
         break; 
      case e_joint_WristPitch: 
         lcl_Value = prv_ActualPosition.WristPitch;
         lcl_Low = WRISTPITCH_LOW;
         lcl_High = WRISTPITCH_HIGH;
         strcpy(lcl_Joint, "WristPitch");
         break;
      default:;
   }
   DBP("  Kontrolliere Kollision für %s-Gelenk: ", lcl_Joint);
   int16_t lcl_newValue = lcl_Value + aValue;
   if( lcl_newValue > lcl_High){
      DBP("Fehlgeschlagen (%d/%d)\n",lcl_newValue, lcl_High);
      return false;
   }
   if(lcl_Value + aValue < lcl_Low){
      DBP("Fehlgeschlagen (%d/%d)\n",lcl_newValue, lcl_Low);
      return false;
   }
   DBNP("Erfolgreich!\n");
   return true;
}

Bool OpenGrabber(){
   DBNP("Öffne Greifer:...\n");
   if (!SendCmd("F\n"))
      return false;
   OpenOrCloseGrabber(e_Grabber_open);
   return true;
}

Bool CloseGrabber(){
   DBNP("Schließe Greifer:...\n");
   if (!SendCmd("C\n"))
      return false;
   OpenOrCloseGrabber(e_Grabber_close);
   return true;
}

int16_t Degree2Steps(double_t aDegree, e_Joint aJoint){
   if (!aDegree)
      return 0;
   switch(aJoint)
   {
      case e_joint_Waist: return (int)(aDegree * WAIST_STEPSPERDEGREE);
      case e_joint_Elbow: return (int)(aDegree * ELBOW_STEPSPERDEGREE);
      case e_joint_Shoulder: return (int)(aDegree * SHOULDER_STEPSPERDEGREE);
      case e_joint_WristPitch: return (int)(aDegree * WRISTPITCH_STEPSPERDEGREE);
      case e_joint_WristRoll: return (int)(aDegree * WRISTROLL_STEPSPERDEGREE);
      default: return 0;
   }
}

double_t Steps2Degree (int16_t aSteps,e_Joint aJoint){
   if (!aSteps)
      return 0;
   switch(aJoint)
   {
      case e_joint_Waist: return (aSteps / WAIST_STEPSPERDEGREE);
      case e_joint_Elbow: return (aSteps / ELBOW_STEPSPERDEGREE);
      case e_joint_Shoulder: return (aSteps / SHOULDER_STEPSPERDEGREE);
      case e_joint_WristPitch: return (aSteps / WRISTPITCH_STEPSPERDEGREE);
      case e_joint_WristRoll: return (aSteps / WRISTROLL_STEPSPERDEGREE);
      default: return 0;
   }
}

Bool MoveRoboterSteps(int16_t aWaist, int16_t aShoulder, int16_t aElbow, int16_t aWristPitch,int16_t aWristRoll)
{
   DBP("Versuche Gelenke zu rotieren: %d,%d,%d,%d,%d:\n", aWaist, aShoulder, aElbow, aWristPitch, aWristRoll);
   DBP("  Aktuelle Position: %d,%d,%d,%d,%d:\n",prv_ActualPosition.Waist, prv_ActualPosition.Shoulder, prv_ActualPosition.Elbow, prv_ActualPosition.WristPitch, prv_ActualPosition.WristRoll);
   if(aWaist && !CheckRobotMovement(aWaist, e_joint_Waist))
      return setError(ERROR_WAISTED);
   if(aShoulder && !CheckRobotMovement(aShoulder, e_joint_Shoulder))
      return setError(ERROR_SHOULDERED);
   if(aElbow && !CheckRobotMovement(aElbow, e_joint_Elbow))
      return setError(ERROR_ELBOWED);
   if(aWristPitch && !CheckRobotMovement(aWristPitch, e_joint_WristPitch))
      return setError(ERROR_WRISTED);
   char lcl_Cmd[25] = {0};
   sprintf(lcl_Cmd,"I%d,%d,%d,%d,%d,0\n",aWaist,aShoulder,aElbow,-aWristPitch + aWristRoll, aWristPitch + aWristRoll);
   if (!SendCmd(lcl_Cmd))
      return false;
   AddPositions(aWaist, aShoulder, aElbow, aWristPitch, aWristRoll);
   DBP("  -->Neue Position: %d,%d,%d,%d,%d:\n\n",prv_ActualPosition.Waist, prv_ActualPosition.Shoulder, prv_ActualPosition.Elbow, prv_ActualPosition.WristPitch, prv_ActualPosition.WristRoll);
   return true;
}

Bool MoveRoboterDegree(double_t aWaist, double_t aShoulder, double_t aElbow, double_t aWristPitch, double_t aWristRoll)
{
   DBP("Versuche Gelenke zu rotieren: %.2f°,%.2f°,%.2f°,%.2f°,%.2f°: Rechne um in Steps...\n", aWaist, aShoulder, aElbow, aWristPitch, aWristRoll);
   return MoveRoboterSteps(Degree2Steps(aWaist, e_joint_Waist),
                           Degree2Steps(aShoulder, e_joint_Shoulder),
                           Degree2Steps(aElbow, e_joint_Elbow),
                           Degree2Steps(aWristPitch, e_joint_WristPitch),
                           Degree2Steps(aWristRoll, e_joint_WristRoll));  
}

Bool MoveRoboterAlignedDegree(double_t aWaist, double_t aShoulder, double_t aElbow, double_t aWristRoll)
{
   DBP("Versuche Gelenke zu rotieren und Greifer parallel zum Boden auszurichten:  %.2f°,%.2f°,%.2f°,%.2f°\n", aWaist, aShoulder, aElbow, aWristRoll);
   double_t lcl_WristMoovement = -(aShoulder + aElbow + Steps2Degree(prv_ActualPosition.Elbow, e_joint_Elbow) 
                                 + Steps2Degree(prv_ActualPosition.Shoulder, e_joint_Shoulder)
                                 + Steps2Degree(prv_ActualPosition.WristPitch, e_joint_WristPitch));
   DBP("----%f----",Steps2Degree(prv_ActualPosition.Elbow, e_joint_Elbow) 
                                 + Steps2Degree(prv_ActualPosition.Shoulder, e_joint_Shoulder));
   return MoveRoboterDegree(aWaist, aShoulder, aElbow, lcl_WristMoovement, aWristRoll);  
}

Bool MoveRoboterAlignedSteps(int16_t aWaist, int16_t aShoulder, int16_t aElbow,int16_t aWristRoll){
return MoveRoboterAlignedDegree(Steps2Degree(aWaist, e_joint_Waist),
                                 Steps2Degree(aShoulder, e_joint_Shoulder),
                                 Steps2Degree(aElbow, e_joint_Elbow),
                                 Steps2Degree(aWristRoll, e_joint_WristRoll));
}

Bool AlignGrabber(){
   return MoveRoboterAlignedDegree(0,0,0,0);
}

Bool MoveRoboterHome(){
   DBNP("Versuche Roboter zurück in Home-Position zu bewegen.\n");
   return MoveRoboterSteps(-prv_ActualPosition.Waist, -prv_ActualPosition.Shoulder, -prv_ActualPosition.Elbow, -prv_ActualPosition.WristPitch, -prv_ActualPosition.WristRoll) && CloseGrabber();
}

int main () {
   OpenPort("/dev/usb/lp3");
   MoveRoboterSteps(200,300,400,500,0);  
   MoveRoboterAlignedDegree(15.0, 14.0, -6.0, 0);
   MoveRoboterAlignedSteps(-250, -300, -200, 0);
   OpenGrabber();
   getc(stdin);
   MoveRoboterHome();
   ClosePort();
   return 0;
}