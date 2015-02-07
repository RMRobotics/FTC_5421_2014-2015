#ifndef JOYHARVESTER_H
#define JOYHARVESTER_H
#pragma systemFile

void clearServos(){
	servoSetCont(HarvesterStop, 0);
	servoSetCont(HarvesterWinch,0);
}

void stopOpen(){
	servoSetCont(HarvesterStop, 100);
}

void stopClose(){
	servoSetCont(HarvesterStop, -100);
}

void winchDown(){
	servoSetCont(HarvesterWinch, 100);
}

void winchUp(){
	servoSetCont(HarvesterWinch, 100);
}

#endif
