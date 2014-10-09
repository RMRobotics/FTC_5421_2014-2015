#ifndef JOYSTICK_H
#define JOYSTICK_H
#pragma systemFile



#include "..\Libraries\Joystick.c"
#endif // JOYSTICK_H

/*
int   Math_TrimDeadband(int input, int deadband=g_JoystickDeadband);
float Math_TrimDeadband(float input, float deadband=g_JoystickDeadband);
float Math_Normalize(float input, float originalMax, float newMax);

int   Math_TrimDeadband(int input, int deadband) {
	int output = 0;
	if (abs(input)>deadband==true) {
		output = input;
	}
	return output;
}
float Math_TrimDeadband(float input, float deadband) {
	float output = 0;
	if (abs(input)>deadband==true) {
		output = input;
	}
	return output;
}
float Math_Normalize(float input, float originalMax, float newMax) {
	return input*newMax/originalMax;
}*/
