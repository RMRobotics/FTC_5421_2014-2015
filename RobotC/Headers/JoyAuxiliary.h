#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"

void joyAuxInit(DesiredEncVals *desiredEncVals){
	motorResetEncoder(desiredEncVals, Lift);
}


#define LIFT_UP_POW -100
#define ENC_SIGN sgn(LIFT_UP_POW)

#define LIFT_MAX ENC_SIGN * 9800 //high goal
#define NINETY_GOAL ENC_SIGN * 7400
#define SIXTY_GOAL ENC_SIGN * 4600
#define THIRTY_GOAL ENC_SIGN * 2000
#define LIFT_MIN 0

void joyLift(DesiredMotorVals *desiredMotorVals, DesiredEncVals *desiredEncVals, TJoystick *joyState){
	long encTarget = ENC_OFF;
	if (joyButtonPressed(joyState, JOY2, BUTTON_RB)) { //raise
		desiredMotorVals->power[Lift] = LIFT_UP_POW;
		encTarget = LIFT_MAX;
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_LB)) { //lower
		desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
		encTarget = LIFT_MIN;
	} else {
		if (joyGetTophat(joyState, JOY2) == TOPHAT_N) { //high goal
			if (abs(motorGetEncoder(Lift)) > abs(NINETY_GOAL)) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			encTarget = NINETY_GOAL;
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_E || joyGetTophat(joyState, JOY2) == TOPHAT_W) { //med goal
			if (abs(motorGetEncoder(Lift)) > abs(SIXTY_GOAL)) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			encTarget = SIXTY_GOAL;
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_S) { //low goal
			if (abs(motorGetEncoder(Lift)) > abs(THIRTY_GOAL)) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			encTarget = THIRTY_GOAL;
		} else {
			desiredMotorVals->power[Lift] = 0;
			encTarget = ENC_OFF;
		}
	}
	motorSetEncoder(desiredEncVals, Lift, encTarget);
}

//#define MAX_HARVEST_TIME_MS_POS 2000
//#define MIN_HARVEST_TIME_MS_POS -2000
//int timePosMs = 0;
//long lastUpdateTimeMs = 0;

void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		desiredMotorVals->power[Harvester] = 65;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -65;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}

	if (joyButtonPressed(joyState, JOY2, BUTTON_X)) {
		//if (timePosMs < MAX_HARVEST_TIME_MS_POS) {
			desiredMotorVals->power[HarvesterMove] = 50;
			//timePosMs += nPgmTime - lastUpdateTimeMs;
		//}
	}else if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
		//if (timeposMs > MIN_HARVEST_TIME_MS_POS) {
			desiredMotorVals->power[HarvesterMove] = -50;
			//timePosMs -= nPgmTime - lastUpdateTimeMs;
		//}
	} else {
		desiredMotorVals->power[HarvesterMove] = 0;
	}
	//lastUpdateTimeMs = nPgmTime;
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
