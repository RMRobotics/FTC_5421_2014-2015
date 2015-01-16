#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"

void joyLift(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY1, BUTTON_Y)) {
		desiredMotorVals->power[Lift] = 100;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RB)) {
		desiredMotorVals->power[Lift] = -100;
	} else {
		desiredMotorVals->power[Lift] = 0;
	}
}

TTimers centerWingPulse = T1;
TTimers centerWingMoving = T2;
bool centerWingIsMoving = false;
bool centerWingDown = false;
int centerWingStartTimeMs = 0;
int centerWingMoveTimeMs = 0; //200 to move
int centerWingStartPulseTimeMs = 0;
float centerWingPulseTimeFractionS = 0.2; //fraction of a second to pulse

bool baseWingDown = false;

void joyWing(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	//Add cooldown time so lisa doesn't accidentally put it down then raise it
	if(!centerWingIsMoving){
		if (!centerWingDown){
			if(joyButtonPressed(joyState, JOY1, BUTTON_B)){
				centerWingIsMoving = true;
				centerWingMoveTimeMs = time1[centerWingMoving];
				desiredMotorVals->power[Wing_Middle] = -75;
			}else if((time1[centerWingPulse]-centerWingStartPulseTimeMs) == 100){
				centerWingStartPulseTimeMs = time1[centerWingPulse];
				desiredMotorVals->power[Wing_Middle] = 75;
			}
		}else if (centerWingDown){
			if(joyButtonPressed(joyState, JOY1, BUTTON_A)){
				centerWingIsMoving = true;
				centerWingMoveTimeMs = time1[centerWingMoving];
				desiredMotorVals->power[Wing_Middle] = 75;
			}
		}
	}else if (centerWingIsMoving){
		if(centerWingMoveTimeMs >= 200){
			if(centerWingDown){
				centerWingIsMoving = false;
				centerWingDown = false;
				desiredMotorVals->power[Wing_Middle] = 0;
			}else if (!centerWingDown){
				centerWingIsMoving = false;
				centerWingDown = true;
				desiredMotorVals->power[Wing_Middle] = 0;
			}
		}
	}
//For the base wing
	if(joyButtonPressed(joyState, JOY1, BUTTON_Y) && (!baseWingDown)){
		servoSetNonCont(Wing_Base, servoDefinitions[wing_Base].minValue;
	}
	if(joyButtonPressed(joyState, JOY1, BUTTON_X) && (baseWingDown)){
		servoSetNonCont(Wing_Base, servoDefinitions[wing_Base].maxValue;
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
//int bucketStartTimeMs = 0;
int bucketMoveTimeMs = 1000;
void joyBucketDrop(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (time1[T2] > bucketMoveTimeMs) {
		if (joyButtonPressed(joyState, JOY1, BUTTON_B)){
			ClearTimer(T2);
			if (bucketDown) {
				servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket_Drop].maxValue);
			} else {
				servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket_Drop].minValue);
			}
			bucketDown = !bucketDown;
		}
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
