//Dieses Programm testet für ein gegebenen Neigungswinke des TCP die erreichbaren Punkte
//und schreibt diese in 4 Dateien, einmal eine Gesamtübersicht und je einmal pro Achse eine
//in Scheiben geschnittene Übersicht, im Unterordnerliegen 4 gnuplot Scripte um die
//Dateien und die erreichbarkeit der einzelnen Punkte grafisch darzustellen
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Kinematics.h"

//Regelt die Schrittweite bei langsamen Computern auf 10 setzen
#define STEPWIDTH 5

int main (int argc, char *argv[]){
//Minimal mögliche X Koordinate : -225
//Minimal mög440liche Y Koordinate : -440
//Minimal mögliche Z Koordiante: -100
   float Wristpinch = argc > 1 ? atof(argv[1]) : 0.0;
   printf("Berechne Reichweite für TCP-Neigung von: %.2f°\n", Wristpinch);
   //Öffnen de Filehandler
   FILE *FileHandle = fopen("Gnuplot/ReachTestFull.txt", "w");
   FILE *FileHandlePartialX = fopen("Gnuplot/ReachTestPartialX.txt", "w");
   FILE *FileHandlePartialY = fopen("Gnuplot/ReachTestPartialY.txt", "w");
   FILE *FileHandlePartialZ = fopen("Gnuplot/ReachTestPartialZ.txt", "w");
   int i = 0;
   for(int X = -300; X < 440; X = X + STEPWIDTH){
      for(int Y = -440; Y < 440; Y = Y + STEPWIDTH){
         // Reichweite des Roboters ist maximal 600 (in Z Richtung sonst ca 440)
         // Kugelradius kann also nicht größer sein
         for(int Z = -100; X * X + Y * Y + Z * Z < 360000; Z = Z + STEPWIDTH)
         {
            //Kontrollieren ob der Punkt Valide ist und schreiben in die zugehörigen Dateien
            if (ValidateXYZ(X,Y,Z, Wristpinch)){
               fprintf(FileHandle, "%d %d %d\n",X,Y,Z);
               if (Y % 110 == 0)
                  fprintf(FileHandlePartialY, "%d %d %d\n",X,Y,Z);
               if (Z % 100 == 0)
                  fprintf(FileHandlePartialZ, "%d %d %d\n",X,Y,Z);
               if (X % 100 == 0)
                  fprintf(FileHandlePartialX, "%d %d %d\n",X,Y,Z);
               i++;
            }
         }
      }
   }
   //Schließen der Dateien
   fclose(FileHandlePartialX);
   fclose(FileHandlePartialY);
   fclose(FileHandlePartialZ);
   fclose(FileHandle);
   printf("Liste erfolgreich erstellt: Bis zu %d Koordinaten geschrieben!\n", i);
   return 0;
}