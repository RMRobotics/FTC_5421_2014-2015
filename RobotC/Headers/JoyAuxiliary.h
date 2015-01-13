#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"


void joyLift(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY1, BUTTON_Y)) {
		desiredMotorVals->power[Lift] = 50;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RB)) {
		desiredMotorVals->power[Lift] = -50;
	} else {
		desiredMotorVals->power[Lift] = 0;
	}
}


bool centerWingIsMoving = false;
bool centerWingDown = false;
bool baseWingDown = false;
int centerWingPulseTimeMs = 200;

void joyWing(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (centerWingIsMoving) {
		if (time1[T1] > centerWingPulseTimeMs) {
			desiredMotorVals->power[Wing_Middle] = 0;
			centerWingIsMoving = false;
			centerWingDown = !centerWingDown;
		}
	} else {
		if (joyButtonPressed(joyState, JOY1, BUTTON_X)) {
			ClearTimer(T1);
			centerWingIsMoving = true;
			if (centerWingDown) {
				desiredMotorVals->power[Wing_Middle] = 75;
			} else {
				desiredMotorVals->power[Wing_Middle] = -75;
			}
		}
	}
	if (joyButtonPressed(joyState, JOY1, BUTTON_A)) {
		if (baseWingDown) {
			servoSetNonCont(Wing_Base, servoDefinitions[Wing_Base].maxValue);
			baseWingDown = false;
		} else {
			servoSetNonCont(Wing_Base, servoDefinitions[Wing_Base].minValue);
			baseWingDown = true;
		}
	}
}

void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		desiredMotorVals->power[Harvester] = 50;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -50;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
