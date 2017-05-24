#ifndef RM101CONTROL_H
#define RM101CONTROL_H

#include "Utility.h"

typedef enum {e_grabber_close = 0, e_grabber_open = 1} e_grabber;

typedef struct {
   int16_t Waist;
   int16_t Shoulder;
   int16_t Elbow;
   int16_t WristPitch;
   int16_t WristRoll;
   e_grabber Wrist_Grabber;    
} RobotPosition, *RobotPositionPtr;

#define WAIST_LOW -3000
#define WAIST_HIGH 3000
#define SHOULDER_LOW -750
#define SHOULDER_HIGH 2000
#define ELBOW_LOW -975
#define ELBOW_HIGH 500
#define WRISTPITCH_LOW 1250 //Gegenläufig daher ein Gelenk Positiv eins negativ (erstes hier negativ)
#define WRISTPITCH_HIGH 1750

#define WAIST_STEPSPERDEGREE 25
#define SHOULDER_STEPSPERDEGREE 25
#define ELBOW_STEPSPERDEGREE 25
#define WRIST_STEPSPERDEGREE 20

#endif /* RM101CONTROL_H */