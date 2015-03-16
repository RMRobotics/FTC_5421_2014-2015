#pragma config(UserModel, "Headers/Pragmas.h")

#include "Headers/Motor.h"

DesiredMotorVals desiredMotorVals;
DesiredEncVals desiredEncVals;

TSemaphore PIDconstantSemaphore;

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
	semaphoreLock(PIDconstantSemaphore);

	//wait for semaphore lock on pid_kp, pid_ki, pid_kd
	if (bDoesTaskOwnSemaphore(PIDconstantSemaphore)) {

		hogCPU();

		int desiredPow = desiredMotorVals.power[MecMotor_FR];
		long PIDUpdateTimeMs = nPgmTime;
		long timeElapsedMs = PIDUpdateTimeMs - lastPIDUpdateTimeMs;
		int curEncoderVal = motorGetEncoder((tMotor) MecMotor_FR);
		float desiredRevSpeed = ((desiredPow/motorGetMaxReferencePower()) * MAX_ENCTICK_PER_MS);

		float currentRevSpeed = ((float) (curEncoderVal - lastEncoderVal)) / timeElapsedMs;
		writeDebugStream("%f, ", currentRevSpeed);

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
			//writeDebugStream("Desired value %d exceeds %d!\n", desiredMotorVals.power[MecMotor_FR], motorGetMaxReferencePower());
			desiredMotorVals.power[MecMotor_FR] = sgn(desiredMotorVals.power[MecMotor_FR]) * motorGetMaxReferencePower();
		}

		motorSetActualPowerToDesired(&desiredMotorVals);

		semaphoreUnlock(PIDconstantSemaphore);
		releaseCPU();

		wait1Msec(updateTimeLengthMs);
	}
}

task main()
{
	clearDebugStream();
	writeDebugStreamLine("This is PIDTest\n");
	memset(&desiredEncVals, 0, sizeof(desiredEncVals));
	motorInit(&desiredEncVals);
	semaphoreInitialize(PIDconstantSemaphore);

	startTask(PID);

	long loopStartTimeMs = nPgmTime;

	semaphoreLock(PIDconstantSemaphore);

	for (pid_kp=0; pid_kp<5; pid_kp++) {
		for (pid_ki=0; pid_ki<0; pid_ki+=0.01) {
			for (pid_kd=0; pid_kd<0; pid_kd++) {
				writeDebugStream("%f, %f, %f, ", pid_kp, pid_ki, pid_kd);

				if (bDoesTaskOwnSemaphore(PIDconstantSemaphore)) {
    			semaphoreUnlock(PIDconstantSemaphore);
    		}

  			while(motorGetEncoder((tMotor) MecMotor_FR) < 5000){
					hogCPU();

					motorUpdateState();
					desiredMotorVals.power[MecMotor_FR] = 50;

					releaseCPU();
				}

				semaphoreLock(PIDconstantSemaphore);
				motor[MecMotor_FR] = 0; //can do this because we have lock on semaphore
				writeDebugStream("Changing constants!\n");
				wait1Msec(1000); //wait for motor to spin down
			}
		}
	}
}
