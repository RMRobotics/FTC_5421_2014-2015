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

void initialize(){
	clearDebugStream();
	writeDebugStream("This is ParkGrabMedTube\n");
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
	//SLIDE SIDE IS FACING TOWARD THE CENTERPIECE
	long tubeGrabStartTimeMs = 0;

	initialize();
	waitForStart();

	while(!end){
		long loopStartTimeMs = nPgmTime;
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
					driveSetEncoderRotateCW(&desiredEncVals, 0.10 * ENC_PER_REV);
					currentState = STATE_TURNTOWARDTUBE;
					break;
				case STATE_TURNTOWARDTUBE:
					writeDebugStream("State: Turn Toward Tube\n");
					driveSetMecMotorRotateCW(&desiredMotorVals, 11.0);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						driveSetEncoderN(&desiredEncVals, 6.0 * ENC_PER_REV); //for next state
						motorSetEncoder(&desiredEncVals, Lift, -SIXTY_GOAL, true); //for next state
						currentState = STATE_DRIVETOWARDTUBE;
					}
					break;
				case STATE_DRIVETOWARDTUBE:
					writeDebugStream("State: Drive Toward Tube\n");

					//ONLY LIMIT LIFT
					desiredMotorVals.power[Lift] = -100;
					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);

					driveSetMecMotorN(&desiredMotorVals, 0.75);

					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(driveMecHasHitEncoderTarget(&desiredEncVals)) { //keep driving
						servoSetNonCont(TubeGrabber, servoDefinitions[TubeGrabber].minValue);
						tubeGrabStartTimeMs = nPgmTime;
					}

					if ((nPgmTime - tubeGrabStartTimeMs) > 500) {
						motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals); //cut drive
						restMecMotors();
						currentState = STATE_WAITFORLIFT;
					}
					break;
				case STATE_WAITFORLIFT:
					writeDebugStream("State: WAIT FOR LIFT\n");

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(motorAllHitEncoderTarget(&desiredEncVals)) { //wait for lift
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
					driveSetEncoderS(&desiredEncVals, 6.0 * ENC_PER_REV); //for next state
					motorSetEncoder(&desiredEncVals, Lift, 0, true); //for next state
					currentState = STATE_DRIVETOWARDPZONE;
				case STATE_DRIVETOWARDPZONE:
					writeDebugStream("State: Drive toward Pzone\n");
					desiredMotorVals.power[Lift] = 100;
					driveSetMecMotorS(&desiredMotorVals, 0.75);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if(driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						driveSetEncoderRotateCW(&desiredEncVals, 1.25 * ENC_PER_REV);
						currentState = STATE_ROTTOWARDPZONE;
					}
					break;
				case STATE_ROTATETUBE:
					writeDebugStream("State: Rotate Tube\n");
					driveSetMecMotorRotateCW(&desiredMotorVals, -1.0);

					motorLimitDesiredPowerToEncoder(&desiredMotorVals, &desiredEncVals);
					motorSetActualPowerToDesired(&desiredMotorVals);
					motorUpdateState();

					if (driveMecHasHitEncoderTarget(&desiredEncVals)) {
						restMecMotors();
						currentState = STATE_END;
					}
					break;
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
