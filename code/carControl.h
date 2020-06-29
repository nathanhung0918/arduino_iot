#ifndef _CARCONTROL_H_
#define _CARCONTROL_H_


#include <Servo.h>

// Servo object.
Servo servoRight, servoLeft;

// The last direction of movement.
int lastDirection;

// The direction of movement. ( anticlock-wise + clock-wise -)
const int moveDirection[11][2] = {	{ 1500, 1500 }, 													// Stop. ( 0 )
									{ 1510, 1470 }, { 1515, 1470 }, { 1520, 1470 }, { 1525, 1470 },		// Turn left. ( 1, 2, 3, 4 )
									{ 1530, 1470 }, 													// Forward. ( 5 )
									{ 1530, 1475 }, { 1530, 1480 }, { 1530, 1485 }, { 1530, 1490 },		// Turn right. ( 5, 6, 7, 8 )
									{ 1480, 1520 }														// Backward. ( 10 )
								  };

/* Move the car along the "direction". */
void move( const int direction ) {
	if (direction != 0 ) lastDirection = direction;
	servoLeft.writeMicroseconds( moveDirection[ direction ][0] );
	servoRight.writeMicroseconds( moveDirection[ direction ][1] );
}



#endif















































