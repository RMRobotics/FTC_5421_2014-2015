#pragma config(UserModel, "Headers/Pragmas.h")

#include "Headers\Global.h"
#include "Headers\Drive.h"
#include "Headers\joystick.h"
#include "Headers\Motor.h"
#include "Headers\Servo.h"

//Stores desired motor values
DesiredMotorVals desiredMotorVals;
//Stores desired encoder values
DesiredEncVals desiredEncVals;

//ALL states for center Tube Autonomous
typedef enum CenterTubeStates {
	STATE_START, //
	STATE_DRIVETOWARDCENTERPIECE, //
	STATE_DETECTIR,
	STATE_ORBITTOCENTERGOAL,//
	STATE_DROPBUCKET, //
	STATE_DRIVERIGHT, //
	STATE_KICKSTAND, //
	STATE_WAITFORLIFT,
	STATE_END,
	STATE_SHORTBUCKET,
	STATE_CLOSEBUCKET,
} CenterTubeStates;

static CenterTubeStates currentState = STATE_START;

//States for center position
typedef enum CenterPos {
	HORIZ_TO_PARK,
	ANGLE_TO_PARK,
	VERT_TO_PARK,
} CenterPos;

void initialize(){
	clearDebugStream();
	writeDebugStream("This is CenterTubeIR\n");
	memset(&desiredMotorVals, 0, sizeof(desiredMotorVals));
	memset(&desiredEncVals, 0, sizeof(desiredEncVals));
	servoInit();
	motorInit(&desiredEncVals);
	motorResetAllEncoders(&desiredEncVals);
}

#define LIFT_MAX 10500
#define NINETY_GOAL 7800
#define SIXTY_GOAL 4600
#define THIRTY_GOAL 2000
#define LIFT_MIN 0

#define ENC_PER_REV 1110

//IR Sensor
int dirEnh, strEnh = 0; //direction, strength enhanced

long restStartTimeMs = 0;
bool rest = false;
bool end = false;

void restMecMotors() {
	driveResetMecEncoder(&desiredEncVals);
	driveZeroMecMotor(&desiredMotorVals);
	motorSetActualPowerToDesired(&desiredMotorVals);
	rest = true;
	restStartTimeMs = nPgmTime;
}


