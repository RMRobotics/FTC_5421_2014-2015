#ifndef TEST_H
#define TEST_H
#pragma systemFile

void BlockCubix(DesiredMotorVals *desiredMotorVals,DesiredEncVals *desiredEncVals){
	joyWaitForStart();
	while(true){
		time1[T1] = 0; //in ms
		driveSetMecMotorN(desiredMotorVals, 1.0);
		while (time1[T1] < 2250) {
			motorSetActualPowerToDesired(desiredMotorVals);
			writeDebugStream("Driving forward!\n");
		}
		driveZeroMecMotor(desiredMotorVals);
		while (time1[T1] < 3000) {
			motorSetActualPowerToDesired(desiredMotorVals);
			writeDebugStream("Stopping!\n");
		}
	}
}

#endif
