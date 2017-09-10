#ifndef KINEMATIC_H
#define KINEMATIC_H

#include <sys/types.h>
#include <math.h>
#include "Rm101Kinematics.h"

Bool CheckRobotMovementAll(RobotPosition aRobotPosition);
Bool MoveRoboterXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll);
Bool ValidateXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch);

#endif /* KINEMATIC_H */