//Dieses Programm zeigt beispielaft, wie man Objekte transportiert
//Ein ca 7cm hohes Objekt wird bei 240/-50/-50 (Bodenhöhe) aufgegriffen und 
//bei 240/50/-50 abgesetzt#
//StartParameter ist der LPPort, an dem das Gerät angeschlossen ist.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Control.h"
#include "Rm101Kinematics.h"

int main (int argc, char *argv[]){
   //Pfad zum Port
   char PortPath[13];
   sprintf(PortPath,"%s%s","/dev/usb/lp", argc > 1 ? argv[1] : "1");
   //Versuchen Port zu öffnen
   if(OpenPort(PortPath))  
   {
      //Roboterewegungen
      MoveRoboterXYZ(240,-50, 30, -90, 15);
      OpenGrabber();
      MoveRoboterXYZ(240,-50, -10, -90, 0);
      CloseGrabber();
      MoveRoboterXYZ(240, 50, -10, -90, -30);
      OpenGrabber();
      MoveRoboterXYZ(240, 50, 30, -90, 0);
      //Pausiert bis Tastendruck
      printf("Bitte drücken sie eine Taste um in die Home Position zurückzukehren.\n");
      getc(stdin);
      //Roboter in Home Position bewegen
      MoveRoboterHome();
      //Port schließen
      ClosePort();
   }
   return 0;
}