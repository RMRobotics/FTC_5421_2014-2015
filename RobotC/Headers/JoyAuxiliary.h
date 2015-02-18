#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"

void joyAuxInit(DesiredEncVals *desiredEncVals){
	motorResetEncoder(desiredEncVals, Lift);
}

/*
#define LIFT_MAX 15000
#define HIGH_GOAL 10000
#define NINETY_GOAL 7000
#define SIXTY_GOAL 5000
#define THIRTY_GOAL 3000
*/
#define LIFT_UP_POW 100
//#define ENC_SIGN sgn(LIFT_UP_POW)


void joyLift(DesiredMotorVals *desiredMotorVals, DesiredEncVals *desiredEncVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY2, BUTTON_RB)) { //lower
		desiredMotorVals->power[Lift] = LIFT_UP_POW;
		//motorSetEncoder(desiredEncVals, Lift, 0);
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_LB)) { //raise
		desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
		//motorSetEncoder(desiredEncVals, Lift, ENC_SIGN * LIFT_MAX);
	} else {
		/*if (joyGetTophat(joyState, JOY2) == TOPHAT_N) { //high goal
			if (motorGetEncoder(Lift) > HIGH_GOAL) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			motorSetEncoder(desiredEncVals, Lift, ENC_SIGN * HIGH_GOAL);
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_E || joyGetTophat(joyState, JOY2) == TOPHAT_W) { //med goal
			if (motorGetEncoder(Lift) > SIXTY_GOAL) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			motorSetEncoder(desiredEncVals, Lift, ENC_SIGN * SIXTY_GOAL);
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_S) { //low goal
			if (motorGetEncoder(Lift) > THIRTY_GOAL) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			motorSetEncoder(desiredEncVals, Lift, ENC_SIGN * THIRTY_GOAL);
		} else {*/
			desiredMotorVals->power[Lift] = 0;
		//}
	}
}

void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		desiredMotorVals->power[Harvester] = 65;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -65;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}

	if (joyButtonPressed(joyState, JOY2, Button_X)) {
		desiredMotorVals->power[HarvesterMove] = 50;
	}else if (joyButtonPressed(joyState, JOY2, Button_y)) {
		desiredMotorVals->power[HarvesterMove] = -50;
	} else {
		desiredMotorVals->power[HarvesterMove] = 0;
	}
}

void joyBucketDrop(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY2, BUTTON_LT)){ //open
		servoSetNonCont(Bucket, 235); //max value is for init only
	}else if(joyButtonPressed(joyState, JOY2, BUTTON_RT)){
		servoSetNonCont(Bucket, servoDefinitions[Bucket].minValue);
	}
}

void joyGrabber(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_B)) {
		writeDebugStream("moving servo up!\n");
		servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].minValue);
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_A)) {
		writeDebugStream("moving servo down\n");
		servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].maxValue);
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
