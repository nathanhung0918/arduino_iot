#include <Bridge.h>
#include "inputPin.h"
#include "carControl.h"
#include "iotProject.h"

#define DEBUG


/* Initialization */
void setup() {
	Bridge.begin();
	Console.begin();

	while( !Console );
	
	// Initialize car pin mode.
	servoLeft.attach( PIN_CAR_LEFT );
	servoRight.attach( PIN_CAR_RIGHT );

	// Initialize tracker sensor pin mode.
	pinMode( PIN_TRACKER_LEFT, INPUT );
	pinMode( PIN_TRACKER_CENTERRIGHT, INPUT );
	pinMode( PIN_TRACKER_CENTER, INPUT );
	pinMode( PIN_TRACKER_CENTERRIGHT, INPUT );
	pinMode( PIN_TRACKER_RIGHT, INPUT );
	
	// Initialize ultrasound sensor.
	pinMode( PIN_ULTRASOUND_INPUT, INPUT );
	pinMode( PIN_ULTRASOUND_OUTPUT, OUTPUT );

	Console.println( "Ready." );
}

/* Loop */
void loop() {

	// Read the digital tracker sensor input.
	readTrackerSensor();
	
	// Get the distance from ultrasound sensor.
	count_getDistance = 0;
	do { 
		getDistance();
		count_getDistance++;
	} while ( distance <= 5 || distance >= 450 || count_getDistance < 10 );
	
	// Move the car.
	if ( distance > 10 ) moveCar();
	else {
		move(0);
		
		// Take 1 picture in this 100 loop.
		if ( !takePicture ) {
			takePicture = true;
			
			// If it is the debug mode.
			#ifdef DEBUG
				Console.print( "Upload picture.\n" );
			#endif
			
			uploadPicture();
		}
	}
	
	// Calculate the loop count.
	if ( ++count_loop > 100 ) {
		count_loop = 0;
		takePicture = false;
	}
	
	// If it is the debug mode.
	#ifdef DEBUG
		Console.print( "Tracker sensor = " );
		Console.print( trackerSensorIn, BIN );
		Console.print( "\tDistance = ");
		Console.print( distance );
		Console.print( "\tLast direction = " );
		Console.print( lastDirection );
		Console.print( " ;\n" );
	#endif
  
}

/* Read the tracker sensor. ( Left , Center left , Center , Center right , Right ) */
void readTrackerSensor() {
	trackerSensorIn = bitWrite( trackerSensorIn, 4, digitalRead( PIN_TRACKER_LEFT ) );
	trackerSensorIn = bitWrite( trackerSensorIn, 3, digitalRead( PIN_TRACKER_CENTERLEFT ) );
	trackerSensorIn = bitWrite( trackerSensorIn, 2, digitalRead( PIN_TRACKER_CENTER ) );
	trackerSensorIn = bitWrite( trackerSensorIn, 1, digitalRead( PIN_TRACKER_CENTERRIGHT ) );
	trackerSensorIn = bitWrite( trackerSensorIn, 0, digitalRead( PIN_TRACKER_RIGHT ) );
}

/* Get the distance from ultrasound sensor. */
void getDistance() {
	// Generate ultrasonic pulse.
	digitalWrite( PIN_ULTRASOUND_OUTPUT, LOW );
	delayMicroseconds( 2 );
	digitalWrite( PIN_ULTRASOUND_OUTPUT, HIGH );
	delayMicroseconds( 10 );
	digitalWrite( PIN_ULTRASOUND_OUTPUT, LOW );
	
	// Detect the echo , and determine the distance.
	distance = pulseIn( PIN_ULTRASOUND_INPUT, HIGH ) / 58;
}

/* Get the car position by tracker sensor. */
void moveCar() {
	// Black = 0 ; White = 1
	switch ( trackerSensorIn ) {
		// Turn right.
		case 0b01111 :
			move(1);
			break;
		case 0b00111 :
			move(2);
			break;
		case 0b10111 :
			move(3);
			break;
		case 0b10011 :
			move(4);
			break;
		// Forward.
		case 0b11011 :
			move(5);
			break;
		// Turn left.
		case 0b11001 :
			move(6);
			break;
		case 0b11101 :
			move(7);
			break;
		case 0b11100 :
			move(8);
			break;
		case 0b11110 :
			move(9);
			break;
		// If out of the black line , move the car along the last direction.
		case 0b11111 :
			move( lastDirection );
			break;
		// On the black line , turn reverse last direction.
		case 0b00000 : 
			if ( lastDirection < 5 ) lastDirection = 9;
			else lastDirection = 1;
			move( lastDirection );
			break;
		// Backward , if the car is beyond the field.
		default : 
			move( 5 );
			break;
	}
}

void uploadPicture() {
	//
	filename="";
    picture.runShellCommand("date +%s");
    while(picture.running());
    
    while(picture.available()>0){
      char c = picture.read();
      filename +=c;
    }
    filename.trim();
    filename += ".png";
    
    picture.runShellCommand("fswebcam " + path + filename + " -r 1280x720");
    while(picture.running());
    
    picture.runShellCommand("python " + path + "upload_picture.py " + path + filename);
    while(picture.running());
}



