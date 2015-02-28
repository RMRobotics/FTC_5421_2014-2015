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
#pragma config(Motor,  mtr_S3_C3_2,     motor0,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C4_1,     Lift,					 tmotorTetrix, openLoop, encoder)
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

#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-irseeker-v2.h"
#include "../drivers/hitechnic-gyro.h"
#include "../drivers/lego-ultrasound.h"

// Give the sensor a nice easy to use name
const tMUXSensor GYRO = msensor_S4_1;
const tMUXSensor HTIRS2 = msensor_S4_4;
const tMUXSensor LEGOUS = msensor_S4_3;


#include "..\Headers\joystick.h"
#include "..\Headers\Motor.h"
#include "..\Headers\Servo.h"
#include "..\Headers\Global.h"
#include "..\Headers\Drive.h"

//Stores desired motor values
DesiredMotorVals desiredMotorVals;
//Stores desired encoder values
DesiredEncVals desiredEncVals;

//ALl states for Grab Medium Tube Autonomous
typedef enum GrabMedTubeStates {
	STATE_START, //
	STATE_ALIGNONRAMP, //
	STATE_OFFRAMP, //
	STATE_DRIVETOWARDTUBE, //
	STATE_DRIVEGRABTUBE,
	STATE_CLOSEBUCKET,//
	STATE_CORRECTIONTURN,
	STATE_DRIVETOWARDRAMP, //
	STATE_DRIVETOWARDCENTERPIECE, //
	STATE_DRIVETOWARDPZONE, //
	STATE_RAISELIFT,
	STATE_ROTTOWARDPZONE,
	STATE_DROPBUCKET, //
	STATE_LOWERLIFT,
	STATE_WAITFORLIFT,
	STATE_ROTATETUBE,
	STATE_END,
} GrabMedTubeStates;

static GrabMedTubeStates currentState = STATE_START;

