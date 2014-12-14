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


task main(){
	while(true){
		eraseDisplay();
		if(nNxtButtonPressed == 1){ //right arrow
			motor[Slides_Left] = 50;
			motor[Slides_Right] = 50;
		}else if (nNxtButtonPressed == 2){//left arrow
			motor[Slides_Left] = -50;
			motor[Slides_Right] = -50;
		}else {
			motor[Slides_Left] = 0;
			motor[Slides_Right] = 0;
		}
	}
}