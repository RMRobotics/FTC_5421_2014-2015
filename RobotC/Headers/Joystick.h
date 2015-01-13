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




//JoystickDriver.c does not work with PC emulator so we'll have to write code separate
//from that for the PC emulator

/*Robot version: Updates 'joystick' struct to latest bluetooth message
	PC Emulator version: do whatever debugging is necessary here
*/
void joyUpdateJoystickSettings();

/*Robot version: Calls waitForStart()
	PC Emulator version: do nothing
*/
void joyWaitForStart();

#if (defined(NXT) || defined(TETRIX)) && defined(_Target_Emulator_) //target the PC emulator
#warn "YOU ARE COMPILING FOR THE EMULATOR."
	typedef struct
	{
	  bool    UserMode;          // Autonomous or Telep-Operated mode
	  bool    StopPgm;           // Stop program

	  short   joy1_x1;           // -128 to +127
	  short   joy1_y1;           // -128 to +127
	  short   joy1_x2;           // -128 to +127
	  short   joy1_y2;           // -128 to +127
	  short   joy1_Buttons;      // Bit map for 12-buttons
	  short   joy1_TopHat;       // value -1 = not pressed, otherwise 0 to 7 for selected "octant".

	  short   joy2_x1;           // -128 to +127
	  short   joy2_y1;           // -128 to +127
	  short   joy2_x2;           // -128 to +127
	  short   joy2_y2;           // -128 to +127
	  short   joy2_Buttons;      // Bit map for 12-buttons
	  short   joy2_TopHat;       // value -1 = not pressed, otherwise 0 to 7 for selected "octant".
	} TJoystick;

	TJoystick joystick;

	short joy1x1 = 0;
	short joy1y1 = 0;
	short joy1x2 = 0;
	short radius = 30;
	int angleDegree = 0;
	int radiusStep = 5;
	int angleStep = 10;
	int x2Step = 10;

	void joyUpdateJoystickSettings() {
		//Generates circles on joy1x1, generates linear sliding on joy1x2
		joy1x1 = cos(angleDegree * PI / 180.0) * radius;
		joy1y1 = sin(angleDegree * PI / 180.0) * radius;
		joy1x2 += x2Step;
		if (joy1x2 > 120 || joy1x2 < -115){
			x2Step = -x2Step;
		}
 		if (radius > 115 || radius < 26) { //Deadzone is somewhere around 20, and keep it below 127
			radiusStep = -radiusStep;
		}
		radius += radiusStep; // radius = radius + radiusStep
		angleDegree += angleStep;
		joystick.joy1_x1 = joy1x1;
		joystick.joy1_y1 = joy1y1;
		joystick.joy1_x2 = joy1x2;
	}

	void joyWaitForStart() { //Do nothing for PC emulator version
	}

#else
	#include "..\Drivers\JoystickDriver.c"
	void joyUpdateJoystickSettings() {
		getJoystickSettings(joystick);
	}
	//Normal version use waitForStart
	void joyWaitForStart() {
		waitForStart();
	}
#endif

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

/*Enum to assign tophat ("d-pad") numbers meaningful names
  Uses compass directions
*/
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

//Enum for joystick numbers (first and second)
typedef enum Joystick {
	JOY1 = 0,
	JOY2 = 1,
} Joystick;

/*Returns pointer to 'joystick' struct */
TJoystick *joyGetJoystickPointer() {
	return &joystick;
}

/*Checks if button is pressed in joystick #(joyNum)
  Note that joyNum will be interpreted as joystick #1 if passed 0,
  and joystick #2 otherwise */
bool joyButtonPressed(TJoystick *joyState, Joystick joyNum, JoyButtons joyButton) {
	if(joyNum == 0) {
		return ((joyState->joy1_Buttons & (1 << (joyButton - 1))) != 0);
	} else {
		return ((joyState->joy2_Buttons & (1 << (joyButton - 1))) != 0);
	}
}

/*Checks tophat button
*/
JoyTophat joyGetTophat(TJoystick *joyState) {
	return (JoyTophat) joyState->joy1_TopHat;
}
#endif
