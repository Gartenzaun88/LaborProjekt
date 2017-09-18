//Dieses Programm zeigt beispielhaft, wie Nutzereingaben verwendet werden können
//um bestimmte Positionen anzufahren
//Startargument ist der LPPort, an dem der Roboter angeschlossen ist
//Wird das Programm zwischendurch beendet muss der Roboter manuell in
//Home Position zurückgesetzt werden
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Kinematics.h"

//Input ist der LPT Port
int main (int argc, char *argv[]){
   //Pfad zum Port
   char PortPath[13];
   //Buffer für kommendes Kommando
   char Cmd[40];
   sprintf(PortPath,"%s%s","/dev/usb/lp", argc > 1 ? argv[1] : "1");
   //Versuchen Port zu öffnen
   ValidateXYZ(200,-200,100,50.0);
   if(OpenPort(PortPath))  
   {
      while(1){
         int X, Y, Z;
         float Wristpinch, WristRoll;
         //Abfragen der anzufahrenden Position
         printf("\n\n\nBitte geben sie Koordinaten an für [X Y Z TCP-Neigung TCP-Drehung] oder 'h' um den Roboter in Home Position zu bewegen\n");
         printf(" Beispielsweise: \"240 -100 125 -90 90.0\"\n");
         fgets(Cmd,30,stdin);
         //Abfrage ob 'h' eingegeben wurde
         if(!(Cmd[0] == 'h')){
            //Kontrolle ob Eingabe gültig ist
            if (sscanf(Cmd,"%d %d %d %f %f",&X, &Y, &Z, &Wristpinch, &WristRoll) == 5) {
               printf("[%d %d %d %f %f]\n",X, Y, Z, Wristpinch, WristRoll);
               MoveRoboterXYZ(X, Y, Z, Wristpinch, WristRoll);
            } else 
            //Ansonsten neuer Versuch
               printf("Ungültige Eingabe, Versuchen sie es erneut\n");              
         } else
            //Bei 'h' beende die Schleife
            break;
      }  
      //Roboter in Home Position bewegen
      MoveRoboterHome();
      //Port schließen
      ClosePort();
   }
   return 0;
}