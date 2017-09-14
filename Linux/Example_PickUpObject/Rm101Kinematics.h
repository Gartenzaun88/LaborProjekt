#ifndef KINEMATIC_H
#define KINEMATIC_H

#include <sys/types.h>
#include <math.h>
#include "Rm101Control.h"

/** Kontrolliet ob die übergebene RoboterPosition vom Roboter eingenommen werden kann
 *  @param[in] aRobotPosition Struct mit den Steps für jedes Gelenk 
 *  @return True, wenn die übergebene Position angenommen werden aknn
 */
Bool CheckRobotMovementAll(RobotPosition aRobotPosition);

/** Versucht den Roboter so auszurichten, dass sein TCP an der [X,Y,Z] Koordinate ist
 *  @param[in] X,Y,Z Zielkoordinaten
 *  @param[in] aWristpitch Neigung, welcher der TCP relativ zur Horizontalen haben soll
 *  @param[in] aWristRoll Rotation, um welche der TCP gedreht sein soll
 *  @return True, wenn der Roboter die angegebene Position anfährt
 */
Bool MoveRoboterXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll);

/** Kontrolliert, ob der TCP des Roboters die [X,Y,Z] Koordinate erreichen kann
 *  @param[in] X,Y,Z Zielkoordinaten
 *  @param[in] aWristpitch Neigung, welcher der TCP relativ zur Horizontalen haben soll
 *  @return True, wenn der Roboter die angegebene erreichen kann
 */
Bool ValidateXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch);

#endif /* KINEMATIC_H */