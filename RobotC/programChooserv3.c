#pragma config(Motor,  motorA,          front,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          backRight,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          backLeft,      tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//define variables

int drive = 0;
int onRamp = 0;
int teamDelay = 0;
int optional1 = 0;
int optional2 = 0;

int startVar[5]; //startVar[0-4]
string autoActions[5]; //autoActions[0-4]

void programChooserInit(){
	startVar[0] = drive;
	startVar[1] = onRamp;
	startVar[2] = teamDelay;
	startVar[3] = optional1;
	startVar[4] = optional2;
	autoActions[0] = "drive forward";
	autoActions[1] = "on ramp";
	autoActions[2] = "delay";
	autoActions[3] = "optional";
	autoActions[4] = "optional2";
}

int changeState(int addVar,string action){
	int moveOn = 0;
	while(moveOn == 0){
		nxtDisplayString(1, "%s: %d", action,addVar);
		if (nNxtButtonPressed == 1){      //right arrow
					addVar++;
					addVar = addVar%2;
				}else if (nNxtButtonPressed == 2){ //left arrow
					addVar++;
					addVar = addVar%2;
				}
			if (nNxtButtonPressed == 3){  //orange button
				moveOn++;
			}
		}
		return addVar;
}

//Drive forward: int: 1 is true and 0 is false
//On Ramp: int: 1 is true and 0 is false
//delay: int: 1 is true and 0 is false

task main()
{
	programChooserInit();
	while(true){
		eraseDisplay();
		for(int i = 0; i<5;i++){
			startVar[i] = changeState(startVar[i], autoActions[i]);
			nxtDisplayClearTextLine(1);
			wait10Msec(50);
		}
		wait10Msec(5000);
	}
}