task main()
{
	//SLIDE SIDE IS FACING DOWN THE RAMP
	CenterPos centerPos;
	initialize();
	waitForStart();

	while(!end){
		if (rest) {
			writeDebugStream("resting!\n");
			motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
			motorSetActualPowerToDesired(&desiredMotorVals);
			motorUpdateState();
			if (nPgmTime - restStartTimeMs > 1000) {
				rest = false;
				restStartTimeMs = 0;
			}
		} else {
			switch(currentState){
				case STATE_START:
					currentState = STATE_DRIVETOWARDCENTERPIECE;
					break;
				case STATE_DRIVETOWARDCENTERPIECE:
					writeDebugStream("State: Drive Toward Centerpiece\n");

					motorUpdateState();

					motor[Lift] = -100;

					if (motorGetEncoder(Lift) <= -SIXTY_GOAL) {
						motor[Lift] = 0;
					}

					motor[MecMotor_FL] = 100;
					motor[MecMotor_BL] = 100;
					motor[MecMotor_FR] = 100;
					motor[MecMotor_BR] = 100;

					if (motorGetEncoder((tMotor)MecMotor_FL) < 1000) {
						motor[Lift] = 0;
						restMecMotors();
						currentState = STATE_DETECTIR;
					}

					break;
				case STATE_DETECTIR:
					writeDebugStream("State: Detect IR\n");
					HTIRS2readEnhanced(HTIRS2, dirEnh, strEnh);

					if (strEnh > 30) {
						writeDebugStream("Detected vertical to park\n");
						centerPos = VERT_TO_PARK;
					} else if (strEnh > 10) {
						writeDebugStream("Detected angle to park\n");
						centerPos = ANGLE_TO_PARK;
					} else {
						writeDebugStream("Detected horizontal to park\n");
						centerPos = HORIZ_TO_PARK;
					}

					if (centerPos == VERT_TO_PARK) {
						currentState = STATE_WAITFORLIFT;
					} else {
						currentState = STATE_ORBITTOCENTERGOAL;
					}
					break;
			case STATE_ORBITTOCENTERGOAL:
					writeDebugStream("State: Orbit to center goal\n");

					motorUpdateState();

					motor[Lift] = -100;

					if (motorGetEncoder(Lift) <= -LIFT_MAX) {
						motor[Lift] = 0;
					}

					driveSetMecMotorPolarDegrees(&desiredMotorVals, 315, 1.0, 0.5);
					motor[MecMotor_FL] = desiredMotorVals.power[MecMotor_FL];
					motor[MecMotor_BL] = desiredMotorVals.power[MecMotor_BL];
					motor[MecMotor_FR] = desiredMotorVals.power[MecMotor_FR];
					motor[MecMotor_BR] = desiredMotorVals.power[MecMotor_BR];

					if (centerPos == ANGLE_TO_PARK) {
						if (motorGetEncoder((tMotor)MecMotor_FL) < 2000) {
							restMecMotors();
							currentState = STATE_WAITFORLIFT;
						}
					} else if (centerPos == HORIZ_TO_PARK) {
						if (motorGetEncoder((tMotor)MecMotor_FL) < 3000) {
							restMecMotors();
							currentState = STATE_WAITFORLIFT;
						}
					}
					break;
				case STATE_WAITFORLIFT:
					writeDebugStream("State: WAIT FOR LIFT\n");

					motorUpdateState();

					motor[Lift] = -100;

					if (motorGetEncoder(Lift) <= -LIFT_MAX) {
						motor[Lift] = 0;
						restMecMotors();
						currentState = STATE_DROPBUCKET;
					}
					break;
				case STATE_DROPBUCKET:
					writeDebugStream("State: Drop Bucket\n");

					servoSetNonCont(Bucket, servoDefinitions[Bucket].minValue);

					restMecMotors();
					currentState = STATE_CLOSEBUCKET;
					break;
				case STATE_CLOSEBUCKET:
					writeDebugStream("State: Close bucket\n");
					servoSetNonCont(Bucket, servoDefinitions[Bucket].maxValue);

					restMecMotors();

					currentState = STATE_DRIVERIGHT;
				case STATE_DRIVERIGHT:
					writeDebugStream("State: Drive Right Toward Kickstand\n");

					motorUpdateState();

					motor[Lift] = 15;

					if (motorGetEncoder(Lift) >= 500) {
						motor[Lift] = 0;
					}

					motor[MecMotor_FL] = 100;
					motor[MecMotor_BL] = -100;
					motor[MecMotor_FR] = -100;
					motor[MecMotor_BR] = 100;

					if (motorGetEncoder((tMotor) MecMotor_FL) > 2000) {
						restMecMotors();
						currentState = STATE_KICKSTAND;
					}

					break;
				case STATE_KICKSTAND:
					writeDebugStream("State: Drive Kickstand\n");
					motor[Lift] = 15;

					if (motorGetEncoder(Lift) >= 500) {
						motor[Lift] = 0;
					}

					motor[MecMotor_FL] = 100;
					motor[MecMotor_BL] = 100;
					motor[MecMotor_FR] = 100;
					motor[MecMotor_BR] = 100;

					if (motorGetEncoder(MecMotor_FL) > 2000) {
						restMecMotors();
						currentState = STATE_END;
					}

					break;
				case STATE_END:
					motor[Lift] = 15;

					if (motorGetEncoder(Lift) >= 500) {
						motor[Lift] = 0;
						end = true;
						motorResetAllEncoders(&desiredEncVals);
					}
					break;
			}
		}
	writeDebugStream("DMV: %d %d %d %d\n", desiredMotorVals.power[MecMotor_FL], desiredMotorVals.power[MecMotor_BL], desiredMotorVals.power[MecMotor_FR], desiredMotorVals.power[MecMotor_BR]);
	writeDebugStream("VIRTENC: %d %d %d %d\n", motorGetEncoder((tMotor) MecMotor_FL), motorGetEncoder((tMotor) MecMotor_BL), motorGetEncoder((tMotor) MecMotor_FR), motorGetEncoder((tMotor) MecMotor_BR);
	writeDebugStream("REALENC: %d %d %d %d\n", nMotorEncoder[Donatello_FL], nMotorEncoder[Leonardo_BL], nMotorEncoder[Michelangelo_FR], nMotorEncoder[Raphael_BR]);
	}

/*	driveSetEncoderN(&desiredEncVals, ######);
	driveSetEncoderW(&desiredEncVals, ######);
	driveSetEncoderN(&desiredEncVals, ######);
	servoSetNonCont(Wing_Base, servoDefinitions[Wing_Base].minValue);
	driveSetEncoderS(&desiredEncVals, ######);
	desiredMotorVals->power[Wing_Middle] = -100;
	time[T1] = 0;
	while (time[T1] < 200){
		motorSetActualPowerToDesired(&desiredMotorVals);
	}
	desiredMotorVals->power[Wing_Middle] = 0;
	motorSetActualPowerToDesired(&desiredMotorVals);
	driveSetEncoderW(&desiredEncVals, ######);
	driveSetEncoderS(&desiredEncVals, ######);
	driveSetEncoderW(&desiredEncVals, ######); */
}