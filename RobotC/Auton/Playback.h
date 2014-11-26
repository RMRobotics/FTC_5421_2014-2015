#ifndef PLAYBACK_H
#define PLAYBACK_H

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
	bool end;
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
					motor[singleMotor] = round(((float)curJoy->joy1_y1 / 128.0)*100);
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
