#ifndef JOYMECANUMDRIVE_H
#define JOYMECANUMDRIVE_H
#pragma systemFile

#include "Joystick.h"
#include "Motor.h"
#include "Drive.h"

//Joystick constants
#define DEADBAND 10

//This function interprets a TJoystick struct and sets desired motor powers.
//This uses mecanum drive in arcade style.
//  Left joystick controls strafing and forwards/backwards
//  Right joystick (x-axis only) controls rotation
void joymecdriveSetDesiredPower(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	//////////////
	//DRIVE CODE//
	//////////////
	int joy1y1 = joyState->joy1_y1;
	int joy1x2 = joyState->joy1_x2;
	int joy1x1 = joyState->joy1_x1;

	//Due to short/int sizes they allow these to be -128 to 127, so chop off -128
	if (joy1y1 == -128) {
		joy1y1 = -127;
	}
	if (joy1x1 == -128) {
		joy1x1 = -127;
	}
	if (joy1x2 == -128) {
		joy1x2 = -127;
	}

	int angle = 0;
	float speed = 0.0;
	float rotation = 0.0;

	//Deadband check
	if (abs(joy1y1) >= DEADBAND || abs(joy1x1) >= DEADBAND || abs(joy1x2) >= DEADBAND) {
		//We use atan2 because we want the actual angle, not the reference angle
		angle = (int) radiansToDegrees((atan2(joy1y1,joy1x1)));
		//Atan2 returns angle from positive x-axis, up to +180 or -180
		//We want to transform this into an angle from positive y-axis from 0 to 360
		if (angle >= 0 && angle <= 90) {
			//First quadrant, find angle to y-axis
    	angle = 90 - angle;
    } else if (angle > 90 && angle <= 180) {
    	//Second quadrant, find angle to y-axis and subtract from 360: 360-(angle-90) = 450 - angle
    	angle = 450 - angle;
    } else if (angle < 0 && angle >= -90) {
    	//Fourth quadrant, add 90 to abs(angle)
    	angle = abs(angle) + 90;
    } else {
    	//Third quadrant, add 90 to abs(angle)
   		angle = abs(angle) + 90;
    }
		speed = sqrt((long)(pow(joy1x1,2) + pow(joy1y1,2)));
		rotation = ((float) joy1x2) / 127.0;
		driveSetMecMotorPolarDegrees(desiredMotorVals, angle, speed, rotation);
	} else {
	  driveZeroMecMotor(desiredMotorVals);
	}

	/////////
	//Other//
	/////////


}

#endif
