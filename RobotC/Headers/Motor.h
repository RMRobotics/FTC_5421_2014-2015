#ifndef MOTOR_H
#define MOTOR_H
#pragma SystemFile

////////////////////////////////////////////////////////////////////////////////////////
/*A note on array indexing:
  Arrays can be accessed by converting the tMotor enum to an int to get the array
  index. Here's the rationale for determining array size.
  Although FTC caps the number of motors to 8 12v DC motors + 3 NXT motors,
  RobotC caps it to kNumbOfTotalMotors. We have no idea how RobotC generates the tMotor
  enum, so assume the worst and make the size of the array kNumbOfTotalMotors.
*/
////////////////////////////////////////////////////////////////////////////////////////

//Constants to keep PID working and counter stalling regardless of battery level
#define MAX_NEVEREST_POWER 78
#define MIN_STALL_POWER 15

#define MAX_NUM_MOTORS (int)kNumbOfRealMotors

//Arrays for storing power limits for each motor
static int maxMotorPower[MAX_NUM_MOTORS];
static int minMotorPower[MAX_NUM_MOTORS];

//Array for storing desired motor powers.
static int desiredMotorPower[MAX_NUM_MOTORS];

//Enum for referencing mecanum motors
//They're named after TMNT because Lisa.
//TODO place in config file. this is forced to be here because pragmas can't be included...
typedef enum MecMotors {
	MecMotor_FL = Donatello_FL,
	MecMotor_BL = Leonardo_BL,
	MecMotor_FR = Michelangelo_FR,
	MecMotor_BR = Raphael_BR,
} MecMotors;

//Initialize motor definitions and state
void initMotors() {
	//memset used here to initialize default values
	memset(maxMotorPower, 100, sizeof(maxMotorPower));
	memset(minMotorPower, MIN_STALL_POWER, sizeof(minMotorPower));
	memset(desiredMotorPower, 0, sizeof(desiredMotorPower));
}

//Private function, used to set desired power for motors
static void motorSetDesiredPower (tMotor currentMotor, int power) {
	desiredMotorPower[(int)currentMotor] = power;
}

//Used to set desired power, checked by max/min power limits
void motorSetDesiredSafePower(tMotor currentMotor, int power) {
	int maxPower = maxMotorPower[(int)currentMotor];
	int minPower = minMotorPower[(int)currentMotor];
	if (power > maxPower) {
		power = maxPower;
	} else if (power < -maxPower) {
		power = -maxPower;
	} else if (power > -minPower && power < minPower) { //in deadband
		power = 0;
	}
	motorSetDesiredPower(currentMotor, power);
}

//Update actual motor values with desired motor values
void motorSetActualPowerToDesired() {
	for (int i=0; i<MAX_NUM_MOTORS; i++) {
		motor[i] = desiredMotorPower[i];
	}
}

#endif
