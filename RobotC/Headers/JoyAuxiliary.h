#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"

void joyAuxInit(DesiredEncVals *desiredEncVals){
	motorResetEncoder(desiredEncVals, Lift);
}


#define LIFT_MAX 90000
#define HIGH_GOAL LIFT_MAX
#define NINETY_GOAL 30000
#define SIXTY_GOAL 15000
#define THIRTY_GOAL 3000
#define LIFT_MIN 0

#define LIFT_UP_POW 100
#define ENC_SIGN sgn(LIFT_UP_POW)
#define ENCTARGET_OFF 0
long encTarget = ENCTARGET_OFF;
int encOverflowCount = 0;

void joyLift(DesiredMotorVals *desiredMotorVals, DesiredEncVals *desiredEncVals, TJoystick *joyState){
	long currentEnc = 0;
	//Check for overflow, update currentEnc
	if (abs(motorGetEncoder(Lift)) > MAX_ENC_TARGET) {
		int remainder = motorGetEncoder(Lift) - MAX_ENC_TARGET;
		encOverflowCount = encOverflowCount + sgn(motorGetEncoder(Lift));
		motorResetEncoder(desiredEncVals, Lift);
		currentEnc = remainder + (encOverflowCount * MAX_ENC_TARGET);
	} else {
		currentEnc = motorGetEncoder(Lift) + (encOverflowCount * MAX_ENC_TARGET);
	}

	if (joyButtonPressed(joyState, JOY2, BUTTON_RB)) { //lower
		desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
		encTarget = LIFT_MIN;
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_LB)) { //raise
		desiredMotorVals->power[Lift] = LIFT_UP_POW;
		encTarget = LIFT_MAX;
	} else {
		if (joyGetTophat(joyState, JOY2) == TOPHAT_N) { //high goal
			if (motorGetEncoder(Lift) > NINETY_GOAL) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			encTarget = NINETY_GOAL;
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_E || joyGetTophat(joyState, JOY2) == TOPHAT_W) { //med goal
			if (motorGetEncoder(Lift) > SIXTY_GOAL) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			encTarget = SIXTY_GOAL;
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_S) { //low goal
			if (motorGetEncoder(Lift) > THIRTY_GOAL) {
				desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
			} else {
				desiredMotorVals->power[Lift] = LIFT_UP_POW;
			}
			encTarget = THIRTY_GOAL;
		} else {
			desiredMotorVals->power[Lift] = 0;
			encTarget = ENCTARGET_OFF;
		}
	}
	if (encTarget != ENCTARGET_OFF) {
		//If near goal, set target else don't target
		if ((abs(currentEnc) > (encTarget - MAX_ENC_TARGET)) && (abs(currentEnc) <= encTarget)) {
			motorResetEncoder(desiredEncVals, Lift);
			motorSetEncoder(desiredEncVals, Lift, ENC_SIGN * (encTarget % MAX_ENC_TARGET));
		} else if ((abs(currentEnc) > encTarget) && (abs(currentEnc) < (encTarget + MAX_ENC_TARGET))) {
			motorResetEncoder(desiredEncVals, Lift);
			motorSetEncoder(desiredEncVals, Lift, -ENC_SIGN * (abs(currentEnc) - encTarget));
		} else {
			desiredEncVals->encoder[Lift] = ENC_OFF;
		}
	}
}

//#define MAX_HARVEST_TIME_MS_POS 2000
//#define MIN_HARVEST_TIME_MS_POS -2000
//int timePosMs = 0;
//long lastUpdateTimeMs = 0;

void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		desiredMotorVals->power[Harvester] = 100;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -100;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}

	if (joyButtonPressed(joyState, JOY2, Button_X)) {
		//if (timePosMs < MAX_HARVEST_TIME_MS_POS) {
			desiredMotorVals->power[HarvesterMove] = 50;
			//timePosMs += nPgmTime - lastUpdateTimeMs;
		//}
	}else if (joyButtonPressed(joyState, JOY2, Button_y)) {
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
		servoSetNonCont(Bucket, servoDefinitions[Bucket].maxValue);
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
