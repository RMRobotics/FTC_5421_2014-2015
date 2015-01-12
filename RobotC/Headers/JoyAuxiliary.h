#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"


void joyLift(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY1, BUTTON_A)) {
		desiredMotorVals->power[Lift] = 50;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_B)) {
		desiredMotorVals->power[Lift] = -50;
	} else {
		desiredMotorVals->power[Lift] = 0;
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	writeDebugStream("Left Slide Motor: %d\n", motor[Slides_Left]);
	writeDebugStream("Right Slide Motor: %d\n", motor[Slides_Right]);
	writeDebugStream("Left Slide Motor Desired: %d\n", desiredMotorVals->power[Slides_Left]);
	writeDebugStream("Right Slide Motor Desired: %d\n", desiredMotorVals->power[Slides_Right]);
}
#endif
