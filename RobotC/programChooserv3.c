#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Hubs,  S3, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  mtr_S3_C1_1,     Michelangelo_FR, tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C1_2,     Donatello_FL,  tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C2_1,     Raphael_BR,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C2_2,     Leonardo_BL,   tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S3_C3_1,     HarvesterMove, tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S3_C3_2,     motor0,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S3_C4_1,     Lift,					 tmotorTetrix, openLoop, encoder, reversed)
#pragma config(Motor,  mtr_S3_C4_2,     Harvester,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    servo1,        tServoNone)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,        tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,        tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4, 			 tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    TubeGrabber,   tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    Bucket,        tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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
#include "AutonFxn\test.h"
#include "Auton\Auton.h"

//define variables
bool startVar[5]; //startVar[0-4] only 5 functions
string autoActions[5]; //autoActions[0-4]

typedef enum Chooser{
	BLOCK,
	RAMP,
	DELAY,
	CENTER,
	MEDIUM,
}Chooser;

void programChooserInit(){
	startVar[BLOCK] = false;
	startVar[RAMP] = false;
	startVar[DELAY] = false;
	startVar[CENTER] = false;
	startVar[MEDIUM] = false;
	autoActions[0] = "block";
	autoActions[1] = "on ramp";
	autoActions[2] = "delay";
	autoActions[3] = "center";
	autoActions[4] = "medTube";
}

bool changeState(bool addVar,string action){
	bool moveOn = false;
	while(!moveOn){
		nxtDisplayString(2, "%s: %b", action,addVar);
		if (nNxtButtonPressed == 1){      //right arrow
					addVar = !addVar;
				}else if (nNxtButtonPressed == 2){ //left arrow
					addVar = !addVar;
				}
			if (nNxtButtonPressed == 3){  //orange button
				moveOn = !moveOn;
			}
		}
		return addVar;
}

//Drive forward: int: 1 is true and 0 is false
//On Ramp: int: 1 is true and 0 is false
//delay: int: 1 is true and 0 is false

task main()
{
	writeDebugStream("This is ProgramChooser\n");
	programChooserInit();
	initialize();
	eraseDisplay();
	for(int i = 0; i<sizeof(Chooser)/2;i++){
		startVar[i] = changeState(startVar[i], autoActions[i]);
		nxtDisplayClearTextLine(2);
		wait10Msec(50);
	}
	nxtDisplayString(2, "IT WORKS");
	//wait10Msec(5000);
	if (startVar[BLOCK] == 0){	// no block
		if (startVar[RAMP] == 0){	//no block, not on ramp
			if (startVar[DELAY] == 0){	//no block, not on ramp, no delay
				if (startVar[CENTER] == 0){	//no block, not on ramp, no delay, no center tube
					if (startVar[MEDIUM] == 0){	//no block, not on ramp, no delay, no center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //no block, not on ramp, no delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){
					if (startVar[MEDIUM] == 0){	//no block, not on ramp, no delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //no block, not on ramp, no delay, center tube, medium tube

					}
				}
			}else if(startVar[DELAY] == 1){ //delay
				if (startVar[CENTER] == 0){	//center tube
					if (startVar[MEDIUM] == 0){	//no block, not on ramp, delay, no center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){//no block, not on ramp, delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){ //center tube
					if (startVar[MEDIUM] == 0){	//no block, not on ramp, delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //no block, not on ramp, delay, center tube, medium tube

					}
				}
			}
		}else if(startVar[RAMP] == 1){ //onramp
			if (startVar[DELAY] == 0){	//no block, on ramp, no delay
				if (startVar[CENTER] == 0){	//no block, on ramp, no delay, no center tube
					if (startVar[MEDIUM] == 0){	//no block, on ramp, no delay, no center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //no block, on ramp, no delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){
					if (startVar[MEDIUM] == 0){	//no block, on ramp, no delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //no block, on ramp, no delay, center tube, medium tube

					}
				}
			}else if(startVar[DELAY] == 1){ //delay
				if (startVar[CENTER] == 0){	//center tube
					if (startVar[MEDIUM] == 0){	//no block, on ramp, delay, no center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){//no block, on ramp, delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){ //center tube
					if (startVar[MEDIUM] == 0){	//no block, on ramp, delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //no block, on ramp, delay, center tube, medium tube

					}
				}
			}
		}
	}else if(startVar[BLOCK] == 1){	//block
		if (startVar[RAMP] == 0){	//block, not on ramp
			if (startVar[DELAY] == 0){	//block, not on ramp, no delay
				if (startVar[CENTER] == 0){	//block, not on ramp, no delay, no center tube
					if (startVar[MEDIUM] == 0){	//block, not on ramp, no delay, no center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //block, not on ramp, no delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){
					if (startVar[MEDIUM] == 0){	//block, not on ramp, no delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //block, not on ramp, no delay, center tube, medium tube

					}
				}
			}else if(startVar[DELAY] == 1){ //delay
				if (startVar[CENTER] == 0){	//center tube
					if (startVar[MEDIUM] == 0){	//block, not on ramp, delay, no center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){//block, not on ramp, delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){ //center tube
					if (startVar[MEDIUM] == 0){	//block, not on ramp, delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //block, not on ramp, delay, center tube, medium tube

					}
				}
			}
		}else if(startVar[RAMP] == 1){ //onramp
			if (startVar[DELAY] == 0){	//block, on ramp, no delay
				if (startVar[CENTER] == 0){	//block, on ramp, no delay, no center tube
					if (startVar[MEDIUM] == 0){	//block, on ramp, no delay, no center tube, no medium tube
						BlockCubix(&desiredMotorVals, &desiredEncVals);
					}else if(startVar[MEDIUM] == 1){ //block, on ramp, no delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){
					if (startVar[MEDIUM] == 0){	//block, on ramp, no delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //block, on ramp, no delay, center tube, medium tube

					}
				}
			}else if(startVar[DELAY] == 1){ //delay
				if (startVar[CENTER] == 0){	//center tube
					if (startVar[MEDIUM] == 0){	//block, on ramp, delay, no center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){//block, on ramp, delay, no center tube, medium tube

					}
				}else if(startVar[CENTER] == 1){ //center tube
					if (startVar[MEDIUM] == 0){	//block, on ramp, delay, center tube, no medium tube

					}else if(startVar[MEDIUM] == 1){ //block, on ramp, delay, center tube, medium tube

					}
				}
			}
		}
	}
}
