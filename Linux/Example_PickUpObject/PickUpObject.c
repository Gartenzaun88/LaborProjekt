#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Control.h"
#include "Rm101Kinematics.h"

//Input ist der LPT Port
int main (int argc, char *argv[]){
   char PortPath[13];
   char Cmd[40];
   //CheckRobotMovement(30000, e_joint_Elbow);
   sprintf(PortPath,"%s%s","/dev/usb/lp", argc > 1 ? argv[1] : "1");
   if(OpenPort(PortPath))  
   {
      // while(1){
      //    int X, Y, Z;
      //    float Wristpinch, WristRoll;
      //    printf("\n\n\nBitte geben sie Koordinaten an für [X Y Z TCP-Neigung TCP-Drehung] oder 'h' um den Roboter in Home Position zu bewegen\n");
      //    printf(" Beispielsweise: \"240 -100 125 -90 90.0\"\n");
      //    fgets(Cmd,30,stdin);
      //    if(!(Cmd[0] == 'h')){
      //       sscanf(Cmd,"%d %d %d %f %f",&X, &Y, &Z, &Wristpinch, &WristRoll);
      //       printf("[%d %d %d %f %f]\n",X, Y, Z, Wristpinch, WristRoll);
      //       MoveRoboterXYZ(X, Y, Z, Wristpinch, WristRoll);
      //    } else
      //       break;
      // }  
         MoveRoboterXYZ(240,-50, 30, -90, 15);
         OpenGrabber();
         MoveRoboterXYZ(240,-50, -10, -90, 0);
         CloseGrabber();
         MoveRoboterXYZ(240, 50, -10, -90, -30);
         OpenGrabber();
         MoveRoboterXYZ(240, 50, 30, -90, 0);
         getc(stdin);
      MoveRoboterHome();
      ClosePort();
   }
   return 0;
}