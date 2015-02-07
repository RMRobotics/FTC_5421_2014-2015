#ifndef JOYHARVESTER_H
#define JOYHARVESTER_H
#pragma systemFile

//127 is no power, 255 is max forward, 0 is max backwards

void clearServos(){
	servoSetCont(HarvesterWinch,127);
}

void stopOpen(){
	servoSetCont(HarvesterStop, 0);
}

void stopClose(){
	servoSetCont(HarvesterStop, 255);
}

void winchDown(){
	servoSetCont(HarvesterWinch, 100);
}

void winchUp(){
	servoSetCont(HarvesterWinch, 100);
}

#endif
