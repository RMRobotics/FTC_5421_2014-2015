#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"
#include "joyHarvester.h"

void joyAuxInit(DesiredEncVals *desiredEncVals){
	motorResetEncoder(desiredEncVals, Lift);
}

#define LIFT_MAX 15000
#define HIGH_GOAL 10000
#define NINETY_GOAL 7000
#define SIXTY_GOAL 5000
#define THIRTY_GOAL 3000
#define LIFT_UP_POW 100
#define ENC_SIGN sgn(LIFT_UP_POW)

void joyLift(DesiredMotorVals *desiredMotorVals, DesiredEncVals *desiredEncVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY2, BUTTON_RB)) { //lower
		desiredMotorVals->power[Lift] = LIFT_UP_POW;
		motorSetEncoder(desiredEncVals, Lift, 0);
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_LB)) { //raise
		desiredMotorVals->power[Lift] = -1 * LIFT_UP_POW;
		motorSetEncoder(desiredEncVals, Lift, ENC_SIGN * LIFT_MAX);
	} else {
		if (joyGetTophat(joyState, JOY2) == TOPHAT_N) { //high goal
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
		} else {
			desiredMotorVals->power[Lift] = 0;
		}
	}
}

void joyHarvester(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_LT)) {
		desiredMotorVals->power[Harvester] = 100;
	} else if (joyButtonPressed(joyState, JOY1, BUTTON_RT)) {
		desiredMotorVals->power[Harvester] = -100;
	} else {
		desiredMotorVals->power[Harvester] = 0;
	}
}

void joyBucketDrop(DesiredMotorVals *desiredMotorVals, TJoystick *joyState){
	if (joyButtonPressed(joyState, JOY2, BUTTON_RT)){
		servoSetNonCont(Bucket, servoDefinitions[Bucket].maxValue);
	}else if(joyButtonPressed(joyState, JOY2, BUTTON_LT)){
		servoSetNonCont(Bucket, servoDefinitions[Bucket].minValue);
	}
}

typedef enum {
	NO_BALLS,
	BIG_BALLS,
	ALL_BALLS,
} HarvestState;

typedef enum {
	NO_BALLS,
	BIG_BALLS,
	ALL_BALLS,
} HarvestPreState;

typedef enum {
	STOPPER_OPEN,
	STOPPER_CLOSE,
	WINCH_BIG_ALL,
	WINCH_BIG_START,
	WINCH_ALL_BIG,
	WINCH_ALL_START,
	WINCH_START_BIG,
	WINCH_START_ALL,
	WINCH_DECIDE,
	NOTHING,
} HarvestMovement;

long harvestTime = nPgmTime;
HarvestState harvestState = NO_BALLS; //0 - no balls, 1 - big balls, 2 - all balls
HarvestPreState harvestPreState = NO_BALLS;
HarvestMovement harvestMode = NOTHING;
long timerCapture = 0;

void joyHarvesterState(TJoystick *joyState) {
	harvestTime = nPgmTime;
	switch(harvestMode){
		case NOTHING:
			if (joyButtonPressed(joyState, JOY2, BUTTON_X)) {
					writeDebugStream("moving harvester position->Big\n");
					harvestPreState = BIG_BALLS;
			} else if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
					writeDebugStream("moving harvester position->All\n");
					harvestPreState = ALL_BALLS;
			}
			if (harvestPreState != harvestState){
				harvestMode = STOPPER_OPEN;
			}
			break;
		case STOPPER_OPEN:
			stopOpen();
			harvestMode = WINCH_DECIDE;
			break;
		case WINCH_DECIDE:
			if((harvestState == BIG_BALLS) && (harvestPreState == ALL_BALLS)){
				writeDebugStream("moving harvester BIG->All\n");
				winchDown();
				timerCapture = nPgmTime;
				harvestMode = WINCH_BIG_ALL;
			}else if((harvestState == BIG_BALLS) && (harvestPreState == NO_BALLS)){
				writeDebugStream("moving harvester BIG->START\n");
				winchUp();
				timerCapture = nPgmTime;
				harvestMode = WINCH_BIG_START;
			}else if((harvestState == ALL_BALLS) && (harvestPreState == NO_BALLS)){
				writeDebugStream("moving harvester ALL->START\n");
				winchUp();
				timerCapture = nPgmTime;
				harvestMode = WINCH_ALL_START;
			}else if((harvestState == ALL_BALLS) && (harvestPreState == BIG_BALLS)){
				writeDebugStream("moving harvester ALL->BIG\n");
				winchUp();
				timerCapture = nPgmTime;
				harvestMode = WINCH_ALL_BIG;
			}else if((harvestState == NO_BALLS) && (harvestPreState == BIG_BALLS)){
				writeDebugStream("moving harvester ALL->BIG\n");
				winchDown();
				timerCapture = nPgmTime;
				harvestMode = WINCH_START_BIG;
			}else if((harvestState == NO_BALLS) && (harvestPreState == ALL_BALLS)){
				writeDebugStream("moving harvester START->ALL\n");
				winchDown();
				timerCapture = nPgmTime;
				harvestMode = WINCH_START_ALL;
			}
			break;
		case STOPPER_CLOSE:
			stopClose();
			harvestMode = NOTHING;
			break;
		case WINCH_BIG_ALL:
			if (harvestTime - timerCapture == 125){
				clearServos();
				harvestMode = STOPPER_CLOSE;
			}
			break;
		case WINCH_BIG_START:
			if (harvestTime - timerCapture == 125){
				clearServos();
				harvestMode = STOPPER_CLOSE;
			}
			break;
		case WINCH_START_ALL:
			if (harvestTime - timerCapture == 350){
				clearServos();
				harvestMode = STOPPER_CLOSE;
			}
			break;
		case WINCH_START_BIG:
			if (harvestTime - timerCapture == 250){
				clearServos();
				harvestMode = STOPPER_CLOSE;
			}
			break;
		case WINCH_ALL_BIG:
			if (harvestTime - timerCapture == 125){
				clearServos();
				harvestMode = STOPPER_CLOSE;
			}
			break;
		case WINCH_ALL_START:
			if (harvestTime - timerCapture == 255){
				clearServos();
				harvestMode = STOPPER_CLOSE;
			}
			break;
	}
