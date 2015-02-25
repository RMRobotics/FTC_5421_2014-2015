#ifndef MOTOR_H
#define MOTOR_H
#pragma systemFile

#include "Helper.h"
#include "Global.h"

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

/*Threshold difference from previous encoder at which the encoder value is considered bad */
#define ENC_ERROR_THRESHOLD 3000

/*Encoder target value which we ignore (use this when you want to ignore encoder checks) */
#define ENC_OFF -(MAX_ENC_TARGET + 1)

/*Maximum encoder target possible. This is technically the maximum number an long
  variable can hold, but we'll give it some room so that we don't introduce a
  race condition between the overflow and the encoder check. */
long MAX_ENC_TARGET = 2000000000; //must be a long b/c constants are treated as ints

//Slew rate for scaling values (amount to add per loop). Percentage.
#define MOTOR_SLEW_RATE 100

//Struct for storing motor data for each motor
typedef struct MotorData {
	int maxPower;
	int minPower;
	long encSlowLength; //Max length (in encoder units) over which we slow down the robot in order to hit the target precisely.
	long encSlowStep; //Encoder step by which we decrement motor power by 1
	long encHitZone; //Offset by which we define the encoder to have hit the target
} MotorData;

//Array for storing MotorData for each motor
static MotorData motorDefinitions[MAX_NUM_MOTORS];

//Struct for storing motor status for each motor
//This is used to handle encoder overflows automatically so we can use values up to
//MAX_LONG for encoder targets.
//This also handles position in time.
typedef struct MotorState {
	long encoder;
	long lastRealEncoderPos; //stores last real encoder pos
	long timePosMs; //stores position in "time" (50 means net movement forward for 50 ms)
	long lastUpdateTimeMs; //last time this motorstate as updated
} MotorState;

//Array for storing MotorState for eachmotor
static MotorState motorStates[MAX_NUM_MOTORS];

//Flag for seeing if motor definitions have been initialized
bool motorDefsInitialized = false;


#define NUM_MOTORS 7
//Array for storing all motor enums that we use (this way we can loop through)
tMotor motorList[NUM_MOTORS];

/*Returns max reference power for other functions */
int motorGetMaxReferencePower() {
	return MAX_REFERENCE_POWER;
}

/* Gets motor name given tMotor */
static void motorGetName(tMotor curMotor, string *motorName) {
	if (curMotor == MecMotor_FL) {
		*motorName = "MecMotor_FL";
	} else if (curMotor == MecMotor_BL) {
		*motorName = "MecMotor_BL";
	} else if (curMotor == MecMotor_FR) {
		*motorName = "MecMotor_FR";
	} else if (curMotor == MecMotor_BR) {
		*motorName = "MecMotor_BR";
	} else if (curMotor == Lift) {
		*motorName = "Lift";
	} else if (curMotor == Harvester) {
		*motorName = "Harvester";
	} else if (curMotor == HarvesterMove) {
		*motorName = "HarvesterMove";
	}
}

/*Zero all motors */
void motorZeroAllMotors(DesiredMotorVals *desiredMotorVals) {
	for (int i=0; i<NUM_MOTORS; i++) {
		desiredMotorVals->power[motorList[i]] = 0;
	}
}

/*Private function, returns power value after scaling power
  to fit motor constraints. Expects values from 0 to
  motorGetMaxReferencePower().
  If definitions have not been initialized, debug and assume
  normal motor. */
