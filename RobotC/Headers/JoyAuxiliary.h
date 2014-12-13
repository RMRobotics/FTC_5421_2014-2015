
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"


void joyChuteMotors(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	bool chuteVacuum = joyButtonPressed(joyState, JOY1, BUTTON_RT);
	bool chuteEject = joyButtonPressed(joyState, JOY1, BUTTON_LT);
	if (chuteVacuum){
		desiredMotorVals->power[Chute_Left] = 50;
		desiredMotorVals->power[Chute_Right] = 50;
	}else if (chuteEject){
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
	if (slidesUp){
		desiredMotorVals->power[Slides_Left] = 50;
		desiredMotorVals->power[Slides_Right] = 50;
	}else if (slidesDown){
		desiredMotorVals->power[Slides_Left] = -50;
		desiredMotorVals->power[Slides_Right] = -50;
	}else {
		desiredMotorVals->power[Slides_Left] = 0;
		desiredMotorVals->power[Slides_Right] = 0;
	}
}

void joyTubeGrabber(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	bool tubeRelease = joyButtonPressed(joyState, JOY2, BUTTON_RB);
	bool tubeGrab = joyButtonPressed(joyState, JOY2, BUTTON_LB);
	if (tubeRelease){
		servoSetNonCont(TubeGrabber, 0);
	}else if (tubeGrab){
		servoSetNonCont(TubeGrabber, 100);
	}
}

void joyKickStand(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	bool kickStandGrab = joyButtonPressed(joyState, JOY1, BUTTON_RB);
	bool kickStandRelease = joyButtonPressed(joyState, JOY1, BUTTON_LB);
	if (kickStandGrab){
		servoSetNonCont(KickStand, 145);
	}else if (kickStandRelease){
		servoSetNonCont(KickStand, 3);
	}
}
