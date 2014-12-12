
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"


void joyChuteMotors(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	bool chuteVacuum = joyButtonPressed(joyState, JOY1, BUTTON_RT);
	bool chuteEject = joyButtonPressed(joyState, JOY1, BUTTON_LT);
	if (chuteVacuum == true){
		desiredMotorVals->power[Chute_Left] = 50;
		desiredMotorVals->power[Chute_Right] = 50;
	}else if (chuteEject == true){
		desiredMotorVals->power[Chute_Left] = -50;
		desiredMotorVals->power[Chute_Right] = -50;
	}else {
		desiredMotorVals->power[Chute_Left] = 0;
		desiredMotorVals->power[Chute_Right] = 0;
	}
}

void joySlideMotors(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	bool slidesUp = joyButtonPressed(joyState, JOY2, BUTTON_RT);
	bool slidesDown = joyButtonPressed(joyState, JOY2, BUTTON_LT);
	if (slidesUp == true){
		desiredMotorVals->power[Slides_Left] = 50;
		desiredMotorVals->power[Slides_Right] = 50;
	}else if (slidesDown == true){
		desiredMotorVals->power[Slides_Left] = -50;
		desiredMotorVals->power[Slides_Right] = -50;
	}else {
		desiredMotorVals->power[Slides_Left] = 0;
		desiredMotorVals->power[Slides_Right] = 0;
	}
}
