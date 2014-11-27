#ifndef MOTOR_H
#define MOTOR_H
#pragma systemFile

#include "Helper.h"

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
#define MIN_NEVEREST_POWER 15
#define MAX_NORMAL_POWER 100
#define MIN_NORMAL_POWER 15

/*Used as reference point for max power. All external functions should use this and
  0 for minimum reference point. */
#define MAX_REFERENCE_POWER 100

/*Length (in encoder units) over which we slow down the robot in order to hit the
  target precisely. */
#define ENC_SLOW_LENGTH 3000
#define ENC_SLOW_RATE MAX_REFERENCE_POWER / ENC_SLOW_LENGTH

//Slew rate for scaling values (amount to add per loop). Percentage.
#define MOTOR_SLEW_RATE 5

#define MAX_NUM_MOTORS ((int)kNumbOfTotalMotors)
#define NUM_MOTORS 4

//Array for storing all motor enums that we use (this way we can loop through)
tMotor motorList[NUM_MOTORS];

//Struct for storing motor power limits for each motor
typedef struct MotorData {
	int maxPower;
	int minPower;
} MotorData;

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

//Array for storing MotorData for each motor
static MotorData motorDefinitions[MAX_NUM_MOTORS];

//Struct definition for desired motor powers
typedef struct DesiredMotorVals {
	int power[MAX_NUM_MOTORS];
} DesiredMotorVals;

//Struct definition for desired encoder limit
typedef struct DesiredEncVals {
	int encoder[MAX_NUM_MOTORS];
} DesiredEncVals;

//Returns max reference power
int motorGetMaxReferencePower() {
	return MAX_REFERENCE_POWER;
}

//Flag for seeing if motor definitions have been initialized
bool motorDefsInitialized = false;

//Initialize motor definitions
void motorInit() {
	//VOLATILE
	//list all motors
	motorList[0] = MecMotor_FL;
	motorList[1] = MecMotor_BL;
	motorList[2] = MecMotor_FR;
	motorList[3] = MecMotor_BR;

	//init maxPower to MAX_NORMAL_POWER and minPower to MIN_NORMAL_POWER
	for (int i=0;i<MAX_NUM_MOTORS;i++) {
		motorDefinitions[i].maxPower = MAX_NORMAL_POWER;
		motorDefinitions[i].minPower = MIN_NORMAL_POWER;
	}

	//set drive max motor powers
	motorDefinitions[MecMotor_FL].maxPower = MAX_NEVEREST_POWER;
	motorDefinitions[MecMotor_BL].maxPower = MAX_NEVEREST_POWER;
	motorDefinitions[MecMotor_FR].maxPower = MAX_NEVEREST_POWER;
	motorDefinitions[MecMotor_BR].maxPower = MAX_NEVEREST_POWER;

	//set drive min motor powers
	motorDefinitions[MecMotor_FL].minPower = MIN_NEVEREST_POWER;
	motorDefinitions[MecMotor_BL].minPower = MIN_NEVEREST_POWER;
	motorDefinitions[MecMotor_FR].minPower = MIN_NEVEREST_POWER;
	motorDefinitions[MecMotor_BR].minPower = MIN_NEVEREST_POWER;

	//check to make sure motors listed fills up motorList length
	int numOfMotors = sizeof(motorList) / sizeof(tMotor);
	if (numOfMotors != NUM_MOTORS) {
		writeDebugStream("Motors failed to initialize (check NUM_MOTORS and motorList[] to see if they agree!\n");
	} else {
		motorDefsInitialized = true;
	}
}

/*Private function, returns power value after scaling power
  to fit motor constraints. Expects values from 0 to
  motorGetMaxReferencePower().
  If definitions have not been initialized, debug and assume
  normal motor. */
static int motorScalePower(tMotor currentMotor, int power) {
	if (motorDefsInitialized) {
		if (power != 0) { //Scaling assumes non zero power
			int maxPower;
			int minPower;
			maxPower = motorDefinitions[currentMotor].maxPower;
			minPower = motorDefinitions[currentMotor].minPower;

			//Scale power
			float scaledPower = (((float)power / (float)motorGetMaxReferencePower()) *
													(float)(maxPower-minPower)) + (sgn(power) * minPower);
		}
		return (int) power;
	} else {
		writeDebugStream("Motors not initialized!\n");
		return 0;
	}
}

/*Bounds power to fit motor constraints.
	Does NOT scale power */
static int motorBoundPower(tMotor currentMotor, int power) {
	if (motorDefsInitialized) {
		int boundPower = power;
		int minPower = motorDefinitions[currentMotor].minPower;
		int maxPower = motorDefinitions[currentMotor].maxPower;
		if (abs(power) < minPower) { //in deadband
			boundPower = 0;
		} else if (abs(power) > maxPower) {
			boundPower = sgn(boundPower) * maxPower;
		}
		return boundPower;
	} else {
		writeDebugStream("Motors not initialized!\n");
		return 0;
	}
}

/*Handles limits on desired power given a target encoder distance.
	- Sets desired powers to 0 if	the motor encoder exceeds desired
		encoder values.
	- Limits desired powers when the encoder approaches the target.
	- If the desired encoder value is zero, it is ignored. */
void motorLimitDesiredPowerToEncoder(DesiredMotorVals *desiredVals,
DesiredEncVals *desiredEncVals) {
	if (motorDefsInitialized) {
		for (int i=0; i<NUM_MOTORS; i++) {
			tMotor curMotor = motorList[i];
			int desiredEnc = desiredEncVals->encoder[curMotor];
			if (desiredEnc > 0) {
				if (nMotorEncoder[curMotor] > desiredEnc) {
					desiredVals->power[curMotor] = 0;
				} else if ((desiredEnc - nMotorEncoder[curMotor]) < ENC_SLOW_LENGTH) {
					desiredVals->power[curMotor] = ENC_SLOW_RATE * (desiredEnc-nMotorEncoder[curMotor]);
				}
			} else if (desiredEnc < 0) {
				if (nMotorEncoder[curMotor] < desiredEnc) {
					desiredVals->power[curMotor] = 0;
				} else if ((desiredEnc - nMotorEncoder[curMotor]) < -ENC_SLOW_LENGTH) {
					desiredVals->power[curMotor] = ENC_SLOW_RATE * (desiredEnc-nMotorEncoder[curMotor]);
				}
			}
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

//Update actual motor values with desired motor values
void motorSetActualPowerToDesired(DesiredMotorVals *desiredVals) {
	if (motorDefsInitialized) {
		for(int i=0; i<NUM_MOTORS; i++) {
			//Scale powers to motor range
			int scaled = motorScalePower(motorList[i], desiredVals->power[motorList[i]]);
			int scaledSlew = motorScalePower(motorList[i], MOTOR_SLEW_RATE);

			//Find differences between motor and desired
			int diff = scaled - motor[motorList[i]];

			//Determine whether to change by slew rate, or by jumping directly to desired speed (smaller is better)
		  //Make sure to keep the sign of the change
			int rate = sgn(diff) * (int)(helpFindMinAbsFloat(diff,scaledSlew));

			//update motors
			motor[motorList[i]] = motor[motorList[i]] + rate;
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

#endif
