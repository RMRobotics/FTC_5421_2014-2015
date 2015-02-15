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

/*Max length (in encoder units) over which we slow down the robot in order to hit the
  target precisely. */
#define ENC_SLOW_LENGTH 200
/* Encoder step by which we decrement motor power by 1 */
#define ENC_SLOW_STEP 20

/*Encoder target value which we ignore (use this when you want to ignore encoder checks) */
#define ENC_OFF -32767

/*Maximum encoder target possible. This is technically the maximum number an int
  variable can hold, but we'll give it some room so that we don't introduce a
  race condition between the overflow and the encoder check. */
#define MAX_ENC_TARGET 30000

//Slew rate for scaling values (amount to add per loop). Percentage.
#define MOTOR_SLEW_RATE 100

//Struct for storing motor power limits for each motor
typedef struct MotorData {
	int maxPower;
	int minPower;
} MotorData;

//Array for storing MotorData for each motor
static MotorData motorDefinitions[MAX_NUM_MOTORS];

//Flag for seeing if motor definitions have been initialized
bool motorDefsInitialized = false;


#define NUM_MOTORS 7
//Array for storing all motor enums that we use (this way we can loop through)
tMotor motorList[NUM_MOTORS];

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
}

/*Returns max reference power for other functions */
int motorGetMaxReferencePower() {
	return MAX_REFERENCE_POWER;
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

/* Returns real encoder value for a specific tMotor.
	 This function attempts a check for bad encoder
	 values but will abort the check if the function takes
	 longer than the specified time. */
int motorGetEncoder(tMotor curMotor) {
	long encFnStartTimeMs = nPgmTime;
	int returnEnc = 0;
	int pow = motor[curMotor];

	//Check for sporadic encoder values as documented by Cougar Robotics #623
	int checkEnc = nMotorEncoder[curMotor];
	int curEnc = nMotorEncoder[curMotor];

	if (pow > 0) { //we expect encoder value to be changing
		while (true) {
			//This can potentially take a long time, so abort after specified time
			if ((nPgmTime - encFnStartTimeMs) > 5) {
				writeDebugStream("Bad enc check taking too long, aborting!\n");
				break;
			}
			while (curEnc == checkEnc) {
				//This can also potentially take a long time, so abort after specified time
				if ((nPgmTime - encFnStartTimeMs) > 5) {
					writeDebugStream("Bad enc check taking too long, aborting!\n");
					break;
				}
				curEnc = nMotorEncoder[curMotor];
			}
			if ((sgn(curEnc - checkEnc) == sgn(pow)) {
				break;
			} else {
				checkEnc = curEnc;
			}
		}
	} else { //we expect encoder value to be still
		//monitor for short period
		while ((nPgmTime - encFnStartTimeMs) < 5) {
			curEnc = nMotorEncoder[curMotor];
			if (curEnc != checkEnc) {
				//assume latest value is good
				checkEnc = curEnc;
			}
		}
	}

	long encFnTimeMs = nPgmTimeMs - encFnStartTimeMs;
	if (encFnTimeMs > 5) {
		writeDebugStream("motorGetEncoder function call took %d ms!", encFnTimeMs);
	}
	return curEnc;
}

/* Resets encoder, desired and real, for a specific tMotor */
void motorResetEncoder(DesiredEncVals *desiredEncVals, tMotor curMotor) {
	if (motorDefsInitialized) {
		desiredEncVals->encoder[curMotor] = ENC_OFF;
		nMotorEncoder[curMotor] = 0;
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

/* Resets all encoders, desired and real */
void motorResetAllEncoders(DesiredEncVals *desiredEncVals) {
	if (motorDefsInitialized) {
		for (int i=0;i<NUM_MOTORS;i++) {
			motorResetEncoder(desiredEncVals, motorList[i]);
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

/* Sets encoder target for a specific tMotor and resets
	 the real encoder. Checks to make sure the target is
	 below the maximum target, and that the target is not
	 ENC_OFF.
	 If it fails the check, then the encoder is still reset
	 but it refuses to set the
	 new target. */
void motorSetEncoder(DesiredEncVals *desiredEncVals, tMotor curMotor, int target) {
	if (motorDefsInitialized) {
		motorResetEncoder(desiredEncVals, curMotor);
		if (abs(target) > MAX_ENC_TARGET) {
			writeDebugStream("Encoder target value (%d) past maximum target value!\n", target);
		} else if (target == ENC_OFF) {
			writeDebugStream("Encoder target value cannot be (%d)!\n", ENC_OFF);
		} else {
			desiredEncVals->encoder[curMotor] = target;
		}
	} else {
		writeDebugStream("Motors not initialized!\n");
	}
}

/*Checks if a specific tMotor has hit its encoder targets.
  - If there is no target set, returns true.
  - If the signs of the desired encoder value and the actual
  	encoder value do not agree, then this returns false.*/
bool motorHasHitEncoderTarget(DesiredEncVals *desiredEncVals, tMotor curMotor) {
	int desiredEnc = desiredEncVals->encoder[curMotor];
	int curEnc = motorGetEncoder(curMotor);
	if (desiredEnc == ENC_OFF) {
		return true;
	} else if ((sgn(curEnc) == sgn(desiredEnc)) && (abs(curEnc) >= abs(desiredEnc))) {
		return true;
	} else {
		return false;
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
			int desiredEnc = desiredEncVals->encoder[curMotor];
			int curEnc = motorGetEncoder(curMotor);
			if (desiredEnc != ENC_OFF && (desiredMotorVals->power[curMotor] != 0)) {
				if ((sgn(desiredEnc) == sgn(curEnc)) || curEnc == 0) {
					if (motorHasHitEncoderTarget(desiredEncVals, curMotor)) {
						desiredMotorVals->power[curMotor] = 0;
					} else if (abs(desiredEnc - curEnc) < ENC_SLOW_LENGTH) {
						//Calculate number of steps left to decrement motor
						long steps = ((long) desiredEnc - curEnc) / ENC_SLOW_STEP;
						//Add one to make sure motor never shuts off and set target
						desiredMotorVals->power[curMotor] = (int) steps + ((steps > 0)?1:-1);

					}
				} else {
					writeDebugStream("Motor (%d) is not going in the same direction as the encoder target (%d)!\n", curMotor, desiredEnc);
					writeDebugStream("Current encoder value: %d\n", curEnc);
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


#endif
