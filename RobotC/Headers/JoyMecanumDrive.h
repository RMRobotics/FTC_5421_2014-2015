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
	motorSetDesiredSafePower(Motor_Mec_FL, joyState->joy1_y1 + joyState->joy1_x2 + joyState->joy1_x1);
	motorSetDesiredSafePower(Motor_Mec_BL, joyState->joy1_y1 + joyState->joy1_x2 - joyState->joy1_x1);
	motorSetDesiredSafePower(Motor_Mec_BR, joyState->joy1_y1 - joyState->joy1_x2 + joyState->joy1_x1);
	motorSetDesiredSafePower(Motor_Mec_FR, joyState->joy1_y1 - joyState->joy1_x2 - joyState->joy1_x2);
}

#endif
