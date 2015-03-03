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

#define LIFT_MAX ENC_SIGN * (LIFT_MIN + 10500) //high goal
#define NINETY_GOAL ENC_SIGN * (LIFT_MIN + 7800)
#define SIXTY_GOAL ENC_SIGN * (LIFT_MIN + 4600)
#define THIRTY_GOAL ENC_SIGN * (LIFT_MIN + 2000)
long LIFT_MIN = 0;

void joyLift(DesiredMotorVals *desiredMotorVals, DesiredEncVals *desiredEncVals, TJoystick *joyState){
	long encTarget = ENC_OFF;
	bool encCapModeOn = false; //whether or not to use encoder target as hard stop

	if (joyButtonPressed(joyState, JOY2, BUTTON_LB)) { //raise
		desiredMotorVals->power[Lift] = LIFT_UP_POW;
		encTarget = LIFT_MAX;
		encCapModeOn = true;
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_RB)) { //lower
		desiredMotorVals->power[Lift] = (-1 * 0.01 * LIFT_UP_POW);
		encCapModeOn = true;
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

	if (joyButtonPressed(joyState, JOY2, BUTTON_BACK)) { //reset lift min, turn off targeting
		LIFT_MIN = motorGetEncoder(Lift);
		encTarget = ENC_OFF;
	}
	motorSetEncoder(desiredEncVals, Lift, encTarget, encCapModeOn);
}

#define MAX_HARVESTMOVE_TIME_POS 800
#define MIN_HARVESTMOVE_TIME_POS 0

void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		desiredMotorVals->power[Harvester] = 100;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -100;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}

	if (joyButtonPressed(joyState, JOY2, BUTTON_START)) {
		motorResetTimePosMs(HarvesterMove);
	}

	if (joyButtonPressed(joyState, JOY2, BUTTON_X)) { //down
		if ((motorGetTimePosMs(HarvesterMove) < MAX_HARVESTMOVE_TIME_POS) || joyButtonPressed(joyState, JOY2, BUTTON_START)) {
			desiredMotorVals->power[HarvesterMove] = 50;
		} else {
			desiredMotorVals->power[HarvesterMove] = 0;
		}
	}else if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
		if ((motorGetTimePosMs(HarvesterMove) >= MIN_HARVESTMOVE_TIME_POS) || joyButtonPressed(joyState, JOY2, BUTTON_START)) {
			desiredMotorVals->power[HarvesterMove] = -50;
		} else {
			desiredMotorVals->power[HarvesterMove] = 0;
		}
	} else {
		desiredMotorVals->power[HarvesterMove] = 0;
	}
}

void joyBucketDrop(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY2, BUTTON_RT)){ //open
		servoSetNonCont(Bucket, servoDefinitions[Bucket].maxValue);
	}else if(joyButtonPressed(joyState, JOY2, BUTTON_LT)){
		servoSetNonCont(Bucket, servoDefinitions[Bucket].minValue);
	}
}

void joyGrabber(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_A)) {
		servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].minValue);
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_B)) {
		servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].maxValue);
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_X)) {
		servoSetNonCont(TubeGrabber, (servoDefinitions[TubeGrabber].maxValue*(.75)));
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
