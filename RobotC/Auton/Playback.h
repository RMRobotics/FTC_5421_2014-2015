#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "Headers\JoyPlayMusic.h"

//Enumerates all states in this autonomous program
typedef enum PlaybackStateNames {
	STATE_BEGIN,
	STATE_RUN,
	STATE_END,
} PlaybackStateNames;

static PlaybackStateNames currentState = STATE_BEGIN;

/* State machine for playback autonomous.
   This will play back recorded joystick commands.
   isEnd will be set to true when state machine
   has finished. */
void playbackRun() {
	bool end = false;
	while(!end) {
		switch(currentState) {
			case STATE_BEGIN:
				if (dataOpenReadJoyLog()) {
					currentState = STATE_RUN;
				} else {
					currentState = STATE_END;
				}
				break;
			case STATE_RUN:
				TJoystick *curJoy = dataReadJoystickLog();
				if (curJoy) {
					joyplaymusicPlay(curJoy);
				} else {
					currentState = STATE_END;
				}
				wait1Msec(50);
				break;
			case STATE_END:
				dataCloseJoyLog();
				end = true;
				break;
			default:
				writeDebugStream("Invalid state (default)!");
				break;
		}
	}
}
#endif
