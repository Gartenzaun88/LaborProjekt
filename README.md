# LaborProjekt
Das Repository beinhaltet mehrere C-Bibliothek zur Steuerung des RM101 Movemaster von Mitsubishi

RM101-Control ist eine C-Bibliothek zur Steuerung des Roboters.

RM101-Kinematics ist eine Bibliothek, welche auf RM101-Control aufbaut und die Inverse Kinematik des Roboters berechnet, dadurch ist es möglich den Roboter auch zu Koordinaten zu schicken.

Die Example-Ordner beinhalten Beispiel Programme zum benutzen der Bibliotheken.

Das CalculateReach Programm berechnet für ein übergebenen Winkel des TCP zur horizontalen, alle erreichbaren Punkte und speichert diese in Textdateien. 
Im Unterordner GnuPlot sind 4 Gnuplot-Scripte zur grafischen Auswertung der errechneten möglichen Positionen
