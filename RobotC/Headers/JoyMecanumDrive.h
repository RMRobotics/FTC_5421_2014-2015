#ifndef JOYMECANUMDRIVE_H
#define JOYMECANUMDRIVE_H
#pragma systemFile

#include "Joystick.h"
#include "Motor.h"
#include "Drive.h"

//Joystick constants
#define DEADBAND 10

/*This function interprets a TJoystick struct and sets desired motor powers.
  This uses mecanum drive in arcade style.
  Left joystick controls strafing and forwards/backwards
  Right joystick (x-axis only) controls rotation */
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
	JoyTophat tophat = joyGetTophat(joyState);
	switch (tophat) {
		case TOPHAT_N:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 0, 1.0, 0.0);
			break;
		case TOPHAT_NE:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 45, 1.0, 0.0);
			break;
		case TOPHAT_E:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 90, 1.0, 0.0);
			break;
		case TOPHAT_SE:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 135, 1.0, 0.0);
			break;
		case TOPHAT_S:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 180, 1.0, 0.0);
			break;
		case TOPHAT_SW:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 225, 1.0, 0.0);
			break;
		case TOPHAT_W:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 270, 1.0, 0.0);
			break;
		case TOPHAT_NW:
			driveSetMecMotorPolarDegrees(desiredMotorVals, 315, 1.0, 0.0);
			break;
		default:
			//Give up to joystick
			//Deadband check
			if (abs(joy1y1) >= DEADBAND || abs(joy1x1) >= DEADBAND || abs(joy1x2) >= DEADBAND) {
				//We use atan2 because we want the actual angle, not the reference angle
				angle = (int) radiansToDegrees((atan2(joy1x1,joy1y1)));
				speed = sqrt((long)(pow(joy1x1,2) + pow(joy1y1,2))) / 127.0;
				rotation = ((float) joy1x2) / 127.0;
				speed = constrain(speed,-1.0,1.0);
				rotation = constrain(rotation,-1.0,1.0);
				driveSetMecMotorPolarDegrees(desiredMotorVals, angle, speed, rotation);
			} else {
				driveZeroMecMotor(desiredMotorVals);
			}
			break;
	}//switch tophat

	nxtDisplayString(3, "R:%f %f %f %f",  ((float)joy1x2), rotation);
	nxtDisplayString(5, "P: %d %d %d", (int)(angle*100),(int)(speed*100),(int)(rotation*100));

	/////////
	//Other//
	/////////


}

#endif
