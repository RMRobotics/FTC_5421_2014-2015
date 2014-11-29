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
	if (servoDefsInitialized) {
		if (servoDefinitions[currentServo].isContinuous) {
			if (speed > SERVO_CONT_FORWARD) {
				speed = SERVO_CONT_FORWARD;
				} else if (speed < SERVO_CONT_REVERSE) {
				speed = SERVO_CONT_REVERSE;
			}
			servo[currentServo] = speed;
		} else {
			writeDebugStream("Attempting to set noncontinuous rotation servo (%d) continuously!\n", currentServo);
		}
	} else {
		writeDebugStream("Servos not initialized!\n");
	}
}

/*Sets angle for a non-continuous rotation servo. If given servo is
  a continuous rotation servo, then it debugs and does nothing. */
void servoSetNonCont(TServoIndex currentServo, int angle, int servoRate = SERVO_DEFAULT_RATE) {
	if (servoDefsInitialized) {
		if (!servoDefinitions[currentServo].isContinuous) {
			int maxAngle = servoDefinitions[currentServo].maxValue;
			int minAngle = servoDefinitions[currentServo].minValue;
			if (angle > maxAngle) {
				writeDebugStream("Angle (%d) past maximum angle (%d) for servo (%d)!\n", angle, maxAngle, currentServo);
				angle = maxAngle;
			} else if (angle < minAngle) {
				writeDebugStream("Angle (%d) past minimum angle (%d) for servo (%d)!\n", angle, minAngle, currentServo);
				angle = minAngle;
			}
			servoChangeRate[currentServo] = servoRate;
			servo[currentServo] = angle;
		} else  { //Continuous rotation servo
			writeDebugStream("Attempting to set continuous rotation servo (%d) noncontinuously!\n", currentServo);
		}
	} else {
		writeDebugStream("Servos not initialized!\n");
	}
}

/*Returns an int value of the last target set for the servo.
  If servoDefinitions is not initialized, returns -1. */
int servoReadTargetValue(TServoIndex currentServo) {
	if (servoDefsInitialized) {
		return ServoValue[currentServo];
	} else {
		writeDebugStream("Servos not initialized!\n");
		return -1;
	}
}

#endif
