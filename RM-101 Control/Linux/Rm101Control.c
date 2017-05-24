#include "Rm101Control.h"
#include "FileHandle.h"
#include <string.h>

static RobotPosition prv_ActualPosition = {0,0,0,0,0,e_Grabber_close};
extern Bool SendCmd(char* aCmd);

static void AddPositions(int16_t aWaist, int16_t aShoulder, int16_t aElbow, int16_t aWristPitch,int16_t aWristRoll){
   prv_ActualPosition.Waist += aWaist;
   prv_ActualPosition.Shoulder += aShoulder;
   prv_ActualPosition.Elbow += aElbow;
   prv_ActualPosition.WristPitch += aWristPitch;
   prv_ActualPosition.WristRoll += aWristRoll;
}

static void setGrabberPosition(e_Grabber aPosition){
   prv_ActualPosition.Wrist_Grabber = aPosition;
}

Bool CheckRobotMovement(int16_t aValue, e_Joint aJoint)
{
   int16_t lcl_Value = 0;
   int16_t lcl_Low = 0;
   int16_t lcl_High = 0;
   char lcl_Joint[11] = {0};
   switch (aJoint){
      case e_Joint_Waist: 
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
   DBP("Erfolgreich(%d)\n",lcl_newValue);
   return true;
}

Bool OpenGrabber(){
   DBNP("Öffne Greifer:...\n");
   if (!SendCmd("F"))
      return false;
   setGrabberPosition(e_Grabber_open);
   return true;
}

Bool CloseGrabber(){
   DBNP("Schließe Greifer:...\n");
   if (!SendCmd("C"))
      return false;
   setGrabberPosition(e_Grabber_close);
   return true;
}

Bool MoveSteps(int16_t aWaist, int16_t aShoulder, int16_t aElbow, int16_t aWristPitch,int16_t aWristRoll)
{
   DBP("Versuche Gelenke zu rotieren: %d,%d,%d,%d,%d:\n", aWaist, aShoulder, aElbow, aWristPitch, aWristRoll);
   DBP("  Aktuelle Position: %d,%d,%d,%d,%d:\n",prv_ActualPosition.Waist, prv_ActualPosition.Shoulder, prv_ActualPosition.Elbow, prv_ActualPosition.WristPitch, prv_ActualPosition.WristRoll);
   if(aWaist && !CheckRobotMovement(aWaist, e_Joint_Waist))
      return setError(ERROR_WAISTED);
   if(aShoulder && !CheckRobotMovement(aShoulder, e_joint_Shoulder))
      return setError(ERROR_SHOULDERED);
   if(aElbow && !CheckRobotMovement(aElbow, e_joint_Elbow))
      return setError(ERROR_ELBOWED);
   if(aWristPitch && !CheckRobotMovement(aWristPitch, e_joint_WristPitch))
      return setError(ERROR_WRISTED);
   char lcl_Cmd[25] = {0};
   sprintf(lcl_Cmd,"I%d,%d,%d,%d,%d,0",aWaist,aShoulder,aElbow,aWristPitch + aWristRoll, -aWristPitch + aWristRoll);
   if (!SendCmd(lcl_Cmd))
      return false;
   AddPositions(aWaist, aShoulder, aElbow, aWristPitch, aWristRoll);
   return true;
}

Bool MoveToHomePosition (){
   DBNP("Versuche Roboter zurück in Home-Position zu bewegen.\n");
   return MoveSteps(-prv_ActualPosition.Waist, -prv_ActualPosition.Shoulder, -prv_ActualPosition.Elbow, -prv_ActualPosition.WristPitch, -prv_ActualPosition.WristRoll) && CloseGrabber();
}

Bool MoveDegree(int16_t aWaist, int16_t aShoulder, int16_t aElbow, int16_t aWristPitch,int16_t aWristRoll)
{
   DBP("Versuche Gelenke zu rotieren: %d,%d,%d,%d,%d: Rechne um in Steps...\n", aWaist, aShoulder, aElbow, aWristPitch, aWristRoll);
   return MoveSteps(25 * aWaist, 25 * aShoulder, 25 * aElbow, (int16_t)(19.44 * aWristPitch), 20 * aWristRoll);   
}

int main () {
   OpenPort("/dev/ttyUSB0");
   MoveSteps(300,400,500,600,700);
   MoveSteps(-200,400,-300,-200,-340);
   MoveDegree(15,16,17,18,19);
   OpenGrabber();
   MoveToHomePosition();
   ClosePort();
return 0;
}