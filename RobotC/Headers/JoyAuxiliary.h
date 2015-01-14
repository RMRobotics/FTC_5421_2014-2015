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
int centerWingMoveTimeMs = 200;
int centerWingStartPulseTimeMs = 0;
float centerWingPulseTimeFractionS = 0.2; //fraction of a second to pulse

bool baseWingDown = false;
int baseWingStartTimeMs = 0;
int baseWingMoveTimeMs = 500;

void joyWing(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	//Add cooldown time so lisa doesn't accidentally put it down then raise it
	if (time1[joyGlobalTimer] - centerWingStartTimeMs > centerWingMoveTimeMs) { //if not moving
		if (joyButtonPressed(joyState, JOY1, BUTTON_X)) {
			centerWingStartTimeMs = time1[joyGlobalTimer];
			if (centerWingDown) {
				desiredMotorVals->power[Wing_Middle] = 100;
			} else {
				desiredMotorVals->power[Wing_Middle] = -100;
			}
			centerWingDown = !centerWingDown;
		} else {/*
			if (centerWingDown) {
				desiredMotorVals->power[Wing_Middle] = 0;
			} else {
				//arm is up so pulse it to keep it up
				if (time1[joyGlobalTimer] - centerWingPulseStartTimeMs > 1000*(1.0 - centerWingPulseTimeFractionS)) {
					centerWingPulseStartTimeMs = time1[joyGlobalTimer];
					desiredMotorVals->power[Wing_Middle] = 75;
				} else if (time1[joyGlobalTimer] - centerWingPulseStartTimeMs < 1000*centerWingPulseTimeFractionS) {
					desiredMotorVals->power[Wing_Middle] = 75;
				} else {
					desiredMotorVals->power[Wing_Middle] = 0;
				}
			}*/
			desiredMotorVals->power[Wing_Middle] = 0;
		}
	}
	//Add cooldown time to avoid servo jerking back and forth
	if ((time1[joyGlobalTimer] - baseWingStartTimeMs) > baseWingMoveTimeMs) {
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
		desiredMotorVals->power[Harvester] = 95;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -95;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}
}

bool bucketDown = false;
int bucketStartTimeMs = 0;
int bucketMoveTimeMs = 500;
void joyBucketDrop(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if ((time1[joyGlobalTimer] - bucketStartTimeMs) > bucketMoveTimeMs) {
		if (joyButtonPressed(joyState, JOY1, BUTTON_START)){
			bucketStartTimeMs = time1[joyGlobalTimer];
			if (bucketDown) {
				servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket_Drop].maxValue);
			} else {
				servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket_Drop].minValue);
			}
		}
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
