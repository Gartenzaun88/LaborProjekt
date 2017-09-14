#ifndef KINEMATIC_H
#define KINEMATIC_H

#include <sys/types.h>
#include <math.h>
#include "Rm101Kinematics.h"

/** Kontrolliert für eine gesammte übergeben Roboterposition ob diese von den Gelenken eingenommen werden kann
 *  @param[in] aRobotPosition Position die kontrolliert werden soll
 *  @return True, wenn der sich wie angegeben positionieren kann
 */
Bool CheckRobotMovementAll(RobotPosition aRobotPosition);

/** Versucht den TCP des Roboters an eine gegene Koordinate zu fahren und setzt den Errorcode
 *  Neigung des TCP im Verhältnis zur horizontalen, sowie dessen Rotation müssen ebenfalls übergeben werden
 *  @param[in] X,Y,Z Koordinaten des anzufahrenden Punktes
 *  @param[in] aWristPitch Neigung des TCP gegenüber der horizontalen, Positiv nach oben geneigt
 *  @param[in] aWristRoll Rotation des TCP
 *  @return True, wenn der Roboter die angegebene Position anfährt
 */
Bool MoveRoboterXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch, double_t aWristRoll);

/** Prüft ob der Roboter eine bestimmte Koordinate mit der übergeben Neigung des TCP anfahren kann
 *  @param[in] X,Y,Z Koordinaten des anzufahrenden Punktes
 *  @param[in] aWristPitch Neigung des TCP gegenüber der horizontalen, Positiv nach oben geneigt
 *  @return True, wenn der Roboter die angegebene Position anfahren kann
 */
Bool ValidateXYZ(int16_t X, int16_t Y, int16_t Z, double_t aWristPitch);

#endif /* KINEMATIC_H */