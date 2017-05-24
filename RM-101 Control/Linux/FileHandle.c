#include "FileHandle.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>

static FILE* prv_FileHandle = NULL;

Bool OpenPort(char* aPort) {
   DBNP("Versuche Port zu öffnen:\n");
   prv_FileHandle = fopen(aPort, "w");
   if (prv_FileHandle == NULL) {
      setError(ERROR_COULDNOTTOPENPORT);
      DBP("Port %s konnte nicht geöffnet werden! Errno: %d \n",aPort,errno);
      return false;
   }
   DBP("Port erfolgreich geöffnet. Handle: %d\n", fileno(prv_FileHandle));
   return true;
}

Bool SendCmd(char* aCmd){
   DBP("Sende Zeichenfolge: %s\n", aCmd);
   
   if (prv_FileHandle == NULL){
      setError(ERROR_INVALIDPORTHANDLE);
      DBNP("Fehlgeschlagen, Port geschlossen!\n");  
      return false;
   }
   
   if(fwrite(aCmd, 1, strlen(aCmd), prv_FileHandle) != strlen(aCmd)){
      setError(ERROR_SENDINGDATA);
      DBP("Fehlgeschlagen, keine oder wenige Bytes gesendet, Errno: %d\n", errno);  
      return false;
   }

   DBNP("Erfolgreich.\n");
   return true;
}

Bool ClosePort(void) {
   DBNP("Schließe Port:");
   if(fclose(prv_FileHandle) != 0){
      setError(ERROR_INVALIDPORTHANDLE);
      DBNP("Fehlgeschlagen, ungültiger Port!");
      return false;
   }
   DBNP("Erfolgreich\n");
   return true;
}