static int motorScalePower(tMotor currentMotor, int power) {
	int outputPower = power;
	if (motorDefsInitialized) {
		if (power != 0) { //Scaling assumes non zero power
			int maxPower;
			int minPower;
			maxPower = motorDefinitions[currentMotor].maxPower;
			minPower = motorDefinitions[currentMotor].minPower;
			//Scale power
			float scaledPower = (((float)power / (float)motorGetMaxReferencePower()) *
													(float)(maxPower-minPower)) + (sgn(power) * minPower);

			outputPower = (int) scaledPower;

			/*
			writeDebugStream("Scale pow: %f\n", scaledPower);
			writeDebugStream("Output pow: %d\n", outputPower);
			*/
		}
		return outputPower;
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

/* Resets desired, real, and the MotorState encoder, for a specific tMotor */
void motorResetEncoder(DesiredEncVals *desiredEncVals, tMotor curMotor) {
	if (motorDefsInitialized) {
		desiredEncVals->encoder[curMotor] = ENC_OFF;
		motorStates[curMotor].encoder = 0;
		nMotorEncoder[curMotor] = 0;
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

/* Resets all encoders, desired and real.
	 Use this function when you want reset the encoders to
	 a fresh state, NOT to turn off targeting. */
void motorResetAllEncoders(DesiredEncVals *desiredEncVals) {
	if (motorDefsInitialized) {
		for (int i=0;i<NUM_MOTORS;i++) {
			motorResetEncoder(desiredEncVals, motorList[i]);
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

/* Returns MotorState time value */
long motorGetTimePosMs(tMotor curMotor) {
	return motorStates[curMotor].timePosMs;
}

/* Resets MotorState time value */
void motorResetTimePosMs(tMotor curMotor) {
	motorStates[curMotor].timePosMs = 0;
}

/* Returns MotorState encoder value */
long motorGetEncoder(tMotor curMotor) {
	return motorStates[curMotor].encoder;
}

/* Sets encoder target for a specific tMotor. Checks
	 to make sure the target is below the maximum target,
	 and that the target is not ENC_OFF.
	 Does NOT reset the encoder. */
void motorSetEncoder(DesiredEncVals *desiredEncVals, tMotor curMotor, int target) {
	if (motorDefsInitialized) {
		if (abs(target) > MAX_ENC_TARGET) {
			writeDebugStream("Encoder target value (%d) past maximum target value!\n", target);
		} else {
			desiredEncVals->encoder[curMotor] = target;
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

/*Checks if a specific tMotor has hit its encoder targets.
  - If there is no target set, returns true.
  - "Hit" targets is defined as being anywhere in ENC_HIT_ZONE
    distance from the target. */
bool motorHasHitEncoderTarget(DesiredEncVals *desiredEncVals, tMotor curMotor) {
	long desiredEnc = desiredEncVals->encoder[curMotor];
	if (desiredEnc == ENC_OFF) {
		return true;
	} else {
		long curEnc = motorGetEncoder(curMotor);
		if (desiredEncVals->encoderCapEnabled[curMotor]) {
			if (desiredEncVals->encoderCapIsMax[curMotor]) {
				if (curEnc > desiredEnc) {
					writeDebugStream("Lift hitting cap!\n");
					return true;
				} else {
					return false;
				}
			} else {
				if (curEnc < desiredEnc) {
					writeDebugStream("Lift hitting cap!\n");
					return true;
				} else {
					return false;
				}
			}
		} else {
			long encHitZone = motorDefinitions[curMotor].encHitZone;
			if ((desiredEnc < (curEnc + encHitZone)) && (desiredEnc > (curEnc - encHitZone))) {
				return true;
			} else {
				if ((sgn(motor[curMotor]) != sgn(desiredEnc - curEnc)) && motor[curMotor] != 0) {
					writeDebugStream("Motor not running in same direction as encoder target!\n");
				}
				return false;
			}
		}
	}
}

/*Checks if all tMotors have hit their encoder targets by
  calling motorHasHitEncoderTarget() on all motors. */
bool motorAllHitEncoderTarget(DesiredEncVals *desiredEncVals) {
	if (motorDefsInitialized) {
		for (int i=0;i<NUM_MOTORS;i++) {
			if (!motorHasHitEncoderTarget(desiredEncVals, motorList[i])) {
				return false;
			}
		}
		return true;
	} else {
		writeDebugStream("Motors not initialized!\n");
		return false;
	}
}


/*Handles limits on desired power given a target encoder distance.
	- Sets desired powers to 0 if	the motor encoder exceeds desired
		encoder values.
	- Limits desired powers when the encoder approaches the target.
	- If the desired encoder value is ENC_OFF, it is ignored.
	- If the signs of the desired encoder value and the actual
		encoder value do not agree, then no limitation is applied. */
void motorLimitDesiredPowerToEncoder(DesiredMotorVals *desiredMotorVals,
DesiredEncVals *desiredEncVals) {
	if (motorDefsInitialized) {
		for (int i=0; i<NUM_MOTORS; i++) {
			tMotor curMotor = motorList[i];
			long desiredEnc = desiredEncVals->encoder[curMotor];

			if (desiredEnc != ENC_OFF && (desiredMotorVals->power[curMotor] != 0)) {
				long curEnc = motorGetEncoder(curMotor);

				if (motorHasHitEncoderTarget(desiredEncVals, curMotor)) {
					desiredMotorVals->power[curMotor] = 0;
				} else if (abs(desiredEnc - curEnc) < motorDefinitions[curMotor].encSlowLength) {
					//Calculate number of steps left to decrement motor
					long steps = (desiredEnc - curEnc) / motorDefinitions[curMotor].encSlowStep;
					//Apply only if below current desired speed
					if (abs(steps) < abs(desiredMotorVals->power[curMotor])) {
						desiredMotorVals->power[curMotor] = steps;
						//Add one to make sure motor never shuts off
						if (desiredMotorVals->power[curMotor] == 0) {
							desiredMotorVals->power[curMotor] = (sgn(desiredEnc - curEnc) > 0)?1:-1;
						}
					}
				}
			}
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

/*Update motor states.
	This function attempts a check for bad encoder
	values but will abort the check if the function takes
	longer than the specified time. */
void motorUpdateState() {
	for (int i=0; i<NUM_MOTORS; i++) {
		tMotor curMotor = motorList[i];
		//Update motor time position
		if (motor[curMotor] > 0) {
			long deltaTime = (nPgmTime - motorStates[curMotor].lastUpdateTimeMs);
			motorStates[curMotor].timePosMs = motorStates[curMotor].timePosMs + deltaTime;
		} else if (motor[curMotor] < 0) {
			long deltaTime = (nPgmTime - motorStates[curMotor].lastUpdateTimeMs);
			motorStates[curMotor].timePosMs = motorStates[curMotor].timePosMs - deltaTime;
		}

		motorStates[curMotor].lastUpdateTimeMs = nPgmTime;

		//Update motor encoders
		//Check for sporadic encoder values as documented by Cougar Robotics #623
		int checkEnc = nMotorEncoder[curMotor];
		int knownGoodEnc = motorStates[curMotor].lastRealEncoderPos;
		int curEnc;

		if (abs(checkEnc - knownGoodEnc) > ENC_ERROR_THRESHOLD) {
			string motorName;
			motorGetName(curMotor, &motorName);
			writeDebugStream("Bad encoder val! Compare: %d with (bad) %d for motor: %s\n", knownGoodEnc, checkEnc, motorName);
			//do nothing because enc val is bad
		} else {
			curEnc = checkEnc; //enc val is good
			motorStates[curMotor].encoder += (curEnc - motorStates[curMotor].lastRealEncoderPos);
			motorStates[curMotor].lastRealEncoderPos = curEnc;

			if (abs(curEnc) > 30000) { //reset real encoder when it overflows
				motorStates[curMotor].lastRealEncoderPos = 0;
				nMotorEncoder[curMotor] = 0;
			}
		}
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

			/*
			if (motor[motorList[i]] == MecMotor_FL) {
				writeDebugStream("Initial: %d\n", motor[motorList[i]]);
			}*/

			//update motor
			motor[motorList[i]] = motor[motorList[i]] + rate;

			/*
			if (motorList[i] == MecMotor_FL) {
				writeDebugStream("Slew: %d\n", scaledSlew);
				writeDebugStream("Diff: %d\n", diff);
				writeDebugStream("Rate: %d\n", rate);
				writeDebugStream("Final: %d\n", motor[motorList[i]]);
			}*/
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

//Initialize motor definitions and a DesiredEncVals struct to ENC_OFF
void motorInit(DesiredEncVals *desiredEncVals) {
	//VOLATILE
	//list all motors
	//initialize to -1 so that we can check for any missing motors
	int motorListSize = sizeof(motorList) / sizeof(tMotor);
	for (int i=0; i<motorListSize; i++) {
		motorList[i] = -1;
	}
	motorList[0] = MecMotor_FL;
	motorList[1] = MecMotor_BL;
	motorList[2] = MecMotor_FR;
	motorList[3] = MecMotor_BR;
	motorList[4] = Lift;
	motorList[5] = Harvester;
	motorList[6] = HarvesterMove;

	//init maxPower to MAX_NORMAL_POWER and minPower to MIN_NORMAL_POWER
	//init motor state encoder data to 0
	for (int i=0;i<MAX_NUM_MOTORS;i++) {
		motorDefinitions[i].maxPower = MAX_NORMAL_POWER;
		motorDefinitions[i].minPower = MIN_NORMAL_POWER;
		motorDefinitions[i].encSlowLength = 2000;
		motorDefinitions[i].encSlowStep = 20;
		motorDefinitions[i].encHitZone = 50;
		motorStates[i].encoder = 0;
		motorStates[i].lastRealEncoderPos = 0;
		motorStates[i].timePosMs = 0;
		motorStates[i].lastUpdateTimeMs = 0;
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

	//set lift data
	motorDefinitions[Lift].minPower = 33;
	motorDefinitions[Lift].maxPower = 100;
	motorDefinitions[Lift].encSlowLength = 1000;
	motorDefinitions[Lift].encSlowStep = 10;
	motorDefinitions[Lift].encHitZone = 100;

	//check to make sure motors listed fills up motorList length
	for (int i=0; i<sizeof(motorList) / sizeof(tMotor); i++) {
		if((int) motorList[i] == -1) {
			writeDebugStream("Motors failed to initialize! Make sure motorList contains all motors.");
			return;
		}
	}
	motorDefsInitialized = true;

	//Initialize enc vals to ENC_OFF
	for (int i=0; i<motorListSize; i++) {
		desiredEncVals->encoder[motorList[i]] = ENC_OFF;
	}
	motorResetAllEncoders(desiredEncVals);
}


#endif
