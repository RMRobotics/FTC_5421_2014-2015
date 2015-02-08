#ifndef JOYHARVESTER_H
#define JOYHARVESTER_H
#pragma systemFile

//127 is no power, 255 is max forward, 0 is max backwards

void clearServos(){
	servoSetCont(HarvesterWinch,127);
}

void stopOpen(){
	servoSetNonCont(HarvesterStop, 27);
}

void stopClose(){
	servoSetNonCont(HarvesterStop, 255);
}

void winchDown(){
	servoSetCont(HarvesterWinch, 0);
}

void winchUp(){
	servoSetCont(HarvesterWinch, 255);
}

#endif
