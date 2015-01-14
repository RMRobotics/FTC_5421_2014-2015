#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"

TTimers joyGlobalTimer = T1;

void joyLift(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY1, BUTTON_Y)) {
		desiredMotorVals->power[Lift] = 100;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RB)) {
		desiredMotorVals->power[Lift] = -100;
	} else {
		desiredMotorVals->power[Lift] = 0;
	}
}


bool centerWingIsMoving = false;
bool centerWingDown = false;
int centerWingStartTimeMs = 0;
int centerWingPulseTimeMs = 200;

bool baseWingDown = false;
int baseWingStartTimeMs = 0;
int baseWingPulseTimeMs = 500;

void joyWing(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	//Add cooldown time so lisa doesn't accidentally put it down then raise it
	if (time1[joyGlobalTimer] - centerWingStartTimeMs > centerWingPulseTimeMs) {
		if (joyButtonPressed(joyState, JOY1, BUTTON_X)) {
			centerWingStartTimeMs = time1[joyGlobalTimer];
			if (centerWingDown) {
				desiredMotorVals->power[Wing_Middle] = 75;
			} else {
				desiredMotorVals->power[Wing_Middle] = -75;
			}
			centerWingDown = !centerWingDown;
		} else {
			desiredMotorVals->power[Wing_Middle] = 0;
		}
	}
	//Add cooldown time to avoid servo jerking back and forth
	if ((time1[joyGlobalTimer] - baseWingStartTimeMs) > baseWingPulseTimeMs) {
		if (joyButtonPressed(joyState, JOY1, BUTTON_A)) {
			baseWingStartTimeMs = time1[joyGlobalTimer];
			if (baseWingDown) {
				servoSetNonCont(Wing_Base, servoDefinitions[Wing_Base].maxValue);
			} else {
				servoSetNonCont(Wing_Base, servoDefinitions[Wing_Base].minValue);
			}
			baseWingDown = !baseWingDown;
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
