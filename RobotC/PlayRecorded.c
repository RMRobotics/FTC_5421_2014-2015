#pragma config(UserModel, "Headers/Pragmas.h")

#include "Headers\Global.h"
#include "Headers\Motor.h"
#include "Headers\Servo.h"
#include "Headers\JoyAuxiliary.h"
#include "Headers\JoyMecanumDrive.h"
#include "Headers\JoyPlayMusic.h"
#include "Headers\Data.h"

//Enumerates all states in this autonomous program
typedef enum PlaybackStateNames {
	STATE_BEGIN,
	STATE_RUN,
	STATE_END,
} PlaybackStateNames;

static PlaybackStateNames currentState = STATE_BEGIN;


//Stores desired motor values
//DesiredMotorVals desiredMotorVals;
//Stores desired encoder values
//DesiredEncVals desiredEncVals;

void initialize() {
	clearDebugStream();
	writeDebugStream("This is PlayRecorded\n");
	//Initialize to zeroes
	/*memset(&desiredMotorVals, 0, sizeof(desiredMotorVals));
	memset(&desiredEncVals, 0, sizeof(desiredEncVals));
	motorInit();
	servoInit();*/
}
/*
void callAuxiliaryMotors(){
	joyLift(&desiredMotorVals, joyGetJoystickPointer());
	joyWing(&desiredMotorVals, joyGetJoystickPointer());
	joyHarvester(&desiredMotorVals, joyGetJoystickPointer());
	joyBucketDrop(&desiredMotorVals, joyGetJoystickPointer());
}*/

task main()
{
	initialize();
	joyWaitForStart();

	long pgmStartTimeMs = nPgmTime;

	int timeLengthMs = 5 * 1000;
	int delay = 50; //ms delay between joystick updates

	short fileSize = 0;
	TFileHandle fileHandle;
	string fileName = JOY_LOGFILE;

	bool end = false;
	while(!end) {
		switch(currentState) {
			case STATE_BEGIN:
				if (dataOpenRead(&fileHandle, fileName, &fileSize)) {
					currentState = STATE_RUN;
				} else {
					currentState = STATE_END;
				}
				break;
			case STATE_RUN:
				TJoystick *curJoy;
				if (dataReadTJoystick(&fileHandle, curJoy)) {
					/*joymecdriveSetDesiredPower(&desiredMotorVals, curJoy);
					callAuxiliaryMotors();
					motorSetActualPowerToDesired(&desiredMotorVals);*/
					joyplaymusicPlay(curJoy);
					wait1Msec(delay);
					if ((nPgmTime - pgmStartTimeMs) > timeLengthMs) {
						currentState = STATE_END;
					}
				} else {
					currentState = STATE_END;
				}
				break;
			case STATE_END:
				dataClose(&fileHandle);
				end = true;
				break;
			default:
				writeDebugStream("Invalid state (default)!");
				break;
		}
	}
}
