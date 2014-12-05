
#pragma systemFile

#include "Joystick.h"
#include "Motor.h"
#include "Drive.h"



void joyChuteMotors(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joystick.joy1_Buttons == 8){
		motor[chuteLeft] = 60;
		motor[chuteRight] = 60;
	}
}
