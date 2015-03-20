#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Hubs,  S3, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  mtr_S3_C1_1,     Michelangelo_FR, tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S3_C1_2,     Donatello_FL,  tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S3_C2_1,     Raphael_BR,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S3_C2_2,     Leonardo_BL,   tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S3_C3_1,     HarvesterMove, tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C3_2,     LiftEncoder,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C4_1,     Lift,					 tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S3_C4_2,     Harvester,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    servo1,        tServoNone)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,        tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,        tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4, 			 tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    TubeGrabber,   tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    Bucket,        tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// The sensor is connected to the first port
// of the SMUX which is connected to the NXT port S1.
// To access that sensor, we must use msensor_S1_1.  If the sensor
// were connected to 3rd port of the SMUX connected to the NXT port S4,
// we would use msensor_S4_3

#include "drivers/hitechnic-sensormux.h"
#include "drivers/hitechnic-irseeker-v2.h"
#include "drivers/hitechnic-gyro.h"
#include "drivers/lego-ultrasound.h"

// Give the sensor a nice easy to use name
const tMUXSensor GYRO = msensor_S4_1;
const tMUXSensor HTIRS2 = msensor_S4_4;
const tMUXSensor LEGOUS = msensor_S4_3;

#include "Headers\Joystick.h"
#include "Headers\Motor.h"
#include "Headers\Servo.h"
#include "Headers\Global.h"
#include "Headers\Drive.h"
#include "Auton\Auton.h"

//ALl states for Grab Medium Tube Autonomous
typedef enum PGrabMedTubeStates {
	STATE_START, //
	STATE_TURNTOWARDTUBE,
	STATE_DRIVETOWARDTUBE,
	STATE_DRIVEGRABTUBE,
	STATE_CLOSEBUCKET,//
	STATE_CORRECTIONTURN,
	STATE_DRIVETOWARDPZONE, //
	STATE_RAISELIFT,
	STATE_ROTTOWARDPZONE,
	STATE_DROPBUCKET, //
	STATE_LOWERLIFT,
	STATE_WAITFORLIFT,
	STATE_ROTATETUBE,
	STATE_END,
} PGrabMedTubeStates;

static PGrabMedTubeStates currentState = STATE_START;

