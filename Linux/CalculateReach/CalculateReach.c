#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Control.h"
#include "Rm101Kinematics.h"

int main (int argc, char *argv[]){
   // -225; //Minimal mögliche X Koordinate : -225
   // -440; //Minimal mög440liche Y Koordinate : -440
   // -100; //Minimal mögliche Z Koordiante: -100
   double Wristpinch = argc > 1 ? atof(argv[1]) : 0.0;
   FILE *FileHandle = fopen("ReachTestFull.txt", "w");
   FILE *FileHandlePartial = fopen("ReachTestPartial.txt", "w");
   int i = 0;
   //Check auf Positive Koordinaten, Reichweite des Roboters ist maximal 440, Kugelradius kann also nicht größer sein
   for(int X = -225; X < 440; X = X + 10){
      for(int Y = -440; Y < 440; Y = Y + 10){
         for(int Z = -100; X * X + Y * Y + Z * Z < 360000; Z = Z + 10)
         {
            if (ValidateXYZ(X,Y,Z, Wristpinch)){
               fprintf(FileHandle, "%d %d %d\n",X,Y,Z);
               if (Y % 110 == 0)
                  fprintf(FileHandlePartial, "%d %d %d\n",X,Y,Z);
                  i++;
            }
         }
         fprintf(FileHandle, "\n");
      }
   }
   fclose(FileHandlePartial);
   fclose(FileHandle);
   printf("Liste erfolgreich erstellt: %d Koordinaten geschrieben", i);
}