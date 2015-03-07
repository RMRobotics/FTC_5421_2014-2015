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
bool grabbing = false;

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
	long bucketStartTimeMs = 0;

	initialize();
	waitForStart();
	motorResetAllEncoders(&desiredEncVals);
	while(!end){
		long loopStartTimeMs = nPgmTime;
		if (rest) {
			writeDebugStream("resting!\n");
			motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
			motorSetActualPowerToDesired(&desiredMotorVals);
			motorUpdateState();
			writeDebugStream("Rest difference: %d\n", nPgmTime - restStartTimeMs);
			if (nPgmTime - restStartTimeMs > 2000) {
				rest = false;
				restStartTimeMs = 0;
			}
		} else {
			switch(currentState){
				case STATE_START:
					driveSetEncoderW(&desiredEncVals, .15 * ENC_PER_REV);
					currentState = STATE_ALIGNONRAMP;
					break;
				case STATE_ALIGNONRAMP:
					writeDebugStream("State: AlignonRamp\n");
					driveSetMecMotorW(&desiredMotorVals, 0.05);

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
					driveSetMecMotorN(&desiredMotorVals, 0.25);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)){
						restMecMotors();
						motorResetAllEncoders(&desiredEncVals);
						motorZeroAllMotors(&desiredMotorVals);
						//driveSetEncoderN(&desiredEncVals, 5.0 * ENC_PER_REV); //for next state
						//motorSetEncoder(&desiredEncVals, Lift, -SIXTY_GOAL, true); //for next state
						//driveSetEncoderRotateCW(&desiredEncVals, 0.5 * ENC_PER_REV); //for next state
						//currentState = STATE_CORRECTIONTURN;
						currentState = STATE_DRIVETOWARDTUBE;
					}
					break;
				/*case STATE_CORRECTIONTURN:
					writeDebugStream("State: Correction turn\n");
					driveSetMecMotorRotateCW(&desiredMotorVals, 0.25);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(driveMecHasHitEncoderTarget(&desiredEncVals)){
						restMecMotors();
						//driveSetEncoderN(&desiredEncVals, 1.6 * ENC_PER_REV); //for next state
						//motorSetEncoder(&desiredEncVals, Lift, -SIXTY_GOAL, true); //for next state
						currentState = STATE_DRIVETOWARDTUBE;
					} */
				case STATE_DRIVETOWARDTUBE:
					writeDebugStream("State: Drive Toward Tube\n");

					motorUpdateState();

					/*
					desiredMotorVals.power[Lift] = -100;
					driveSetMecMotorN(&desiredMotorVals, 0.5);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState(); */


					motor[Lift] = -75;
					//NEED TO LIMIT ONLY LIFT HERE

					motor[MecMotor_FL] = 25;
					motor[MecMotor_BL] = 25;
					motor[MecMotor_FR] = 25;
					motor[MecMotor_BR] = 25;

					if (motorGetEncoder(Lift) < -(SIXTY_GOAL)) {
						motor[Lift] = 0;
					}

					if (motorGetEncoder((tMotor) MecMotor_FL) > 4000) {
						motor[MecMotor_FL] = 0;
						motor[MecMotor_BL] = 0;
						motor[MecMotor_FR] = 0;
						motor[MecMotor_BR] = 0;
						motor[Lift] = 0;
						restMecMotors();
						currentState = STATE_WAITFORLIFT;
					} else if (motorGetEncoder((tMotor) MecMotor_FL) > 3600) {
						servoSetNonCont(TubeGrabber, (servoDefinitions[TubeGrabber].maxValue*(.75)))
					}

					/*if(!grabbing) {
						if(driveMecHasHitEncoderTarget(&desiredEncVals)) { //keep driving
							writeDebugStream("Drive toward tube hit enc target!\n");
							servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].minValue);
							grabbing = true;
							tubeGrabStartTimeMs = nPgmTime;
						}
					}

					if (((nPgmTime - tubeGrabStartTimeMs) > 500) && tubeGrabStartTimeMs != 0) {
						motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
						restMecMotors();
						currentState = STATE_WAITFORLIFT;
					}
					if(driveMecHasHitEncoderTarget(&desiredEncVals)){
					} */
					break;
				case STATE_WAITFORLIFT:
					writeDebugStream("State: WAIT FOR LIFT\n");
					motorUpdateState();

					motor[Lift] = -75;
					writeDebugStream("Lift enc: %d\n", motorGetEncoder(Lift));
					if (motorGetEncoder(Lift) < -SIXTY_GOAL) {
						motor[Lift] = 0;
						restMecMotors();
						currentState = STATE_DROPBUCKET;
					}
					writeDebugStream("Motor lift: %d\n", motor[Lift]);
					break;
				case STATE_DROPBUCKET:

					writeDebugStream("State: Drop Bucket\n");
					servoSetNonCont(Bucket, servoDefinitions[Bucket].minValue);
					bucketStartTimeMs = nPgmTime;
					restMecMotors();
					currentState = STATE_CLOSEBUCKET;
					break;
				case STATE_CLOSEBUCKET:
					writeDebugStream("State: Close bucket\n");
					writeDebugStream("Bucket start diff: %d\n", nPgmTime - bucketStartTimeMs);
					servoSetNonCont(Bucket, servoDefinitions[Bucket].maxValue);
					if ((nPgmTime - bucketStartTimeMs) > 5000) {
						restMecMotors();
						currentState = STATE_ROTTOWARDPZONE;
					}
				case STATE_ROTTOWARDPZONE:
					writeDebugStream("State: Rotate Toward Parking Zone\n");

					motorUpdateState();
					/*
					motor[Lift] = 15;

					if (motorGetEncoder(Lift) >= -500) {
						motor[Lift] = 0;
					}*/

					motor[MecMotor_FL] = 25;
					motor[MecMotor_BL] = 25;
					motor[MecMotor_FR] = -25;
					motor[MecMotor_BR] = -25;

					if (motorGetEncoder((tMotor) MecMotor_FL) > 2700) {
						motor[Lift] = 0;
						restMecMotors();
						currentState = STATE_DRIVETOWARDPZONE;
					}

					break;
				case STATE_DRIVETOWARDPZONE:
					writeDebugStream("State: Drive Toward Zone\n");

					motorUpdateState();
					/*
					motor[Lift] = 15;

					if (motorGetEncoder(Lift) >= -500) {
						motor[Lift] = 0;
					}*/

					motor[MecMotor_FL] = 100;
					motor[MecMotor_BL] = 100;
					motor[MecMotor_FR] = 100;
					motor[MecMotor_BR] = 100;


					if (motorGetEncoder((tMotor)MecMotor_FL) > 7500) {
						motor[MecMotor_FL] = 0;
						motor[MecMotor_BL] = 0;
						motor[MecMotor_FR] = 0;
						motor[MecMotor_BR] = 0;
						motor[Lift] = 0;
						restMecMotors();
						currentState = STATE_END;//STATE_ROTATETUBE;
					}

					break;
				case STATE_ROTATETUBE:
					writeDebugStream("State: Rotate Tube\n");

					motorUpdateState();
					/*
					motor[Lift] = 100;

					if (motorGetEncoder(Lift) >= -500) {
						motor[Lift] = 0;
					}*/

					motor[MecMotor_FL] = 25;
					motor[MecMotor_BL] = 25;
					motor[MecMotor_FR] = -25;
					motor[MecMotor_BR] = -25;

					if (motorGetEncoder((tMotor)MecMotor_FL) > 2000) {
						motor[MecMotor_FL] = 0;
						motor[MecMotor_BL] = 0;
						motor[MecMotor_FR] = 0;
						motor[MecMotor_BR] = 0;
						motor[Lift] = 0;
						restMecMotors();
						currentState = STATE_END;
					}
					break;
				case STATE_END:
					//motor[Lift] = 15;
					servoSetNonCont(Bucket, servoDefinitions[Bucket].maxValue);

					//if (motorGetEncoder(Lift) >= -500) {
						motor[Lift] = 0;
						restMecMotors();
						motorResetAllEncoders(desiredEncVals);
						end = true;
					//}

					break;
			}
		}
	//writeDebugStream("DMV: %d %d %d %d\n", desiredMotorVals.power[MecMotor_FL], desiredMotorVals.power[MecMotor_BL], desiredMotorVals.power[MecMotor_FR], desiredMotorVals.power[MecMotor_BR]);
	//writeDebugStream("DEV: %d %d %d %d\n", desiredEncVals.encoder[MecMotor_FL], desiredEncVals.encoder[MecMotor_BL], desiredEncVals.encoder[MecMotor_FR], desiredEncVals.encoder[MecMotor_BR]);
	//writeDebugStream("VIRTENC: %d %d %d %d\n", motorGetEncoder((tMotor) MecMotor_FL), motorGetEncoder((tMotor) MecMotor_BL), motorGetEncoder((tMotor) MecMotor_FR), motorGetEncoder((tMotor) MecMotor_BR));
	//writeDebugStream("REALENC: %d %d %d %d\n", nMotorEncoder[Donatello_FL], nMotorEncoder[Leonardo_BL], nMotorEncoder[Michelangelo_FR], nMotorEncoder[Raphael_BR]);
	//writeDebugStream("REALMOTOR: %d %d %d %d\n", motor[MecMotor_FL], motor[MecMotor_BL], motor[MecMotor_FR], motor[MecMotor_BR]);
	//writeDebugStream("Lift target: %d\n", desiredEncVals.encoder[Lift]);
	//writeDebugStream("Lift enc: %d\n", motorGetEncoder(Lift));
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
