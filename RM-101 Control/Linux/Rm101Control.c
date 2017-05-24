
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "Rm101Control.h"
#include "FileHandle.h"

static RobotPosition glb_ActualPosition = {0,0,0,0,0,e_grabber_close};

int main () {
   OpenPort("/dev/ttyUSB0");
   SendCmd("Toll");
   ClosePort();
return 0;
}