//Bibliothek zum Kontrollieren der Bewegungen des RM-101

#ifndef RM101CONTROL_H
#define RM101CONTROL_H

#include <sys/types.h>
#include <math.h>
//Enum um Wahrheitswerte zu repräsentieren
typedef enum{false = 0, true =1} Bool;

//Enum über Status des Greifers
typedef enum {e_Grabber_close = 0, e_Grabber_open = 1} e_Grabber;

//Enum der einzelnen Gelenke
typedef enum {e_joint_Waist, e_joint_Shoulder, e_joint_Elbow, e_joint_WristPitch, e_joint_WristRoll} e_Joint;

//Strukt in dem eine Roboterposition in Anzahl von Motorsteps gespeichert werden kann
typedef struct {
   int16_t Waist;
   int16_t Shoulder;
   int16_t Elbow;
   int16_t WristPitch;
   int16_t WristRoll;
   e_Grabber Wrist_Grabber;    
} RobotPosition, *RobotPositionPtr;


/** Versucht den LPT-Port an den der Roboter angeschlossen ist zu öffnen und setzt den Fehlercode
 * Zwingend zur Kommunikation mit dem Roboter benötigt
 * Zur Öffnung des Ports werden die Rechte an diesem benötigt!!!
 * @return True, wenn der Port geöffnet werden konnt
 */
Bool OpenPort(char* aPort);

/** Schließt den LPt-Port an den der Roboter angeschlossen ist und setzt den Fehlercode
 * @return True, wenn der Port geschlossen werden konnte
 */
Bool ClosePort(void); 

/** Liefert die momentan Roboterposition Relativ zur Position auf der der Roboter angeschaltet wurde  
 *  @return Die Roboterposition.  
 */
RobotPosition getPositions();

/** Kontrolliert ob ein Gelenk um die gegebene Anzahl an Steps gedreht werden kann  
 *  @param[in] aValue Steps, die das Gelenk gedreht werden soll 
 *  @param[in] aJoint Enum des Gelenk, welches rotiert werden soll
 *  @return True wenn das Gelenk gedreht werden kann
 */
Bool CheckRobotMovement(int16_t aValue, e_Joint aJoint);

/** Versucht den Greifer zu öffnen, und setzt entsprechenden Fehlercode
 *  @return True, wenn Aktion erfolgreich ist
 */
Bool OpenGrabber();

/** Versucht den Greifer zu schließen, und setzt entsprechenden Fehlercode
 *  @return True, wenn Aktion erfolgreich ist
 */
Bool CloseGrabber();

/** Rechnet ein gegebenen Rotationwinkel um ein Gelenk in die korrekte Anzahl an Steps um  
 *  @param[in] aDegree Winkel der umgerechnet werden soll
 *  @param[in] aJoint Enum des Gelenk, für welches die Steps berechnet werden
 *  @return Anzahl an benötigten Steps
 */
int16_t Degree2Steps(double_t aDegree, e_Joint aJoint);

/** Rechnet eine Anzahl an Steps von einem Gelenkmotor in den zugehörigen Rotationwinkel um   
 *  @param[in] aSteps Steps, die umgerechnet werden sollen
 *  @param[in] aJoint Enum des Gelenk, für welches die Steps berechnet werden
 *  @return Rotationswinkel
 */
double_t Steps2Degree (int16_t aSteps,e_Joint aJoint);

/** Versucht jedes Gelenk des Roboters um eine gegebene Anzahl Steps zu rotieren und setzt den Fehlercode.
 *  Sammelt alle Rotationen und sendet diese in einem Befehl, nur erfolgreich wenn alle Rotationen legal sind.   
 *  @param[in] aWaist Steps, um welche das Hüftgelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aShoulder Steps, um welche das Schultergelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aElbow Steps, um welche das Ellbogengelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aWristpitch Steps, um welche das Handgelenk geneigt werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aWristRoll Steps, um welche das Handgelenk rotiert werden soll
 *  @return True, wenn der Roboter die angegebene Position anfährt
 */
Bool MoveRoboterSteps(int16_t aWaist, int16_t aShoulder, int16_t aElbow, int16_t aWristPitch,int16_t aWristRoll);

