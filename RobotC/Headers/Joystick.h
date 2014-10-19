#ifndef JOYSTICK_H
#define JOYSTICK_H
#pragma systemFile

//Information on the TJoystick struct can be found in JoystickDriver.c

//Button numbers for bitmask
//               _==7==_                               _==8==_
//              / __5__ \                             / __6__ \.
//            +.-'_____'-.---------------------------.-'_____'-.+
//           /   |  0  |  '.                       .'  |     |   \.
//          / ___| /|\ |___ \                     / ___|(Y/4)|___ \.
//         / |   7  |  1   | ;   __        __    ; |             | ;
//         | |6<---   --->2| |  |__|      |__|   | |(X/1)   (B/3)| |
//         | |___5  |  3___| ; BACK/9   START/10 ; |___       ___| ;
//         |\    | \|/ |    /  _     ___      _   \    |(A/2)|    /|
//         | \   |__4__|  .','" "', |___|  ,'" "', '.  |_____|  .' |
//         |  '-.______.-' /       \ MODE /       \  '-._____.-'   |
//         |               |       |------|       |                |
//         |              /\       /      \       /\               |
//         |             /  '.___.'        '.___.'  \              |
//         |            /                            \             |
//          \          /                              \           /
//           \________/                                \_________/
// NOTE: ASCII Art adapted from: http://chris.com/ascii/index.php?art=video%20games/other

#include "JoystickDriver.c"

//Enum to assign button numbers meaningful names
typedef enum JoyButtons {
	BUTTON_X = 1,
	BUTTON_A = 2,
	BUTTON_B = 3,
	BUTTON_Y = 4,
	BUTTON_LB = 5, //Left bumper
	BUTTON_RB = 6, //Right bumper
	BUTTON_LT = 7, //Left trigger
	BUTTON_RT = 8, //Right trigger
	BUTTON_BACK = 9,
	BUTTON_START = 10,
} JoyButtons;

//Enum to assign tophat ("d-pad") numbers meaningful names
//Uses compass directions
typedef enum JoyTophat {
	TOPHAT_N = 0,
	TOPHAT_NE = 1,
	TOPHAT_E = 2,
	TOPHAT_SE = 3,
	TOPHAT_S = 4,
	TOPHAT_SW = 5,
	TOPHAT_W = 6,
	TOPHAT_NW = 7,
} JoyTophat;

//Updates 'joystick' struct to latest bluetooth message
//For proper code encapsulation, this function should be here
void joyUpdateJoystickSettings() {
	getJoystickSettings(joystick);
}

//Returns pointer to 'joystick' struct
TJoystick *joyGetJoystickPointer() {
	return &joystick;
}

//Checks if button is pressed in joystick #(joyNum)
//Note that joyNum will be interpreted as joystick #1 if passed 0,
//and joystick #2 otherwise
bool joyButtonPressed(int joyNum, JoyButtons joyButton) {
	if(joyNum) { //C interprets an int as TRUE if int != 0
		return (bool)joy1Btn((int)joyButton); //JoystickDriver defined function
	} else {
		return (bool)joy2Btn((int)joyButton); //JoystickDriver defined function
	}
}
#endif
