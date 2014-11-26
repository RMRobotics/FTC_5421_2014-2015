#ifndef JOYPLAYMUSIC_H
#define JOYPLAYMUSIC_H
#pragma systemFile

#include "Joystick.h"

/*This function interprets a TJoystick struct and plays music.*/
void joyplaymusicPlay(TJoystick *joyState) {
	int toneDuration = 10; //in 10ms
	if (joyButtonPressed(joyState, 0, BUTTON_X)) {
		PlayTone(440, toneDuration);
	} else if (joyButtonPressed(joyState, 0, BUTTON_Y)) {
		PlayTone(392, toneDuration);
	} else if (joyButtonPressed(joyState, 0, BUTTON_A)) {
		PlayTone(349, toneDuration);
	} else if (joyButtonPressed(joyState, 0, BUTTON_B)) {
		PlayTone(329, toneDuration);
	} else if (joyButtonPressed(joyState, 0, BUTTON_LB)) {
		PlayTone(293, toneDuration);
	} else if (joyButtonPressed(joyState, 0, BUTTON_RB)) {
		PlayTone(261, toneDuration);
	} else if (joyButtonPressed(joyState, 0, BUTTON_LT)) {
		PlayTone(246, toneDuration);
	} else if (joyButtonPressed(joyState, 0, BUTTON_RT)) {
		PlayTone(220, toneDuration);
	}
}

#endif
