#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"
/*
TTimers centerWingPulseTimer = T1;
TTimers centerWingMovingTimer = T2;
*/
void joyAuxInit(){
	//clearTimer(centerWingPulseTimer);
	//clearTimer(centerWingMovingTimer);
}

void joyLift(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY2, BUTTON_RB)) {
		desiredMotorVals->power[Lift] = 100;
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_LB)) {
		desiredMotorVals->power[Lift] = -100;
	} else {
		desiredMotorVals->power[Lift] = 0;
	}
}

/*

bool centerWingIsMoving = false;
bool centerWingDown = false;
int centerWingStartTimeMs = 0;
int centerWingMoveTimeMs = 0; //200 to move
int centerWingStartPulseTimeMs = 0;
float centerWingPulseTimeFractionS = 0.2; //fraction of a second to pulse

void joyWing(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	//Add cooldown time so lisa doesn't accidentally put it down then raise it
	if (!centerWingIsMoving) {
		clearTimer(centerWingMovingTimer);
		if (!centerWingDown){
			if(joyButtonPressed(joyState, JOY1, BUTTON_X)){ //down
				centerWingIsMoving = true;
				centerWingMoveTimeMs = time1[centerWingMovingTimer];
				desiredMotorVals->power[Wing_Middle] = -50;
			} else {
				//Pulse the wing motor to keep it up
				int pulseTimeElapsedMs = time1[centerWingPulseTimer] - centerWingStartPulseTimeMs;
				if(pulseTimeElapsedMs >= 1000){
					clearTimer(centerWingPulseTimer);
				} else if (pulseTimeElapsedMs > 100) {
					desiredMotorVals->power[Wing_Middle] = 0;
				} else if (pulseTimeElapsedMs > 0){
					desiredMotorVals->power[Wing_Middle] = 50;
				}
			}
		}else {
			if(joyButtonPressed(joyState, JOY1, BUTTON_Y)){ //up
				centerWingIsMoving = true;
				centerWingMoveTimeMs = time1[centerWingMovingTimer];
				desiredMotorVals->power[Wing_Middle] = 50;
			}
			clearTimer(centerWingPulseTimer);
		}
	}else {
		//Wing is moving, check to see when it has moved for long enough
		centerWingMoveTimeMs = time1[centerWingMovingTimer];
		if(centerWingMoveTimeMs >= 200){
			centerWingIsMoving = false;
			desiredMotorVals->power[Wing_Middle] = 0;
			centerWingDown = !centerWingDown;
		}
	}

	//For the base wing
	if(joyButtonPressed(joyState, JOY1, BUTTON_A)){ //up
		servoSetNonCont(Wing_Base, servoDefinitions[wing_Base].minValue);
	}
	if(joyButtonPressed(joyState, JOY1, BUTTON_B)){ //down
		servoSetNonCont(Wing_Base, servoDefinitions[wing_Base].maxValue);
	}
}
*/

void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		desiredMotorVals->power[Harvester] = 95;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -95;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}
}
/*
void joyBucketDrop(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY2, BUTTON_RT)){
		servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket_Drop].maxValue);
	}else if(joyButtonPressed(joyState, JOY2, BUTTON_LT)){
		servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket_Drop].minValue);
	}
}
*/
void joyGrabber(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyGetTophat(joyState, JOY1) == TOPHAT_N) {
		servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].minValue);
	} else if (joyGetTophat(joyState, JOY2) == TOPHAT_S) {
		servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].maxValue);
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
