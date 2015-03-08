#pragma config(UserModel, "Headers/Pragmas.h")

#include "Headers/Motor.h"

DesiredMotorVals desiredMotorVals;
DesiredEncVals desiredEncVals;

int updateTimeLengthMs = 50;

//1100 ticks per revolution
long lastPIDUpdateTimeMs = 0;
int lastEncoderVal = 0;
float PIDerror = 0;

float MAX_ENCTICK_PER_MS = 8.0;

int PID_INTEGRAL_LIMIT = 20; //must be greater than 2*MAX_ENCTICK_PER_MS
float accumulatedError = 0;

float lastPIDError = 0;

float pid_kp = 2.0;
float pid_ki = 0.04;
float pid_kd = 0.1;

task PID {
	hogCPU();

	int desiredPow = desiredMotorVals.power[MecMotor_FR];
	long PIDUpdateTimeMs = nPgmTime;
	long timeElapsedMs = PIDUpdateTimeMs - lastPIDUpdateTimeMs;
	int curEncoderVal = motorGetEncoder((tMotor) MecMotor_FR);
	float desiredRevSpeed = ((desiredPow/motorGetMaxReferencePower()) * MAX_ENCTICK_PER_MS);

	float currentRevSpeed = ((float) (curEncoderVal - lastEncoderVal)) / timeElapsedMs;
	writeDebugStream("FR speed: %f\n", currentRevSpeed);

	PIDerror = currentRevSpeed - desiredRevSpeed;

	//Proportion
	//When the motor's state suddenly changes (like we move off of the low friction ramp to the higher friction
	//mat tiles), this term makes sure the motor reacts quickly.
	//This means that pid_kp should be much greater than pid_ki.
	float proportionTerm = pid_kp * PIDerror;

	//Integral
	//When the motor is running in a constant state, this term accounts for small errors and corrects for them.
	accumulatedError += PIDerror;

	if (abs(accumulatedError) < PID_INTEGRAL_LIMIT) {
		accumulatedError = sgn(accumulatedError) * PID_INTEGRAL_LIMIT;
	}
	float integralTerm = pid_ki * accumulatedError;

	//Derivative
	//This is mostly used for fine tuning, such as to reduce oscillation.
	float derivativeTerm = pid_kd * (PIDerror - lastPIDError);

	lastPIDUpdateTimeMs = PIDUpdateTimeMs;
	lastEncoderVal = curEncoderVal;
	lastPIDError = PIDerror;

	//Calculate amount to change desiredPow by to get desired speed
	desiredMotorVals.power[MecMotor_FR] = desiredPow + (proportionTerm + integralTerm + derivativeTerm);

	//Limit power
	if (abs(desiredMotorVals.power[MecMotor_FR]) > motorGetMaxReferencePower()) {
		writeDebugStream("Desired value %d exceeds %d!\n", desiredMotorVals.power[MecMotor_FR], motorGetMaxReferencePower());
		desiredMotorVals.power[MecMotor_FR] = sgn(desiredMotorVals.power[MecMotor_FR]) * motorGetMaxReferencePower();
	}

	motorSetActualPowerToDesired(&desiredMotorVals);

	releaseCPU();
	wait1Msec(updateTimeLengthMs);
}

task main()
{
	clearDebugStream();
	writeDebugStreamLine("This is PIDTest\n");
	memset(&desiredEncVals, 0, sizeof(desiredEncVals));
	motorInit(&desiredEncVals);

	startTask(PID);

	long loopStartTimeMs = nPgmTime;

	while(true){
		hogCPU();

		loopStartTimeMs = nPgmTime;
		motorUpdateState();

		int desiredFR = 10;

		if(motorGetEncoder((tMotor) MecMotor_FR) > 9000){
			desiredFR = 0;
		}
		desiredMotorVals.power[MecMotor_FR] = desiredFR;

		writeDebugStream("%d\n", motorGetEncoder((tMotor) MecMotor_FR));
		writeDebugStream("Loop time ms: %d\n", nPgmTime - loopStartTimeMs);
		wait1Msec(1000);

		releaseCPU();
	}
}
