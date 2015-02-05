#ifndef JOYAUXILIARY_H
#define JOYAUXILIARY_H
#pragma systemFile

#include "Global.h"
#include "Joystick.h"
#include "Drive.h"

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

void joyGrabber(DesiredMotorVals *desiredMotorVals, TJoystick *joyState) {
	if (joyButtonPressed(joyState, JOY2, BUTTON_A)) {
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
