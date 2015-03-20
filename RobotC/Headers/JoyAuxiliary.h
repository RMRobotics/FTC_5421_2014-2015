#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"


void joyLift(DesiredMotorVals *desiredMotorVals, DesiredEncVals *desiredEncVals, TJoystick *joyState){
	long encTarget = ENC_OFF;
	short encFlags = 0;
	bool encCapModeOn = false; //whether or not to use encoder target as hard stop

	if (joyButtonPressed(joyState, JOY2, BUTTON_LB)) { //raise
		desiredMotorVals->power[Lift] = 100;
		encTarget = LIFT_MIN + LIFT_MAX;
		encFlags = ENCFLAGS_CAPMODE_ON ^ ENCFLAGS_CAPMAX_ON;
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_RB)) { //lower
		desiredMotorVals->power[Lift] = (-1);
		encTarget = LIFT_MIN;
		encFlags = ENCFLAGS_CAPMODE_ON;
	} else {
		if (joyGetTophat(joyState, JOY2) == TOPHAT_N) { //high goal
			if (abs(motorGetEncoder(Lift)) > abs(NINETY_GOAL)) {
				desiredMotorVals->power[Lift] = -1;
				encFlags = ENCFLAGS_CAPMODE_ON;
			} else {
				desiredMotorVals->power[Lift] = 100;
				encFlags = ENCFLAGS_CAPMODE_ON ^ ENCFLAGS_CAPMAX_ON;
			}
			encTarget = LIFT_MIN + NINETY_GOAL;
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_E || joyGetTophat(joyState, JOY2) == TOPHAT_W) { //med goal
			if (abs(motorGetEncoder(Lift)) > abs(SIXTY_GOAL)) {
				desiredMotorVals->power[Lift] = -1;
				encFlags = ENCFLAGS_CAPMODE_ON;
			} else {
				desiredMotorVals->power[Lift] = 100;
				encFlags = ENCFLAGS_CAPMODE_ON ^ ENCFLAGS_CAPMAX_ON;
			}
			encTarget = LIFT_MIN + SIXTY_GOAL;
		} else if (joyGetTophat(joyState, JOY2) == TOPHAT_S) { //low goal
			if (abs(motorGetEncoder(Lift)) > abs(THIRTY_GOAL)) {
				desiredMotorVals->power[Lift] = -1;
				encFlags = ENCFLAGS_CAPMODE_ON;
			} else {
				desiredMotorVals->power[Lift] = 100;
				encFlags = ENCFLAGS_CAPMODE_ON ^ ENCFLAGS_CAPMAX_ON;
			}
			encTarget = LIFT_MIN + THIRTY_GOAL;
		} else {
			desiredMotorVals->power[Lift] = 0;
			encTarget = ENC_OFF;
		}
	}

	if (joyButtonPressed(joyState, JOY2, BUTTON_BACK)) { //reset lift min, turn off targeting
		LIFT_MIN = motorGetEncoder(Lift);
		encTarget = ENC_OFF;
	}

	motorSetEncoder(desiredEncVals, Lift, encTarget, encFlags);
}

#define MAX_HARVESTMOVE_TIME_POS 800
#define MIN_HARVESTMOVE_TIME_POS 0
/*
long harvestTimer = nPgmTime;
long lastHarvestTime = 0;
long harvestRunTime = 0;
*/


void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	/*harvestTimer = nPgmTime;
	harvestRunTime = harvestTimer - lastHarvestTime;
	writeDebugStreamLine("%d %d %d",harvestTimer,harvestRunTime,lastHarvestTime);*/
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		/*if(harvestRunTime < 500){
			desiredMotorVals->power[Harvester] = 90;
		}else if(harvestRunTime>1000){
			lastHarvestTime = harvestTimer;
			desiredMotorVals->power[Harvester] = 0;
		}*/
		desiredMotorVals->power[Harvester] = 90;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		/*if(harvestRunTime < 500){
			desiredMotorVals->power[Harvester] = 90;
		}else if(harvestRunTime>1000){
			lastHarvestTime = harvestTimer;
			desiredMotorVals->power[Harvester] = 0;
		}*/
		desiredMotorVals->power[Harvester] = -90;
	} else {
		//lastHarvestTime = harvestTimer;
		desiredMotorVals->power[Harvester] = 0;
	}

	if (joyButtonPressed(joyState, JOY2, BUTTON_START)) {
		motorResetTimePosMs(HarvesterMove);
	}

	if (joyButtonPressed(joyState, JOY2, BUTTON_X)) { //down
		if ((motorGetTimePosMs(HarvesterMove) < MAX_HARVESTMOVE_TIME_POS) || joyButtonPressed(joyState, JOY2, BUTTON_START)) {
			desiredMotorVals->power[HarvesterMove] = 20;
		} else {
			desiredMotorVals->power[HarvesterMove] = 0;
		}
	}else if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
		if ((motorGetTimePosMs(HarvesterMove) >= MIN_HARVESTMOVE_TIME_POS) || joyButtonPressed(joyState, JOY2, BUTTON_START)) {
			desiredMotorVals->power[HarvesterMove] = -20;
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
		servoSetNonCont(TubeGrabber, (servoDefinitions[TubeGrabber].maxValue*(.66)));
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
