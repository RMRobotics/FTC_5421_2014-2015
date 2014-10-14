#ifndef SERVO_H
#define SERVO_H

//Number of non continuous rotation servos
#define NUM_NONCONT_SERVOS 1

//Continuous rotation servo constants
#define SERVO_CONT_STOP 127
#define SERVO_CONT_REVERSE 0
#define SERVO_CONT_FORWARD 255

//Servo default change rate constant
#define SERVO_DEFAULT_RATE 10

//Structure for storing max/min angles of servos.
typedef struct {
	TServoIndex servoID;
	int minAngle;
	int maxAngle;
} ServoData;

//Array for setting max/min angles
static ServoData servoArray[NUM_NONCONT_SERVOS] = {{Servo_Tube, 15, 255}};

void servoSetContinuousSpeed(TServoIndex currentServo, int speed) {
	speed = (speed > SERVO_CONT_FORWARD) ? SERVO_CONT_FORWARD : speed;
	speed = (speed < SERVO_CONT_REVERSE) ? SERVO_CONT_REVERSE : speed;
	servo[currentServo] = speed;
}

void servoSetAngle(TServoIndex currentServo, int angle, int servoRate = SERVO_DEFAULT_RATE) {
	//Defaults to no minimum/maximums
	int minAngle = -1;
	int maxAngle = 256;
	int i;
	for (i=NUM_NONCONT_SERVOS-1; i>=0; i--) { //TODO see if this causes performance issues
		if (servoArray[i].servoID == currentServo) {
			minAngle = servoArray[i].minAngle;
			maxAngle = servoArray[i].maxAngle;
			break;
		}
	}
	angle = (angle > maxAngle) ? maxAngle : angle;
	angle = (angle < minAngle) ? minAngle : angle;
	servoChangeRate[currentServo] = servoRate;
	servo[currentServo] = angle;
}

#endif
