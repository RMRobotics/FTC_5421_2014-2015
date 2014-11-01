#ifndef JOYMECANUMDRIVE_H
#define JOYMECANUMDRIVE_H
#pragma systemFile

#include "Joystick.h"
#include "Motor.h"

//This function interprets a TJoystick struct and sets desired motor powers.
//This uses mecanum drive in arcade style.
//  Left joystick controls strafing and forwards/backwards
//  Right joystick (x-axis only) controls rotation
void joymecdriveSetDesiredPowers(TJoystick *joyState) {
	int joy1y1 = joyState->joy1_y1;
	int joy1x2 = joyState->joy1_x2;
	int joy1x1 = joyState->joy1_x1;
	/*
	motorSetDesiredSafePower(MecMotorFL, joy1y1 + joy1x2 + joy1x1);
	motorSetDesiredSafePower(MecMotorBL, joy1y1 + joy1x2 - joy1x1);
	motorSetDesiredSafePower(MecMotorBR, joy1y1 - joy1x2 + joy1x1);
	motorSetDesiredSafePower(MecMotorFR, joy1y1 - joy1x2 - joy1x1);
	*/
}

#endif
