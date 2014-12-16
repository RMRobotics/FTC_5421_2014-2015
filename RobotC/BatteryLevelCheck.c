#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     Donatello_FL,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     Leonardo_BL,   tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Chute_Left,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     Slides_Left,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     Michelangelo_FR, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C1_2,     Raphael_BR,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     Chute_Right,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     Slides_Right,  tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    KickStand,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    TubeGrabber,          tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    Bucket_Right,         tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    Bucket_Left,          tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


void explodingSound(){
		PlaySound(soundShortBlip);
		wait10Msec(10);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundBlip);
		wait10Msec(10);
		PlaySound(soundShortBlip);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		wait10Msec(10);
		PlaySound(soundShortBlip);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		wait10Msec(10);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		wait10Msec(10);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		wait10Msec(10);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		wait10Msec(10);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		wait10Msec(10);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
}

task main(){
	int nBatteryAverage = nAvgBatteryLevel; //Is an average of nxt reading from last 20 checks
	float nBatAvgVolts = ((float) nBatteryAverage)/1000.0; //convert from milivolts to volts
	int nBatteryLevel = nImmediateBatteryLevel; //Displays the most current nxt battery reading
	float nBatActualVolts = ((float) nBatteryLevel)/1000.0; //Converts the battery reading from milivolts to volts

	float lowNxtPower = 6; //CHANGE THIS WHEN LOW BATTERY LEVEL IS KNOWN

	float extBatAvgVolts = ((float) externalBatteryAvg)/1000.0; //avg external battery average and converts from milivolts to volts
	float extBatActualVolts = ((float) externalBattery)/1000.0; //actual external battery average and converts from milivolts to volts

	float lowExternalPower = 12; //CHANGE THIS WHEN LOW BATTERY LEVEL IS KNOWN

	writeDebugStream("NXT batteries: Voltage: %f\n", nBatActualVolts);
	writeDebugStream("External batteries: Voltage: %f\n", extBatActualVolts);
	nxtDisplayTextLine(2, "NXT bat (V) %f\n", nBatActualVolts);
		nxtDisplayTextLine(5, "Ext bat (V) %f\n", extBatActualVolts);
	if(nBatAvgVolts < lowNxtPower && nBatActualVolts < lowNxtPower){
		writeDebugStream("Replace NXT batteries!: Voltage: %f\n", nBatActualVolts);
				nxtDisplayTextLine(2, "Repl NXT bat! (V): %f\n", nBatActualVolts);
		//. -..- .--. .-.. --- -.. .. -. --.
		explodingSound();
	}
	if(extBatAvgVolts < lowExternalPower && extBatActualVolts < lowExternalPower){
		writeDebugStream("Replace external batteries!: Voltage: %f\n", extBatActualVolts);
		nxtDisplayTextLine(5, "Repl ext bat! (V): %f\n", extBatActualVolts);
		//. -..- .--. .-.. --- -.. .. -. --.
		explodingSound();

	}
	while(true) {}
}
