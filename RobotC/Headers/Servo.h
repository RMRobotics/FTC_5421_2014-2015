#ifndef SERVO_H
#define SERVO_H
#pragma systemFile

//Continuous rotation servo constants
#define SERVO_CONT_STOP 127
#define SERVO_CONT_REVERSE 0
#define SERVO_CONT_FORWARD 255

//Servo default change rate constant
#define SERVO_DEFAULT_RATE 10

//Number of noncontinous rotation servos
#define NUM_NONCONT_SERVOS 1

//Structure for storing max/min angles of servos.
typedef struct ServoData {
	TServoIndex servoID;
	int minAngle;
	int maxAngle;
} ServoData;

//Non continuous rotation servo array for setting max/min angles
static ServoData servoDefinitions[NUM_NONCONT_SERVOS];

//Flag for seeing if servoDefinitions has been initialized
bool servoDefsInitialized = false;

void servoInit() {
	servoDefinitions[0].servoID = Servo_Tube;
	servoDefinitions[0].minAngle = 15;
	servoDefinitions[0].maxAngle = 255;
	servoDefsInitialized = true;
}

/*Sets speed for a continuous rotation servo. If given servo is a
  non-continuous rotation servo, then it debugs and does nothing. */
void servoSetContinuousSpeed(TServoIndex currentServo, int speed) {
	if (speed > SERVO_CONT_FORWARD) {
		speed = SERVO_CONT_FORWARD;
		} else if (speed < SERVO_CONT_REVERSE) {
		speed = SERVO_CONT_REVERSE;
	}
	servo[currentServo] = speed;
}

/*Sets angle for a non-continuous rotation servo. If given servo is
  a continuous rotation servo, then it debugs and does nothing. */
void servoSetAngle(TServoIndex currentServo, int angle, int servoRate = SERVO_DEFAULT_RATE) {
	if (!servoDefsInitialized) {
		//TODO DEBUG "noncontinuous rotation servos not initialized!"
		return;
	}

	//Defaults to no minimum/maximums
	int minAngle = -1;
	int maxAngle = 256;
	//Init counter to (length of servo array - 1)
	int i = NUM_NONCONT_SERVOS - 1;

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