task main()
{
	//SLIDE SIDE IS FACING TOWARD THE CENTERPIECE
	long bucketStartTimeMs = 0;
	bool end = false;
	bool init = true;
	bool grabbing = false;

	initialize();
	writeDebugStream("This is ParkGrabMedTube\n");
	waitForStart();

	while(!end){
		long loopStartTimeMs = nPgmTime;
		if (rest) {
			writeDebugStream("Resting!\n");
			motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
			motorSetActualPowerToDesired(&desiredMotorVals);
			motorUpdateState();
			if (nPgmTime - restStartTimeMs > 2000) {
				rest = false;
				restStartTimeMs = 0;
			}
		} else {
			switch(currentState){
				case STATE_START:
					currentState = STATE_TURNTOWARDTUBE;
					break;
				case STATE_TURNTOWARDTUBE:
					writeDebugStream("State: Turn Toward Tube\n");
					if (init) {
						driveSetEncoderRotateCW(&desiredEncVals, 750);
						init = false;
					}

					driveSetMecMotorRotateCW(&desiredMotorVals, 11.0);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						currentState = STATE_DRIVETOWARDTUBE;
						init = true;
					}
					break;
				case STATE_DRIVETOWARDTUBE:
					writeDebugStream("State: Drive Toward Tube\n");
					if (init) {
						driveSetEncoderN(&desiredEncVals, 6.0 * ENC_PER_REV);
					}

					desiredMotorVals.power[Lift] = LIFT_UP;
					motorSetEncoder(&desiredEncVals, Lift, SIXTY_GOAL, ENCFLAGS_CAPMODE_ON ^ ENCFLAGS_CAPMAX_ON);

					//We limit ONLY the lift during the first part where grabbing = false
					//This will limit drive motors during second part where grabbing = true
					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);

					if (!grabbing) {
						//This overrides limit on drive motors so we don't stop during first part where
					  //grabbing = false.
					  //This also initializes second part where grabbing will be set to true in the next
						//if statement.
						driveSetMecMotorN(&desiredMotorVals, 0.5);
						if(driveMecHasHitEncoderTarget(&desiredEncVals)) {
							driveSetEncoderN(&desiredEncVals, 750);
							servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].minValue);
							grabbing = true;
						}
					} else {
						if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
							restMecMotors();
							currentState = STATE_WAITFORLIFT;
							init = true;
						}
					}

					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					break;
				case STATE_WAITFORLIFT:
					writeDebugStream("State: WAIT FOR LIFT\n");
					desiredMotorVals.power[Lift] = LIFT_UP;
					motorSetEncoder(&desiredEncVals, Lift, SIXTY_GOAL, ENCFLAGS_CAPMODE_ON ^ ENCFLAGS_CAPMAX_ON);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (motorHasHitEncoderTarget(&desiredEncVals, Lift)) {
						restMecMotors();
						currentState = STATE_DROPBUCKET;
						init = true;
					}
					break;
				case STATE_DROPBUCKET:
					writeDebugStream("State: Drop Bucket\n");
					if (init) {
						bucketStartTimeMs = nPgmTime;
						servoSetNonCont(Bucket, servoDefinitions[Bucket].minValue);
						init = false;
					} else {
						if ((nPgmTime - bucketStartTimeMs) > 3000) { //balls need time to drop
							restMecMotors();
							currentState = STATE_CLOSEBUCKET;
							init = true;
						}
					}
					break;
				case STATE_CLOSEBUCKET:
					writeDebugStream("State: Close bucket\n");
					if (init) {
						bucketStartTimeMs = nPgmTime;
						servoSetNonCont(Bucket, servoDefinitions[Bucket].maxValue);
						init = false;
					} else {
						if ((nPgmTime - bucketStartTimeMs) > 1000) {
							restMecMotors();
							currentState = STATE_ROTTOWARDPZONE;
							init = true;
						}
					}
					break;
				case STATE_ROTTOWARDPZONE:
					writeDebugStream("State: Rotate Toward Parking Zone\n");
					if (init) {
						driveSetEncoderRotateCW(&desiredEncVals, 5400);
						init = false;
					}

					driveSetMecMotorRotateCW(&desiredMotorVals, 0.30);
					desiredMotorVals.power[Lift] = LIFT_DOWN;
					motorSetEncoder(&desiredEncVals, Lift, LIFT_MIN, ENCFLAGS_CAPMODE_ON);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						currentState = STATE_DRIVETOWARDPZONE;
						init = true;
					}
					break;
				case STATE_DRIVETOWARDPZONE:
					writeDebugStream("State: Drive Toward Zone\n");
					if (init) {
						driveSetEncoderN(&desiredEncVals, 7500);
						init = false;
					}

					driveSetMecMotorN(&desiredMotorVals, 0.30);
					desiredMotorVals.power[Lift] = LIFT_DOWN;
					motorSetEncoder(&desiredEncVals, Lift, LIFT_MIN, ENCFLAGS_CAPMODE_ON);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						currentState = STATE_END;//STATE_ROTATETUBE;
						init = true;
					}
					break;
				case STATE_ROTATETUBE:
					writeDebugStream("State: Rotate Tube\n");
					if (init) {
						driveSetEncoderRotateCW(&desiredEncVals, 2000);
						init = false;
					}

					driveSetMecMotorRotateCW(&desiredMotorVals, 0.30);
					desiredMotorVals.power[Lift] = LIFT_DOWN;
					motorSetEncoder(&desiredEncVals, Lift, LIFT_MIN, ENCFLAGS_CAPMODE_ON);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						currentState = STATE_END;
						init = true;
					}
					break;
				case STATE_END:
					desiredMotorVals.power[Lift] = LIFT_DOWN;
					motorSetEncoder(&desiredEncVals, Lift, LIFT_MIN, ENCFLAGS_CAPMODE_ON);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(motorHasHitEncoderTarget(&desiredEncVals, Lift)) {
						restMecMotors();
						end = true;
					}
					break;
			} //switch
		} //if
	//writeDebugStream("DMV: %d %d %d %d\n", desiredMotorVals.power[MecMotor_FL], desiredMotorVals.power[MecMotor_BL], desiredMotorVals.power[MecMotor_FR], desiredMotorVals.power[MecMotor_BR]);
	//writeDebugStream("DEV: %d %d %d %d\n", desiredEncVals.encoder[MecMotor_FL], desiredEncVals.encoder[MecMotor_BL], desiredEncVals.encoder[MecMotor_FR], desiredEncVals.encoder[MecMotor_BR]);
	//writeDebugStream("VIRTENC: %d %d %d %d\n", motorGetEncoder((tMotor) MecMotor_FL), motorGetEncoder((tMotor) MecMotor_BL), motorGetEncoder((tMotor) MecMotor_FR), motorGetEncoder((tMotor) MecMotor_BR));
	//writeDebugStream("REALENC: %d %d %d %d\n", nMotorEncoder[Donatello_FL], nMotorEncoder[Leonardo_BL], nMotorEncoder[Michelangelo_FR], nMotorEncoder[Raphael_BR]);
	//writeDebugStream("REALMOTOR: %d %d %d %d\n", motor[MecMotor_FL], motor[MecMotor_BL], motor[MecMotor_FR], motor[MecMotor_BR]);
	//writeDebugStream("Lift target: %d\n", desiredEncVals.encoder[Lift]);
	//writeDebugStream("Lift enc: %d\n", motorGetEncoder(Lift));
	writeDebugStream("Full auton loop took: %d ms\n", nPgmTime - loopStartTimeMs);
	} //while

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