void initialize(){
	clearDebugStream();
	writeDebugStream("This is RampGrabMedTube\n");
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
	long tubeGrabStartTimeMs = 0;

	initialize();
	waitForStart();
	while(!end){
		long loopStartTimeMs = nPgmTime;
		if (rest) {
			writeDebugStream("resting!\n");
			if (nPgmTime - restStartTimeMs > 1000) {
				rest = false;
				restStartTimeMs = 0;
			}
		} else {
			switch(currentState){
				case STATE_START:
					driveSetEncoderW(&desiredEncVals, .10 * ENC_PER_REV);
					currentState = STATE_ALIGNONRAMP; //STATE_ALIGNONRAMP
					break;
				case STATE_ALIGNONRAMP:
					writeDebugStream("State: AlignonRamp\n");
					driveSetMecMotorW(&desiredMotorVals, 0.25);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						driveSetEncoderN(&desiredEncVals, 4.0 * ENC_PER_REV); //for next state
						currentState = STATE_OFFRAMP;
					}
					break;
				case STATE_OFFRAMP:
					writeDebugStream("State: Offramp\n");
					driveSetMecMotorN(&desiredMotorVals, 0.75);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)){
						restMecMotors();
						driveSetEncoderRotateCW(&desiredEncVals, 0.10 * ENC_PER_REV); //for next state
						currentState = STATE_CORRECTIONTURN;
					}
					break;
				case STATE_CORRECTIONTURN:
					writeDebugStream("State: Correction turn\n");
					driveSetMecMotorRotateCW(&desiredMotorVals, -1.0);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(driveMecHasHitEncoderTarget(&desiredEncVals)){
						restMecMotors();
						driveSetEncoderN(&desiredEncVals, 2.0 * ENC_PER_REV); //for next state
						motorSetEncoder(&desiredEncVals, Lift, -SIXTY_GOAL, true); //for next state
						currentState = STATE_DRIVETOWARDTUBE;
					}
				case STATE_DRIVETOWARDTUBE:
					writeDebugStream("State: Drive Toward Tube\n");

					desiredMotorVals.power[Lift] = -100;
					//NEED TO LIMIT ONLY LIFT HERE
					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);

					driveSetMecMotorN(&desiredMotorVals, 0.5);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(driveMecHasHitEncoderTarget(&desiredEncVals)) { //keep driving
						servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].minValue);
						tubeGrabStartTimeMs = nPgmTime;
					}

					if (tubeGrabStartTimeMs > 500) {
						motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
						restMecMotors();
						currentState = STATE_DRIVEGRABTUBE;
					}
					break;
				case STATE_WAITFORLIFT:
					writeDebugStream("State: WAIT FOR LIFT\n");

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(motorAllHitEncoderTarget(&desiredEncVals)) { //wait for lift
						restMecMotors();
						motorResetEncoder(&desiredEncVals, Lift);
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
					driveSetEncoderRotateCW(&desiredEncVals, 0.25 * ENC_PER_REV); //for next state
					motorSetEncoder(&desiredEncVals, Lift, 0, true); //for next state
					currentState = STATE_ROTTOWARDPZONE;
				case STATE_ROTTOWARDPZONE:
					writeDebugStream("State: Rotate Toward Parking Zone\n");
					desiredMotorVals.power[Lift] = 100;
					driveSetMecMotorRotateCW(&desiredMotorVals, 1.0);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(driveMecHasHitEncoderTarget(&desiredEncVals)){
						restMecMotors();
						driveSetEncoderS(&desiredEncVals, 6 * ENC_PER_REV); //for next state
						currentState = STATE_DRIVETOWARDPZONE;
					}
					break;
				case STATE_DRIVETOWARDPZONE:
					writeDebugStream("State: Drive Toward Zone\n");
					driveSetMecMotorS(&desiredMotorVals, 1.0);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(motorAllHitEncoderTarget(&desiredEncVals)){
						restMecMotors();
						motorResetEncoder(desiredEncVals, Lift);
						driveSetEncoderRotateCW(&desiredEncVals, 1.25 * ENC_PER_REV);
						currentState = STATE_ROTATETUBE;
					}
					break;
				case STATE_ROTATETUBE:
					writeDebugStream("State: Rotate Tube\n");
					driveSetMecMotorRotateCW(&desiredMotorVals, 1.0);

					if (motorAllHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						currentState = STATE_END;
					}
					currentState = STATE_END;
				case STATE_END:
					if(motorAllHitEncoderTarget(&desiredEncVals)){
						restMecMotors();
						motorResetAllEncoders(desiredEncVals);
						end = true;
					}
					break;
			}
		}
	//writeDebugStream("DMV: %d %d %d %d\n", desiredMotorVals.power[MecMotor_FL], desiredMotorVals.power[MecMotor_BL], desiredMotorVals.power[MecMotor_FR], desiredMotorVals.power[MecMotor_BR]);
	//writeDebugStream("DEV: %d %d %d %d\n", desiredEncVals.encoder[MecMotor_FL], desiredEncVals.encoder[MecMotor_BL], desiredEncVals.encoder[MecMotor_FR], desiredEncVals.encoder[MecMotor_BR]);
	//writeDebugStream("VIRTENC: %d %d %d %d\n", motorGetEncoder((tMotor) MecMotor_FL), motorGetEncoder((tMotor) MecMotor_BL), motorGetEncoder((tMotor) MecMotor_FR), motorGetEncoder((tMotor) MecMotor_BR));
	//writeDebugStream("REALENC: %d %d %d %d\n", nMotorEncoder[Donatello_FL], nMotorEncoder[Leonardo_BL], nMotorEncoder[Michelangelo_FR], nMotorEncoder[Raphael_BR]);
	//writeDebugStream("REALMOTOR: %d %d %d %d\n", motor[MecMotor_FL], motor[MecMotor_BL], motor[MecMotor_FR], motor[MecMotor_BR]);
		writeDebugStream("Lift target: %d\n", desiredEncVals.encoder[Lift]);
		writeDebugStream("Lift enc: %d\n", motorGetEncoder(Lift));
	writeDebugStream("Full auton loop took: %d ms\n", nPgmTime - loopStartTimeMs);
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
