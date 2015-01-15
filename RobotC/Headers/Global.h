#ifndef GLOBAL_H
#define GLOBAL_H
#pragma systemFile

////////////////////////////////////////////////////////////////////////////////////////
/*A note on array indexing:
Arrays can be accessed by converting the tMotor enum to an int to get the array
index. Here's the rationale for determining array size.
Although FTC caps the number of motors to 8 12v DC motors + 3 NXT motors,
RobotC caps it to kNumbOfTotalMotors. We have no idea how RobotC generates the tMotor
enum, so assume the worst and make the size of the array kNumbOfTotalMotors.
*/
////////////////////////////////////////////////////////////////////////////////////////

//Use this timer as a global timer.
//DO NOT CLEAR THIS TIMER. Use a different timer if you need a clearable one.
TTimers SystemTimer = T1;

#define MAX_NUM_MOTORS ((int)kNumbOfTotalMotors)


//Enum for referencing mecanum motors
//They're named after TMNT because Lisa.
//TODO place in config file. this is forced to be here because pragmas can't be included...
//VOLATILE
typedef enum MecMotor {
	MecMotor_FL = Donatello_FL,
	MecMotor_BL = Leonardo_BL,
	MecMotor_FR = Michelangelo_FR,
	MecMotor_BR = Raphael_BR,
} MecMotor;

//Struct definition for desired encoder limit
typedef struct DesiredEncVals {
	int encoder[MAX_NUM_MOTORS];
} DesiredEncVals;

//Struct definition for desired motor powers
typedef struct DesiredMotorVals {
	int power[MAX_NUM_MOTORS];
} DesiredMotorVals;


#endif
