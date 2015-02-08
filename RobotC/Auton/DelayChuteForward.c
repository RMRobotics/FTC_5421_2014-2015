#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Hubs,  S3, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  mtr_S3_C1_1,     Michelangelo_FR, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S3_C1_2,     Donatello_FL,  tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S3_C2_1,     Raphael_BR,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S3_C2_2,     Leonardo_BL,   tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S3_C3_1,     motor0,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C3_2,     Lift,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C4_1,     motor1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C4_2,     Harvester,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,          tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    HarvesterWinch,       tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_4,    HarvesterStop,        tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    TubeGrabber,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    Bucket,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// The sensor is connected to the first port
// of the SMUX which is connected to the NXT port S1.
// To access that sensor, we must use msensor_S1_1.  If the sensor
// were connected to 3rd port of the SMUX connected to the NXT port S4,
// we would use msensor_S4_3

#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-gyro.h"

// Give the sensor a nice easy to use name
const tMUXSensor GYRO = msensor_S4_1;


#include "..\Headers\Joystick.h"
#include "..\Headers\Motor.h"
#include "..\Headers\Servo.h"
#include "..\Headers\Global.h"
#include "..\Headers\Drive.h"

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
	writeDebugStream("This is chute forward\n");
	joyWaitForStart();
	wait1Msec(5000);
	time1[T1] = 0; //in ms
	driveSetMecMotorS(&desiredMotorVals, 1.0);
	while (time1[T1] < 2500) {
		motorSetActualPowerToDesired(&desiredMotorVals);
		writeDebugStream("Driving forward!\n");
	}
	driveZeroMecMotor(&desiredMotorVals);
	while (time1[T1] < 2500) {
		motorSetActualPowerToDesired(&desiredMotorVals);
		writeDebugStream("Stopping!\n");
	}

	//lower harvester
	while (time1[T1] < 3000) {
		servoSetCont(HarvesterWinch, 0);
	}
}
