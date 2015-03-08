#pragma config(UserModel, "Headers/Pragmas.h")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Headers\Joystick.h"
#include "Headers\Motor.h"
#include "Headers\Servo.h"
#include "Headers\Global.h"
#include "Headers\Drive.h"

//Stores desired motor values
DesiredMotorVals desiredMotorVals;
//Stores desired encoder values
DesiredEncVals desiredEncVals;

void initialize() {
	servoInit();
	motorInit(&desiredEncVals);
	//Initialize to zeroes
	memset(&desiredMotorVals, 0, sizeof(desiredMotorVals));
	memset(&desiredEncVals, 0, sizeof(desiredEncVals));
	clearDebugStream();
}

task main()
{
	initialize();
	writeDebugStream("This is slides forward\n");
	joyWaitForStart();
	wait1Msec(5000);
	time1[T1] = 0; //in ms
	driveSetMecMotorN(&desiredMotorVals, 1.0);
	while (time1[T1] < 2500) {
		motorSetActualPowerToDesired(&desiredMotorVals);
		writeDebugStream("Driving forward!\n");
	}
	driveZeroMecMotor(&desiredMotorVals);
	while (time1[T1] < 10000) {
		motorSetActualPowerToDesired(&desiredMotorVals);
		writeDebugStream("Stopping!\n");
	}
	//lower harvester
	while (time1[T1] < 3000) {
		servoSetCont(HarvesterWinch, 0);
	}
}