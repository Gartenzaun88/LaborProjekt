#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Control.h"

#define RAND_MAX 3000

int random_number(int min, int max)
{
   return (rand() % (max+1-min))+min;
}

int main (){
   //if(OpenPort("/dev/usb/lp3"))
  if(OpenPort("/dev/usb/lp3"))  
   {
      srand(time(NULL));
      int i = 0;
      while(i<10)
         if(MoveRoboterAlignedSteps(random_number(WAIST_LOW/2, WAIST_HIGH/2),
                                 random_number(SHOULDER_LOW/2, SHOULDER_HIGH/2),
                                 random_number(ELBOW_LOW/2, ELBOW_HIGH/2),0)){
            printf("\n\n\n\n------WUHU\n\n\n\n");
            fflush(stdout);
            i++;  
         }       
   MoveRoboterHome();
   ClosePort();
   }
   
}
/*
   MoveRoboterSteps(200,300,400,500,0);  
   MoveRoboterAlignedDegree(15.0, 14.0, -6.0, 0);
   MoveRoboterAlignedSteps(-250, -300, -200, 0);
   OpenGrabber();
   getc(stdin);*/