#ifndef JOYSTICK_H
#define JOYSTICK_H
#pragma systemFile

typedef enum JoystickButton {
	BUTTON_A		= 2,
	BUTTON_B		= 3,
	BUTTON_X		= 1,
	BUTTON_Y		= 4,
	BUTTON_LB		= 5,
	BUTTON_RB		= 6,
	BUTTON_LT		= 7,
	BUTTON_RT		= 8,
	BUTTON_BACK		= 9,
	BUTTON_START	= 10,
	BUTTON_JOYL		= 11,
	BUTTON_JOYR		= 12,
	BUTTON_NUM,
} JoystickButton;

#endif // JOYSTICK_H