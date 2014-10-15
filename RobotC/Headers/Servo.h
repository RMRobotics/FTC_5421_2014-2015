#ifndef SERVO_H
#define SERVO_H
#pragma systemFile

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
	if (speed > SERVO_CONT_FORWARD) {
		speed = SERVO_CONT_FORWARD;
	} else if (speed < SERVO_CONT_REVERSE) {
		speed = SERVO_CONT_REVERSE;
	}
	servo[currentServo] = speed;
}

void servoSetAngle(TServoIndex currentServo, int angle, int servoRate = SERVO_DEFAULT_RATE) {
	//Defaults to no minimum/maximums
	int minAngle = -1;
	int maxAngle = 256;
	//Init counter to (length of servo array - 1)
	int i = (sizeof(servoDefinitions)/sizeof(servoDefinitions[0])) - 1;

	//TODO see if this causes performance issues
	for (i=i; i>=0; i--) {
		if (servoDefinitions[i].servoID == currentServo) {
			minAngle = servoDefinitions[i].minAngle;
			maxAngle = servoDefinitions[i].maxAngle;
			break;
		}
	}
	if (angle > maxAngle) {
		//TODO DEBUG output "angle past maximum angle!"
		angle = maxAngle;
	} else if (angle < minAngle) {
		angle = minAngle;
	}
	servoChangeRate[currentServo] = servoRate;
	servo[currentServo] = angle;
}

#endif
