#ifndef AUTON_H
#define AUTON_H
//This file contains functions and definitions necessary for all autonomous programs

//Stores desired motor values
DesiredMotorVals desiredMotorVals;
//Stores desired encoder values
DesiredEncVals desiredEncVals;

#define LIFT_UP 100
#define LIFT_DOWN -1

#define ENC_PER_REV 1110

void initialize(){
	clearDebugStream();
	memset(&desiredMotorVals, 0, sizeof(desiredMotorVals));
	memset(&desiredEncVals, 0, sizeof(desiredEncVals));
	servoInit();
	motorInit(&desiredEncVals);
}

long restStartTimeMs = 0;
bool rest = false;

void restMecMotors() {
	driveZeroMecMotor(&desiredMotorVals);
	motorSetActualPowerToDesired(&desiredMotorVals);
	rest = true;
	restStartTimeMs = nPgmTime;
}

#endif
