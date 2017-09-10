#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Rm101Control.h"
#include "Rm101Kinematics.h"

int main (){
   // -225; //Minimal mögliche X Koordinate : -225
   // -440; //Minimal mög440liche Y Koordinate : -440
   // -100; //Minimal mögliche Z Koordiante: -100
   FILE *FileHandle = fopen("ReachTestFull.txt", "w");
   
   //Check auf Positive Koordinaten, Reichweite des Roboters ist maximal 440, Kugelradius kann also nicht größer sein
   for(int X = -225; X < 440; X = X + 5){
      for(int Y = -440; Y < 440; Y = Y + 5){
         for(int Z = 0; X * X + Y * Y + Z * Z < 360000; Z = Z + 5)
         {
            if (ValidateXYZ(X,Y,Z, 0.0))
               fprintf(FileHandle, "%d %d %d\n",X,Y,Z);
         }
         fprintf(FileHandle, "\n");
      }
   }
   fclose(FileHandle);
}