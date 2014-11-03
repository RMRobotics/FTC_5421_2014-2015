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
#define MIN_NEVEREST_POWER 10
#define MAX_NORMAL_POWER 100
#define MIN_NORMAL_POWER 15

/*Used as reference point for max power. All external functions should use this and
  0 for minimum reference point. */
#define MAX_REFERENCE_POWER 100

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
//VOLATILE
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
		minMotorPower[i] = MIN_NORMAL_POWER;
	}

	//set drive min motor powers
	minMotorPower[(tMotor)MecMotor_FL] = MIN_NEVEREST_POWER;
	minMotorPower[(tMotor)MecMotor_BL] = MIN_NEVEREST_POWER;
	minMotorPower[(tMotor)MecMotor_FR] = MIN_NEVEREST_POWER;
	minMotorPower[(tMotor)MecMotor_BR] = MIN_NEVEREST_POWER;
}

/*Private function, returns power value after scaling power
  to fit motor constraints */
static int motorScalePower(tMotor currentMotor, int power) {
	int maxPower = maxMotorPower[(int)currentMotor];
	int minPower = minMotorPower[(int)currentMotor];

	//Scale power
	float scaledPower = (float) power * ((float) maxPower / (float) MAX_REFERENCE_POWER);

	//Bound power
	if (scaledPower > -minPower && scaledPower < minPower) { //in deadband
		scaledPower = 0;
	}

	return (int) power;
}

//Returns max reference power
int motorGetMaxReferencePower() {
	return MAX_REFERENCE_POWER;
}

//Update actual motor values with desired motor values
void motorSetActualPowerToDesired(DesiredMotorVals *desiredVals) {
	//Kludgey code, but it works
	//VOLATILE
	motor[MecMotor_FL] = motorScalePower((tMotor)MecMotor_FL, desiredVals->power[MecMotor_FL]);
	motor[MecMotor_FR] = motorScalePower((tMotor)MecMotor_FR, desiredVals->power[MecMotor_FR]);
	motor[MecMotor_BL] = motorScalePower((tMotor)MecMotor_BL, desiredVals->power[MecMotor_BL]);
	motor[MecMotor_BR] = motorScalePower((tMotor)MecMotor_BR, desiredVals->power[MecMotor_BR]);

}

#endif
