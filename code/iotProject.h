#ifndef _IOTPROJECT_H_
#define _IOTPROJECT_H_

#include <Process.h>
#include <String.h>


/* Global variable */

// Tracker sensor input.
unsigned char trackerSensorIn = 0;
// The distance get from the ultrasound sensor.
int distance;

// The count of "getDistance()" , "loop()".
unsigned char count_getDistance = 0;
unsigned char count_loop = 0;
// Determine take a picture or not.
bool takePicture = false;

// Upload picture object , path , filename.
Process picture;
String path = "/mnt/sda1/";
String filename;


/* Function prototype */
void moveCar();
void readTrackerSensor();
void getDistance();
void uploadPicture();


#endif























