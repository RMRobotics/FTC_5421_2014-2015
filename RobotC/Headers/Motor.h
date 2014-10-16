#ifndef MOTOR_H
#define MOTOR_H
#pragma SystemFile

//Pragmas
#include "Pragmas.h"

//Number of motors
#define NUM_MOTORS 4

//Constants to keep PID working and counter stalling regardless of battery level
#define MAX_PID_POWER 75
#define MIN_STALL_POWER 15

void motorSetSafePower(tMotor currentMotor, int power) {
	if (power > MAX_PID_POWER) {
		power = MAX_PID_POWER;
	} else if (power < -MAX_PID_POWER) {
		power = -MAX_PID_POWER;
	} else if (power > -MIN_STALL_POWER && power < MIN_STALL_POWER) { //in deadband
		power = 0;
	}
	power = (power > MAX_PID_POWER) ? MAX_PID_POWER : power;
	power = (power < -MAX_PID_POWER) ? -MAX_PID_POWER : power;
	motor[currentMotor] = power;
}

void motorSetPower (tMotor currentMotor, int power) {
	motor[currentMotor] = power;
}

#endif
