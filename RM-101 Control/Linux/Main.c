#include <stdio.h>
#include <string.h>
#include "Rm101Control.h"



int main (){
   //if(OpenPort("/dev/usb/lp3"))
   OpenPort("/dev/usb/lp3");
   {
   MoveRoboterSteps(200,300,400,500,0);  
   MoveRoboterAlignedDegree(15.0, 14.0, -6.0, 0);
   MoveRoboterAlignedSteps(-250, -300, -200, 0);
   OpenGrabber();
   getc(stdin);
   MoveRoboterHome();
   ClosePort();
   }
}
