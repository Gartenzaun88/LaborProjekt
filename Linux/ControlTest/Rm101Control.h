#ifndef RM101CONTROL_H
#define RM101CONTROL_H

#include <sys/types.h>
#include <math.h>
typedef enum{false = 0, true =1} Bool;

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

RobotPosition getPositions();
Bool CheckRobotMovement(int16_t aValue, e_Joint aJoint);
Bool OpenGrabber();
Bool CloseGrabber();
int16_t Degree2Steps(double_t aDegree, e_Joint aJoint);
double_t Steps2Degree (int16_t aSteps,e_Joint aJoint);
Bool MoveRoboterSteps(int16_t aWaist, int16_t aShoulder, int16_t aElbow, int16_t aWristPitch,int16_t aWristRoll);
Bool MoveRoboterDegree(double_t aWaist, double_t aShoulder, double_t aElbow, double_t aWristPitch, double_t aWristRoll);
Bool MoveRoboterAlignedDegree(double_t aWaist, double_t aShoulder, double_t aElbow, double_t aWristRoll);
Bool MoveRoboterAlignedSteps(int16_t aWaist, int16_t aShoulder, int16_t aElbow,int16_t aWristRoll);
Bool AlignGrabber();
Bool MoveRoboterHome();
Bool OpenPort(char* aPort);
Bool ClosePort(void); 
#endif /* RM101CONTROL_H */