//Dieses Programm lässt den Roboter zufällige aber meist mögliche Positionen ausprobieren
//und fährt 10 verschiedene davon an, dabei richtet er sein Greifer immer parallel zum Boden aus
//Sobald alle angefahren sind, kehrt er in die Homeposition zurück
//Startargument ist der Port an dem der Roboter angeschlossen ist
//Da sowohl der Roboter als auch der Filecontroller Befehle Cashed, wird die Debug Ausgabe der
//tatsächlichen Bewegung weit vorraus sein
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Control.h"

#define RAND_MAX 3000

//Funktion zum generieren einer Zufälligen Zahl zwischen zwei Grenzen
int random_number(int min, int max)
{
   return (rand() % (max+1-min))+min;
}

int main (int argc, char *argv[]){
   //Pfad zum Port
   char PortPath[13];
   sprintf(PortPath,"%s%s","/dev/usb/lp", argc > 1 ? argv[1] : "1");
   CheckRobotMovement(3000, e_joint_Elbow);
   //Versuchen Port zu öffnen 
   if(OpenPort(PortPath))  
   {  
      //Zufallszahl initialisieren
      srand(time(NULL));
      int i = 0;
      while(i<10)
         //Roboter zufällig bewegen
         if(MoveRoboterAlignedSteps(random_number(WAIST_LOW/2, WAIST_HIGH/2),
                                 random_number(SHOULDER_LOW/2, SHOULDER_HIGH/2),
                                 random_number(ELBOW_LOW/2, ELBOW_HIGH/2),0)){
            printf("\n\n\n\nBewegung erfolgreich\n\n\n\n");
            fflush(stdout);
            i++;  
         }    
      //Roboter in Home Position bewegen
      MoveRoboterHome();
      //Port schließen
      ClosePort();
   }
   
}
/*
   MoveRoboterSteps(200,300,400,500,0);  
   MoveRoboterAlignedDegree(15.0, 14.0, -6.0, 0);
   MoveRoboterAlignedSteps(-250, -300, -200, 0);
   OpenGrabber();
   getc(stdin);*/