/** Versucht jedes Gelenk des Roboters um einen gegebenen Winkel zu rotieren und setzt den Fehlercode.
 *  Sammelt alle Rotationen und sendet diese in einem Befehl, nur erfolgreich wenn alle Rotationen legal sind.   
 *  @param[in] aWaist Winkel, um den das Hüftgelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aShoulder Winkel, um den das Schultergelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aElbow Winkel, um den das Ellbogengelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aWristpitch Winkel, um den das Handgelenk geneigt werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aWristRoll Winkel, um den das Handgelenk rotiert werden soll
 *  @return True, wenn der Roboter die angegebene Position anfährt
 */
Bool MoveRoboterDegree(double_t aWaist, double_t aShoulder, double_t aElbow, double_t aWristPitch, double_t aWristRoll);

/** Versucht jedes Gelenk des Roboters um einen gegebenen Winkel zu rotieren, wobei der Greifer 
 *  Parallel zum Boden ausgerichtet ist und setzt den Fehlercode.
 *  Sammelt alle Rotationen und sendet diese in einem Befehl, nur erfolgreich wenn alle Rotationen legal sind.   
 *  @param[in] aWaist Winkel, um den das Hüftgelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aShoulder Winkel, um den das Schultergelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aElbow Winkel, um den das Ellbogengelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aWristRoll Winkel, um den das Handgelenk rotiert werden soll
 *  @return True, wenn der Roboter die angegebene Position anfährt
 */
Bool MoveRoboterAlignedDegree(double_t aWaist, double_t aShoulder, double_t aElbow, double_t aWristRoll);

/** Versucht jedes Gelenk des Roboters um eine gegebene Anzahl Steps zu rotieren , wobei der Greifer 
 *  Parallel zum Boden ausgerichtet ist und setzt den Fehlercode.
 *  Sammelt alle Rotationen und sendet diese in einem Befehl, nur erfolgreich wenn alle Rotationen legal sind.   
 *  @param[in] aWaist Steps, um welche das Hüftgelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aShoulder Steps, um welche das Schultergelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aElbow Steps, um welche das Ellbogengelenk rotiert werden soll, Positiv: Gegen den Uhrzeigersinn
 *  @param[in] aWristRoll Steps, um welche das Handgelenk rotiert werden soll
 *  @return True, wenn der Roboter die angegebene Position anfährt
 */
Bool MoveRoboterAlignedSteps(int16_t aWaist, int16_t aShoulder, int16_t aElbow,int16_t aWristRoll);

/** Versucht den Greifer parallel zum Boden auszurichten und setzt den Fehlercode
 *@return True, wenn die Bewegung möglich ist und der Roboter diese ausführt
 */
Bool AlignGrabber();

/** Bewegt den Roboter zurück in die Position, die er nach dem einschalten innehatte.
 * Punkte auf den Einzelnen Armen zeigen die korrekte Homeposition an
 */
Bool MoveRoboterHome();

/** Gibt den Fehlercode des letzten aufgetretenden Fehlers zurück
 * @return Der Fehlercode, unten in der Datei sind diese beschrieben
 */
u_int8_t getError(void);

//Minimale und Maximale Stepwerte der einzelnen Gelenke
#define WAIST_LOW -3000
#define WAIST_HIGH 3000
#define SHOULDER_LOW -1750
#define SHOULDER_HIGH 2000
#define ELBOW_LOW -1000
#define ELBOW_HIGH 500
#define WRISTPITCH_LOW -1750 //Gegenläufig daher ein Gelenk Positiv eins negativ (erstes hier negativ) Arm zeit nach oben
#define WRISTPITCH_HIGH 1250

//Fehlercodes. Für Systemfehler siehe SystemVariable errno(Linux) oder getlasterror(Windows)
#define ERROR_COULDNOTTOPENPORT 1 //Port konnte nicht geöffnet werden
#define ERROR_INVALIDPORTHANDLE 2 //Ungültiger/nicht existierender Port
#define ERROR_SENDINGDATA 3 //Keine oder nicht alle Daten konnten gesendet werden
#define ERROR_WAISTED 4 //Unmögliche Rotation um Hüft-Gelenk
#define ERROR_SHOULDERED 5 //Unmögliche Rotation um Schulter-Gelenk
#define ERROR_ELBOWED 6 // Unmögliche Rotation um Ellbogen-Gelenk
#define ERROR_WRISTED 7 //Unmögliche Rotation um Hand-Gelenk (Wrist-Roll)

#endif /* RM101CONTROL_H */