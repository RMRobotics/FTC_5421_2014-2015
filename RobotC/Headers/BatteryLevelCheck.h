void explodingSound(){
		PlaySound(soundShortBlip);
		wait10MSec(10);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundBlip);
		wait10MSec(10);
		PlaySound(soundShortBlip);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		wait10MSec(10);
		PlaySound(soundShortBlip);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		wait10MSec(10);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		wait10MSec(10);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		wait10MSec(10);
		PlaySound(soundShortBlip);
		PlaySound(soundShortBlip);
		wait10MSec(10);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
		wait10MSec(10);
		PlaySound(soundBlip);
		PlaySound(soundBlip);
		PlaySound(soundShortBlip);
}

void batteryPowerCheck(){
	int nBatteryAverage = nAvgBatteryLevel; //Is an average of nxt reading from last 20 checks
	float nBatAvgVolts = nBatteryAverage/1000; //convert from milivolts to volts
	int nBatteryLevel = nImmediateBatteryLevel; //Displays the most current nxt battery reading
	float nBatActualVolts = nBatteryLevel/1000; //Converts the battery reading from milivolts to volts

	int lowNxtPower = 6; //CHANGE THIS WHEN LOW BATTERY LEVEL IS KNOWN

	int extBatAvgVolts = externalBatteryAvg/1000; //avg external battery average and converts from milivolts to volts
	int extBatActualVolts = externalBattery/1000; //actual external battery average and converts from milivolts to volts

	int lowExternalPower = 12; //CHANGE THIS WHEN LOW BATTERY LEVEL IS KNOWN

	if(nBatAvgVolts < lowNxtPower && nBatActualVolts < lowNxtPower){
		writeDebugStream("Replace NXT batteries!: Voltage: %d\n", nBatActualVolts);
		//. -..- .--. .-.. --- -.. .. -. --.
		explodingSound();
	}
	if(extBatAvgVolts < lowExternalPower && extBatActualVolts < lowExternalPower){
		writeDebugStream("Replace external batteries!: Voltage: %d\n", extBatActualVolts);
		//. -..- .--. .-.. --- -.. .. -. --.
		explodingSound();
	}
}

