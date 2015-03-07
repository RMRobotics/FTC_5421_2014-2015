#pragma config(UserModel, "Headers/Pragmas.h")

#include "Headers\Global.h"
#include "Headers\Drive.h"
#include "Headers\Joystick.h"
#include "Headers\Motor.h"
#include "Headers\Servo.h"

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

int encTarget = 3200;
tMotor testEncoder = Lift;
tMotor testMotor = Lift;
task main()
{
	initialize();
	writeDebugStream("This is lift encoder test\n");
	joyWaitForStart();
	motorResetEncoder(&desiredEncVals, testEncoder);
	motorSetEncoder(&desiredEncVals, testEncoder, encTarget);
	desiredMotorVals.power[testMotor] = 50;
	writeDebugStream("Testing!\n");
	while (!motorAllHitEncoderTarget(&desiredEncVals)) {
		motorSetActualPowerToDesired(&desiredMotorVals);
		motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
		writeDebugStream("Pow: %d Enc: %d\n", motor[testMotor], nMotorEncoder[testEncoder]);
	}
	writeDebugStream("Stopped!\n");
}
