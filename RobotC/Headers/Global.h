#ifndef GLOBAL_H
#define GLOBAL_H
#pragma systemFile


#include "drivers/hitechnic-sensormux.h"
#include "drivers/hitechnic-irseeker-v2.h"
#include "drivers/hitechnic-gyro.h"
#include "drivers/lego-ultrasound.h"

//Structs for sensors
tHTIRS2 irSeeker;
tHTGYRO gyroSensor;

// The sensor is connected to the first port
// of the SMUX which is connected to the NXT port S1.
// To access that sensor, we must use msensor_S1_1.  If the sensor
// were connected to 3rd port of the SMUX connected to the NXT port S4,
// we would use msensor_S4_3

// Give the sensor a nice easy to use name
const tMUXSensor GYRO = msensor_S4_1;
const tMUXSensor HTIRS2 = msensor_S4_4;
const tMUXSensor LEGOUS = msensor_S4_3;


////////////////////////////////////////////////////////////////////////////////////////
/*A note on array indexing:
Arrays can be accessed by converting the tMotor enum to an int to get the array
index. Here's the rationale for determining array size.
Although FTC caps the number of motors to 8 12v DC motors + 3 NXT motors,
RobotC caps it to kNumbOfTotalMotors. We have no idea how RobotC generates the tMotor
enum, so assume the worst and make the size of the array kNumbOfTotalMotors.
*/
////////////////////////////////////////////////////////////////////////////////////////

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
	long encoder[MAX_NUM_MOTORS];
	bool encoderCapEnabled[MAX_NUM_MOTORS]; //if we want to use encoder as hard stop
	bool encoderCapIsMax[MAX_NUM_MOTORS]; //if the encoder cap is a max or a min
} DesiredEncVals;


//Struct definition for desired motor powers
typedef struct DesiredMotorVals {
	int power[MAX_NUM_MOTORS];
} DesiredMotorVals;


#endif
