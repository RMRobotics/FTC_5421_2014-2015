#ifndef SERVO_H
#define SERVO_H
#pragma systemFile

////////////////////////////////////////////////////////////////////////////////////////
/*A note on array indexing:
Arrays can be accessed by converting the tServoindex enum to an int to get the array
index. Here's the rationale for determining array size.
RobotC caps the # of servos to kNumbOfRealServos. We have no idea how RobotC generates the
tServoIndex enum, so assume the worst and make the size of the array kNumbOfRealServos.
*/
////////////////////////////////////////////////////////////////////////////////////////

//Continuous rotation servo constants
#define SERVO_CONT_STOP 127
#define SERVO_CONT_REVERSE 0
#define SERVO_CONT_FORWARD 255

//Non continuous rotation servo max
#define MAX_NONCONT_ANGLE 255

//Servo default change rate constant
#define SERVO_DEFAULT_RATE 10

//Maximum number of servos
#define MAX_NUM_SERVOS (int)kNumbOfRealServos

//Structure for storing max/min values of servos and servo type
typedef struct ServoData {
	bool isContinuous; //if it is continuous rotation, set to true
	int minValue;
	int maxValue;
} ServoData;

//Servo array for storing all servos
static ServoData servoDefinitions[MAX_NUM_SERVOS];

//Flag for seeing if servoDefinitions has been initialized
bool servoDefsInitialized = false;

void servoInit() {
	//Initialize all servos as if they were non continuous
	for (int i=0; i < MAX_NUM_SERVOS; i++) {
		servoDefinitions[i].isContinuous = false;
		servoDefinitions[i].minValue = 0;
		servoDefinitions[i].maxValue = MAX_NONCONT_ANGLE;
	}

	servoDefinitions[Servo_Tube].isContinuous = true;
	servoDefinitions[Servo_Tube].minValue = 15;
	servoDefinitions[Servo_Tube].maxValue = 255;
	servoDefsInitialized = true;
}

/*Sets speed for a continuous rotation servo. If given servo is a
  non-continuous rotation servo, then it debugs and does nothing. */
void servoSetCont(TServoIndex currentServo, int speed) {
	if (!servoDefsInitialized) {
		//TODO DEBUG "servodefs not initialized!"
		return;
	}
	if (servoDefinitions[currentServo].isContinuous) {
		if (speed > SERVO_CONT_FORWARD) {
			speed = SERVO_CONT_FORWARD;
			} else if (speed < SERVO_CONT_REVERSE) {
			speed = SERVO_CONT_REVERSE;
		}
		servo[currentServo] = speed;
	} else {
		//TODO DEBUG "Attempting to send continuous speed to non continuous servo!"
	}
}

/*Sets angle for a non-continuous rotation servo. If given servo is
  a continuous rotation servo, then it debugs and does nothing. */
void servoSetNonCont(TServoIndex currentServo, int angle, int servoRate = SERVO_DEFAULT_RATE) {
	if (!servoDefsInitialized) {
		//TODO DEBUG "servodefs not initialized!"
		return;
	}
	if (!servoDefinitions[currentServo].isContinuous) {
		int maxAngle = servoDefinitions[currentServo].maxValue;
		int minAngle = servoDefinitions[currentServo].minValue;
		if (angle > maxAngle) {
			//TODO DEBUG output "angle past maximum angle!"
			angle = maxAngle;
		} else if (angle < minAngle) {
			//TODO DEBUG output "angle past minimum angle!"
			angle = minAngle;
		}
		servoChangeRate[currentServo] = servoRate;
		servo[currentServo] = angle;
	} else  { //Continuous rotation servo
		//TODO DEBUG output "Trying to send noncontinuous value to continuous rotation servo!
	}
}

#endif
