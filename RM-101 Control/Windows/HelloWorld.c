#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif
#define DBP(fmt, ...)                                  \
  do {                                                 \
    if (DEBUG_TEST) fprintf(stderr, fmt, __VA_ARGS__); \
  } while (0)
  
#define DBNP(fmt)                         \
  do {                                    \
    if (DEBUG_TEST) fprintf(stderr, fmt); \
  } while (0)

static HANDLE prv_PortHandle = 0;

BOOL OpenComPort(uint8_t aComPort) {
  char lcl_PortNumber[3] = {0};
  char lcl_PortFQDN[11] = "\\\\.\\LPT";
  itoa(aComPort, lcl_PortNumber, 10);
  strcat(lcl_PortFQDN, lcl_PortNumber);

  DBP("Pfad zum Com-Port: %s \n", lcl_PortFQDN);

  prv_PortHandle = CreateFile(TEXT(lcl_PortFQDN), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
  if (prv_PortHandle == INVALID_HANDLE_VALUE) {
    DBP("Verbindung zum Com-Port konnte nicht hergestellt werden. Errorcode: %d \n", GetLastError());
    return FALSE;
  }
  DBNP("Verbindung zum ComPort hergestellt!\n");
  return TRUE;
}

BOOL getPortConfig(LPDCB aPortConfig) {
  aPortConfig->DCBlength = sizeof(DCB);
  if ((GetCommState(prv_PortHandle, aPortConfig) == 0)) {
    DBP("Com-Port Config konnte nicht abgefragt werden. Errorcode :%d\n", GetLastError());
    return FALSE;
  }
  DBNP("Com-Port Config erhalten.\n");
  return TRUE;
}

BOOL sendBytes(char *aMessage) {
  DWORD lcl_MessageLength = strlen(aMessage);
  DWORD lcl_BytesSend = 0;
  if (!WriteFile(prv_PortHandle, aMessage, lcl_MessageLength, &lcl_BytesSend, NULL)) {
    DBP("Daten konnten nicht gesendet werden. Errorcode: %d\n", GetLastError());
    return FALSE;
  }
  if (lcl_BytesSend != lcl_MessageLength)
    DBP("Nicht alle Daten wurden geschrieben. %d von %d Bytes!\n", lcl_BytesSend, lcl_BytesSend + lcl_MessageLength);
  DBP("Daten: \"%s\" erfolgreich gesendet", aMessage);
  return TRUE;
}

int main(int argc, char *argv[]) {
  if (argc > 1)
    OpenComPort(3);
  else
    OpenComPort(1);
  DCB lcl_ComConfig;
  getPortConfig(&lcl_ComConfig);

  printf("Current Settings\n Baud Rate %d\n Parity %d\n Byte Size %d\n Stop Bits %d\n", lcl_ComConfig.BaudRate,
         lcl_ComConfig.Parity, lcl_ComConfig.ByteSize, lcl_ComConfig.StopBits);
  printf("Kommando eingeben:\n");
  char *Inputbuffer = NULL;
  int g =4;
  int test =getline(&Inputbuffer, &argc, stdin);
  sendBytes(Inputbuffer);

  free(Inputbuffer);

  return 0;
}