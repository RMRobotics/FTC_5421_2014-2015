#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"
#include "joyHarvester.h"

void joyAuxInit(){
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
		servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket].maxValue);
	}else if(joyButtonPressed(joyState, JOY2, BUTTON_LT)){
		servoSetNonCont(Bucket_Drop, servoDefinitions[Bucket].minValue);
	}
}
*/

typedef enum {
	NO_BALLS,
	BIG_BALLS,
	ALL_BALLS,
} HarvestState;

long harvestTime = nPgmTime;
bool stopIsMoving = false;
bool winchIsMoving = false;
bool stopIsDown = true; //Stop up or down
HarvestState harvestState = NO_BALLS; //0 - no balls, 1 - big balls, 2 - all balls
int harvestPreState = 0;
int timerCapture = 0;

void joyHarvesterState(TJoystick *joyState) {
	harvestTime = nPgmTime;
	if(harvestState == NO_BALLS){{
		if(!winchIsMoving && !StopIsMoving)
			if (joyButtonPressed(joyState, JOY2, BUTTON_X)) {
					writeDebugStream("moving harvester start->Big\n");
					harvestPreState = 1;
					stopIsMoving = true;
					stopOpen();
					timerCapture = nPgmTime;
			} else if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
					writeDebugStream("moving harvester start->All\n");
					harvestPreState = 2;
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
				winchOpen();
				timerCapture = nPgmTime;
			}else if(winchIsMoving&&(harvestPreState = 1)&&(harvestTime-timerCapture == 270)){
				winchIsMoving = false;
				clearServos();
				stopIsMoving = true;
				stopClose();
				timerCapture = nPgmTime;
			}else if(winchIsMoving&&(harvestPreState = 2)&&(harvestTime-timerCapture == 330)){
				winchIsMoving = false;
				clearServos();
				stopIsMoving = true;
				stopClose();
				timerCapture = nPgmTime;
			}else if(stopIsMoving&&!stopIsDown&&(harvestTime-timerCapture == 270)){
				stopIsDown = true;
				stopIsMoving = false;
				clearServos();
			}
		}
	}else if (harvestState == BIG_BALLS){
		if (joyButtonPressed(joyState, JOY2, BUTTON_Y)) {
				writeDebugStream("moving harvester Big->All\n");
				timerCapture = nPgmTime;
		}
	}else if (harvestState == ALL_BALLS){
		if (joyButtonPressed(joyState, JOY2, BUTTON_X)) {
				writeDebugStream("moving harvester All->Big\n");
				timerCapture = nPgmTime;
		}
	}
}

void joyGrabber(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY1, BUTTON_A)) {
		writeDebugStream("moving servo up!\n");
		servoSetNonCont(TubeGrabber, 145);
	} else if (joyButtonPressed(joyState, JOY2, BUTTON_B)) {
		writeDebugStream("moving servo down\n");
		servoSetNonCont(TubeGrabber, 175);
	}
}

void joyAuxDebug(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
}
#endif
