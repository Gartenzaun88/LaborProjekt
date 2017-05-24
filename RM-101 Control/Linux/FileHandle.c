#include "FileHandle.h"
#include <string.h>

static FILE* prv_FileHandle = NULL;

Bool OpenPort(char* aPort) {
   DBNP("Versuche Port zu öffnen:\n");
   prv_FileHandle = fopen(aPort, "w");
   if (prv_FileHandle == NULL) {
      DBP("Port %s konnte nicht geöffnet werden! Errno: %d \n",aPort,errno);
      return setError(ERROR_COULDNOTTOPENPORT);
   }
   DBP("Port erfolgreich geöffnet. Handle: %d\n", fileno(prv_FileHandle));
   return true;
}

Bool SendCmd(char* aCmd){
   DBP("  Sende Zeichenfolge \"%s\":\n", aCmd);
   
   if (prv_FileHandle == NULL){
      DBNP("  Fehlgeschlagen, Port geschlossen!\n");
      return setError(ERROR_INVALIDPORTHANDLE); 
   }
   
   if(fwrite(aCmd, 1, strlen(aCmd), prv_FileHandle) != strlen(aCmd)){      
      DBP("  Fehlgeschlagen, keine oder wenige Bytes gesendet, Errno: %d\n", errno);  
      return setError(ERROR_SENDINGDATA);
   }
   DBNP("  Erfolgreich.\n");
   return true;
}

Bool ClosePort(void) {
   DBNP("Schließe Port: ");
   if(fclose(prv_FileHandle) != 0){      
      DBNP("Fehlgeschlagen, ungültiger Port!");
      return setError(ERROR_INVALIDPORTHANDLE);
   }
   DBNP("Erfolgreich\n");
   return true;
}