/*	if(harvestState == NO_BALLS){
		if(!winchIsMoving && !StopIsMoving){
			if (joyButtonPressed(joyState, JOY2, BUTTON_X)) {
					writeDebugStream("moving harvester start->Big\n");
					harvestPreState = BIG_BALLS;
					stopIsMoving = true;
					stopOpen();
					timerCapture = nPgmTime;
			} else if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
					writeDebugStream("moving harvester start->All\n");
					harvestPreState = ALL_BALLS;
					stopIsMoving = true;
					stopOpen();
					timerCapture = nPgmTime;
			}
		}else{
			if(stopIsMoving && stopIsDown && (harvestTime-timerCapture == 270)){
				stopIsDown = false;
				stopIsMoving = false;
				clearServos();
				winchIsMoving = true;
				winchDown();
				timerCapture = nPgmTime;
			}else if(winchIsMoving&&(harvestPreState == BIG_BALLS)&&(harvestTime-timerCapture == 270)){
				winchIsMoving = false;
				clearServos();
				stopIsMoving = true;
				stopClose();
				timerCapture = nPgmTime;
			}else if(winchIsMoving&&(harvestPreState == ALL_BALLS)&&(harvestTime-timerCapture == 330)){
				winchIsMoving = false;
				clearServos();
				stopIsMoving = true;
				stopClose();
				timerCapture = nPgmTime;
			}else if(stopIsMoving&&!stopIsDown&&(harvestTime-timerCapture == 270)){
				stopIsDown = true;
				stopIsMoving = false;
				clearServos();
				if(harvestPreState == BIG_BALLS){
					harvestState = BIG_BALLS;
				}else{
					harvestState = ALL_BALLS;
				}
			}
		}
	}else if (harvestState == BIG_BALLS){
		if(!winchIsMoving && !StopIsMoving){
			if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
					writeDebugStream("moving harvester ALL->BIG\n");
					harvestPreState = ALL_BALLS;
					stopIsMoving = true;
					stopOpen();
					timerCapture = nPgmTime;
			}
		}else{
			if(stopIsMoving && stopIsDown && (harvestTime-timerCapture == 270)){
				stopIsDown = false;
				stopIsMoving = false;
				clearServos();
				winchIsMoving = true;
				winchUp();
				timerCapture = nPgmTime;
			}else if(winchIsMoving&&(harvestTime-timerCapture == 330)){
				winchIsMoving = false;
				clearServos();
				stopIsMoving = true;
				stopClose();
				timerCapture = nPgmTime;
			}else if(stopIsMoving&&!stopIsDown&&(harvestTime-timerCapture == 270)){
				stopIsDown = true;
				stopIsMoving = false;
				clearServos();
				harvestState = BIG_BALLS;
			}
		}
	}else if (harvestState == ALL_BALLS){
		if(!winchIsMoving && !StopIsMoving){
			if (joyButtonPressed(joyState, JOY2, BUTTON_X)) {
					writeDebugStream("moving harvester BIG->ALL\n");
					harvestPreState = BIG_BALLS;
					stopIsMoving = true;
					stopOpen();
					timerCapture = nPgmTime;
			}
		}else{
			if(stopIsMoving && stopIsDown && (harvestTime-timerCapture == 270)){
				stopIsDown = false;
				stopIsMoving = false;
				clearServos();
				winchIsMoving = true;
				winchDown();
				timerCapture = nPgmTime;
			}else if(winchIsMoving&&(harvestTime-timerCapture == 330)){
				winchIsMoving = false;
				clearServos();
				stopIsMoving = true;
				stopClose();
				timerCapture = nPgmTime;
			}else if(stopIsMoving&&!stopIsDown&&(harvestTime-timerCapture == 270)){
				stopIsDown = true;
				stopIsMoving = false;
				clearServos();
				harvestState = ALL_BALLS;
			}
		}
	}*/
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
