#ifndef MOTOR_H
#define MOTOR_H
#pragma systemFile

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

#define MAX_NUM_MOTORS (int)kNumbOfTotalMotors

//Arrays for storing power limits for each motor
static int maxMotorPower[MAX_NUM_MOTORS];
static int minMotorPower[MAX_NUM_MOTORS];

//Struct definition for desired motor powers/encoder lengths
typedef struct DesiredMotorVals {
	int power[MAX_NUM_MOTORS];
	int encoder[MAX_NUM_MOTORS];
} DesiredMotorVals;

//Enum for referencing mecanum motors
//They're named after TMNT because Lisa.
//TODO place in config file. this is forced to be here because pragmas can't be included...
typedef enum MecMotor {
	MecMotor_FL = Donatello_FL,
	MecMotor_BL = Leonardo_BL,
	MecMotor_FR = Michelangelo_FR,
	MecMotor_BR = Raphael_BR,
} MecMotor;

//Initialize motor definitions
void motorInit() {
	//VOLATILE
	//init maxMotorPower to 100
	for (int i=0;i<(sizeof(maxMotorPower)/sizeof(int));i++) {
		maxMotorPower[i] = 100;
	}
	//set drive max motor powers
	maxMotorPower[(tMotor)MecMotor_FL] = MAX_NEVEREST_POWER;
	maxMotorPower[(tMotor)MecMotor_BL] = MAX_NEVEREST_POWER;
	maxMotorPower[(tMotor)MecMotor_FR] = MAX_NEVEREST_POWER;
	maxMotorPower[(tMotor)MecMotor_BR] = MAX_NEVEREST_POWER;

	//init minMotorPower to MIN_STALL_POWER
	for (int i=0;i<(sizeof(minMotorPower)/sizeof(int));i++) {
		minMotorPower[i] = MIN_STALL_POWER;
	}
}

//Private function, returns power value after applying power bounds
static int motorBoundPower(tMotor currentMotor, int power) {
	int maxPower = maxMotorPower[(int)currentMotor];
	int minPower = minMotorPower[(int)currentMotor];
	if (power > maxPower) {
		power = maxPower;
	} else if (power < -maxPower) {
		power = -maxPower;
	} else if (power > -minPower && power < minPower) { //in deadband
		power = 0;
	}
	return power;
}

//Returns maximum power for drive motors
int motorGetMaxDrivePower() {
	//Pick one of the four drive motors
	return maxMotorPower[MecMotor_FL];
}

//Update actual motor values with desired motor values
void motorSetActualPowerToDesired(DesiredMotorVals *desiredVals) {
	//Kludgey code, but it works
  //VOLATILE
	motor[MecMotor_FL] = motorBoundPower((tMotor)MecMotor_FL, desiredVals->power[MecMotor_BR]);
	motor[MecMotor_FR] = motorBoundPower((tMotor)MecMotor_FR, desiredVals->power[MecMotor_BR]);
	motor[MecMotor_BL] = motorBoundPower((tMotor)MecMotor_BL, desiredVals->power[MecMotor_BR]);
	motor[MecMotor_BR] = motorBoundPower((tMotor)MecMotor_BR, desiredVals->power[MecMotor_BR]);

}

#endif
