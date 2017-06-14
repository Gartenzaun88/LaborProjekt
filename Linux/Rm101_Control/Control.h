#ifndef CONTROL_H
#define CONTROL_H

#include "Utility.h"
#include <math.h>

typedef enum {e_Grabber_close = 0, e_Grabber_open = 1} e_Grabber;

typedef enum {e_joint_Waist, e_joint_Shoulder, e_joint_Elbow, e_joint_WristPitch, e_joint_WristRoll} e_Joint;

typedef struct {
   int16_t Waist;
   int16_t Shoulder;
   int16_t Elbow;
   int16_t WristPitch;
   int16_t WristRoll;
   e_Grabber Wrist_Grabber;    
} RobotPosition, *RobotPositionPtr;

#define WAIST_LOW -3000
#define WAIST_HIGH 3000
#define SHOULDER_LOW -1750
#define SHOULDER_HIGH 2000
#define ELBOW_LOW -1000
#define ELBOW_HIGH 500
#define WRISTPITCH_LOW -1750 //Gegenläufig daher ein Gelenk Positiv eins negativ (erstes hier negativ) Arm zeit nach oben
#define WRISTPITCH_HIGH 1250

#define WAIST_STEPSPERDEGREE 25.0
#define SHOULDER_STEPSPERDEGREE 25.0
#define ELBOW_STEPSPERDEGREE 12.5
#define WRISTPITCH_STEPSPERDEGREE 19.44
#define WRISTROLL_STEPSPERDEGREE 20.0
#endif /* CONTROL_H */