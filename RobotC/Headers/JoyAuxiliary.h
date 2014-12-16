
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
	bool tubeRelease = joyButtonPressed(joyState, JOY1, BUTTON_LB);
	bool tubeGrab = joyButtonPressed(joyState, JOY1, BUTTON_RB);
	if (tubeRelease){
		servoSetNonCont(TubeGrabber, 0);
	}else if (tubeGrab){
		servoSetNonCont(TubeGrabber, 120);
	}
}

void joyKickStand(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	bool kickStandGrab = joyButtonPressed(joyState, JOY2, BUTTON_RB);
	bool kickStandRelease = joyButtonPressed(joyState, JOY2, BUTTON_LB);
	if (kickStandGrab){
		servoSetNonCont(KickStand, 145);
	}else if (kickStandRelease){
		servoSetNonCont(KickStand, 25);
	}
}

void joyBucket(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	bool dump = joyButtonPressed(joyState, JOY2, BUTTON_A);
	bool fill = joyButtonPressed(joyState, JOY2, BUTTON_B);
	if (dump) {
		servoSetNonCont(Bucket_Left, 100, 5);
		servoSetNonCont(Bucket_Right, 155, 5);
} else if (fill) {
		servoSetNonCont(Bucket_Left, 240, 5);
		servoSetNonCont(Bucket_Right, 15, 5);
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	writeDebugStream("Left Slide Motor: %d\n", motor[Slides_Left]);
	writeDebugStream("Right Slide Motor: %d\n", motor[Slides_Right]);
	writeDebugStream("Left Slide Motor Desired: %d\n", desiredMotorVals->power[Slides_Left]);
	writeDebugStream("Right Slide Motor Desired: %d\n", desiredMotorVals->power[Slides_Right]);
}
