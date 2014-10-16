#include "Headers\Motor.h"
#include "Headers\Servo.h"
#include "JoystickDriver.c"

task main()
{
	waitForStart();
	motorSetSafePower(Motor_Mec_FR, 100);
	servoSetAngle(Servo_Tube, 75);
}
