#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     Donatello_FL,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     Leonardo_BL,   tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Chute_Left,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     Slides_Left,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     Michelangelo_FR, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C1_2,     Raphael_BR,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     Chute_Right,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     Slides_Right,  tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    KickStand,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

typedef enum Mode {
	LEFT,
	RIGHT,
	BOTH,
} Mode;


task main(){
	int currentModeIndex = 0;
	tMotor currentMotor = Slides_Left;
	tMotor currentMotor_2 = Slides_Left;

	while(true){
		eraseDisplay();
		if (nNxtButtonPressed == 3) { //orange button, 2 is left, 1 is right
			currentModeIndex++;
			if (currentModeIndex > 2) {
				currentModeIndex = 0;
			}
		}

		if (currentModeIndex == (int) LEFT) {
			nxtDisplayTextLine(1, "Moving left motor:");
			currentMotor = Slides_Left;
			currentMotor_2 = Slides_Left;
		} else if (currentModeIndex == (int) RIGHT) {
			nxtDisplayTextLine(1, "Moving right motor:");
			currentMotor = Slides_Right;
			currentMotor_2 = Slides_Right;
		} else {
			nxtDisplayTextLine(1, "Moving both motors:");
			currentMotor = Slides_Right;
			currentMotor_2 = Slides_Left;
		}

		if (nNxtButtonPressed == 2) { //left arrow
			motor[currentMotor] = -25;
			motor[currentMotor_2] = -25;
		} else if (nNxtButtonPressed == 1) { //right arrow
			motor[currentMotor] = 25;
			motor[currentMotor_2] = 25;
		} else {
			motor[currentMotor] = 0;
			motor[currentMotor_2] = 0;
		}
		nxtDisplayTextLine(4, "L: %d R: %d", motor[currentMotor], motor[currentMotor_2]);
		wait1Msec(100);
	}
}
