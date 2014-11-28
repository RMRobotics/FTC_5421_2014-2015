#ifndef DRIVE_H
#define DRIVE_H
#pragma systemFile

#include "Motor.h"
#include "Helper.h"

#define ROTATION_ENCODER_RATIO 5 //Ratio of degree of rotation to encoder unit
#define ORBIT_ENCODER_RATIO 10 //Ratio of degree of orbit to encoder unit

////////////////////////////////////////////////////////////////////////////////////////
/* A note on math:
   Mecanum wheels can best be understood as applying force at 45 degrees to the robot:

                     / ______ \
                    / |      | \
                      |      |
                      |      |
                    \ |______| /
                     \        /

   Applying full power to one mecanum wheel will cause the robot to rotate while
   moving diagonally.

   We can take polar coordinates+rotation as our goal for our movement of
   the entire robot. The goal of this is to convert those polar coordinates
   into power for each of the four wheels.

   Before we proceed, note that there exists variation between implementations because
   you must decide at some point how much power to allocate to moving and how much power
   to allocate to rotation. For example, you can split power 50/50, which would result
   in the motors exerting maximum force only when moving while rotating.

   This implementation decides to allocate all power to moving because in FTC it is rare
   that you need to move while rotating, and any remaining power is allocated to rotation.

   Now here's an example.

   Target: (0 degree, 1.0 powerRatio, 0.5 rotationRatio):
      This means that we want to go straight, at maximum power and half CW rotation.

      Consider the FL mecmotor:

				A: Target vector
				B: FL Mecanum vector
				T: Variable (in this case 45 degrees)

				A       B
				|      _
				|    _
				|T _
				|_	45 degrees

			To find maximum power of motor:
			Scalar projection of unit vector of A onto B (because we want to find max component of
			total velocity in direction of mecanum wheel) = cos(T)
				= cos(90 - 45)

		We can generalize this process and derive formulas for all four motors:

	  For FL and BR mecanum:
		  A: Target vector
			B: Mecanum vector
			c: Angle (given)
			T: Variable

			|     A    B
			|    `    -
			|c  `T  -
			|  `  -
			| ` - 45 degrees
			|`-_____________

		Scalar projection of unit vector of A onto B = cos(T)
		  = cos(90 - c - 45) = cos(45 - c)

		For BL and FR mecanum:
		  A: Target vector
			B: Mecanum vector
			c: Angle (given)
			d: 45 degrees (90 - 45 = 90)

			B				       A
			_				 |    `
				_			 |   `
					_	 d |c `
						_	 | `
			45 deg	_|`

		Scalar projection of unit vector of A onto B = cos(d+c)
		  = cos(45 + c) = cos(45 + c)


		Since we're going straight forward (c=0), you can calculate and we get a max_motor_power_direction
		of cos(45) for all four wheels. Keep in mind that SIGN MATTERS - it gives direction in this case.

		Now to find movement power, multiply maximum power by powerRatio:
		  move_power_FL = cos(45) * power = cos(45) * 1.0 = cos(45)
		  move_power_BL = cos(45) * power = cos(45) * 1.0 = cos(45)
		  .....

		Now we find rotation power by multiplying abs(maximum power) by rotationRatio (because
		we only want the MAGNITUDE of max_motor_power_direction, not direction. Direction is
		supplied by rotationRatio's sign):
		  rotation_power_FL = abs(cos(45)) * rotation = 0.5cos(45)
		  .....

		Now we add the two together for leftside motors and subtract for rightside motors:
		  total_power_FL = cos(45) + 0.5cos(45) = 1.5cos(45)
		  total_power_BL = cos(45) + 0.5cos(45) = 1.5cos(45)
		  total_power_FR = cos(45) - 0.5cos(45) = 0.5cos(45)
		  total_power_BR = cos(45) - 0.5cos(45) = 0.5cos(45)

		But we have exceeded our maximum power of cos(45)! So we cap it and effectively ignore rotation
		for leftside motors (remember, we choose movement over rotation):
		  capped_power_FL = cos(45)
		  capped_power_BL = cos(45)
		  capped_power_FR = 0.5cos(45)
		  capped_power_BR = 0.5cos(45)

		Note that this is not final power! The highest mecmotor would be set to cos(45)
		in this scenario, so we need to scale all of them to max_reference_power.
		We will assume max_reference_power = 100:
			scaled_power_FL = (max_reference_power/(max_of_all_four_max_motor_power)) * capped_power_FL
				= (100 / (cos(45)) * cos(45) = 100
		  ....

		We've got our scaled powers :).

		Remember to check your int/float conversions!

		Happy coding! :)

		More detailed explanations can be found here. Note that this tutorial does not reverse the rightside motors in
		the #pragmas as we have, and it also does not handle rotation and multiplication in the same way:
      Part 1: http://blog.elliotjb.com/2013/10/holonomic-x-drive-tutorial-theory-of.html?m=1
      Part 2: http://blog.elliotjb.com/2013/10/holonomic-x-drive-tutorial-programming.html
      Part 3: http://blog.elliotjb.com/2013/10/holonomic-x-drive-tutorial-programming_31.html
      Part 4: http://blog.elliotjb.com/2013/11/holonomic-x-drive-tutorial-writing-test.html?m=1
*/
////////////////////////////////////////////////////////////////////////////////////////


/*Set desired motor values based on polar coordinates and rotation:
  angle: int value of angle IN DEGREES from 0 to 360
  powerRatio: float value from 0 to 1
  rotationRatio: float value from -1 to 1 */
void driveSetMecMotorPolarDegrees(DesiredMotorVals *desiredMotorVals, int angle,
float powerRatio, float rotationRatio) {
	if (angle > 360 || angle < 0 || powerRatio > 1.0 || rotationRatio > 1.0) {
		writeDebugStream("Drive parameters do not fit constraints! Ang: %d, Pow: %f, Rot: %f",
										 angle, powerRatio, rotationRatio);
	} else {
		//Holds max motor powers
		float maxPowFLBR = cosDegrees(45.0 - (float)angle);
		float maxPowFRBL = cosDegrees(45.0 + (float)angle);

		float powFL = (powerRatio * maxPowFLBR) + (rotationRatio * abs(maxPowFLBR));
		float powBL = (powerRatio * maxPowFRBL) + (rotationRatio * abs(maxPowFRBL));
		float powFR = (powerRatio * maxPowFRBL) - (rotationRatio * abs(maxPowFRBL));
		float powBR = (powerRatio * maxPowFLBR) - (rotationRatio * abs(maxPowFLBR));

		//Cap motor values
		powFL = sgn(powFL) * helpFindMinAbsFloat(powFL, maxPowFLBR);
		powBL = sgn(powBL) * helpFindMinAbsFloat(powBL, maxPowFRBL);
		powFR = sgn(powFR) * helpFindMinAbsFloat(powFR, maxPowFRBL);
		powBR = sgn(powBR) * helpFindMinAbsFloat(powBR, maxPowFLBR);

		nxtDisplayString(3, "PW:%f %f %f %f", powFL, powBL, powFR, powBR);

		//Holds max reference power
		float maxRefPow = (float) motorGetMaxReferencePower();

		//Scale to max reference power
		float absHighestPow = helpFindMaxAbsFloat(maxPowFLBR, maxPowFRBL);
		float multiplier = maxRefPow / absHighestPow;

		float scaledPowFL = powFL * multiplier;
		float scaledPowBL = powBL * multiplier;
		float scaledPowFR = powFR * multiplier;
		float scaledPowBR = powBR * multiplier;

		desiredMotorVals->power[MecMotor_FL] = round(scaledPowFL);
		desiredMotorVals->power[MecMotor_BL] = round(scaledPowBL);
		desiredMotorVals->power[MecMotor_FR] = round(scaledPowFR);
		desiredMotorVals->power[MecMotor_BR] = round(scaledPowBR);
	}
}

/* Zero all mecanum motors */
void driveZeroMecMotor(DesiredMotorVals *desiredMotorVals) {
	desiredMotorVals->power[MecMotor_FL] = 0;
	desiredMotorVals->power[MecMotor_BL] = 0;
	desiredMotorVals->power[MecMotor_FR] = 0;
	desiredMotorVals->power[MecMotor_BR] = 0;
}

/* Resets all encoders */
void driveResetEncoders(DesiredMotorVals *desiredMotorVals) {
	for (int i=0;i<NUM_MOTORS;i++) {
		nMotorEncoder[motorList[i]] = 0;
	}
}

/*Sets desired motor values in order to orbit north (forwards).
	The center of rotation is in front of the robot.
	powerRatio: float value from 0 to 1 */
void driveSetMecMotorOrbitN(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 90, powerRatio, -powerRatio);
}

/*Sets desired encoder values in order to orbit north.
  The center of rotation is in front of the robot.
  angle: int value of angle to rotate in degrees */
void driveSetEncoderOrbitN(DesiredEncVals *desiredEncVals, int angle) {
	desiredEncVals->encoder[MecMotor_BL] = angle * -ORBIT_ENCODER_RATIO;
	desiredEncVals->encoder[MecMotor_BR] = angle * ORBIT_ENCODER_RATIO;
}

/*Sets desired motor values in order to orbit south (backwards).
  The center of rotation is behind the robot.
  powerRatio: float value from 0 to 1 */
void driveSetMecMotorOrbitS(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 90, powerRatio, powerRatio);
}

/*Sets desired encoder values in order to orbit south.
  The center of rotation is behind the robot.
  angle: int value of angle to rotate in degrees */
void driveSetEncoderOrbitS(DesiredEncVals *desiredEncVals, int angle) {
	desiredEncVals->encoder[MecMotor_FL] = angle * ORBIT_ENCODER_RATIO;
	desiredEncVals->encoder[MecMotor_FR] = angle * -ORBIT_ENCODER_RATIO;
}

/*The following functions follow the same format as above.
  encoderDistance: distance to go in encoder units */
void driveSetMecMotorN(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 0, 1.0, 0.0);
}
void driveSetEncoderN(DesiredEncVals *desiredEncVals, int encoderDistance) {
	desiredEncVals->encoder[MecMotor_FL] = encoderDistance;
	desiredEncVals->encoder[MecMotor_BL] = encoderDistance;
	desiredEncVals->encoder[MecMotor_FR] = encoderDistance;
	desiredEncVals->encoder[MecMotor_BR] = encoderDistance;
}

void driveSetMecMotorS(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 180, powerRatio, 0.0);
}
void driveSetEncoderS(DesiredEncVals *desiredEncVals, int encoderDistance) {
	driveSetEncoderN(desiredEncVals, -encoderDistance);
}

void driveSetMecMotorE(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 90, powerRatio, 0.0);
}
void driveSetEncoderE(DesiredEncVals *desiredEncVals, int encoderDistance) {
	desiredEncVals->encoder[MecMotor_FL] = encoderDistance;
	desiredEncVals->encoder[MecMotor_BL] = -encoderDistance;
	desiredEncVals->encoder[MecMotor_FR] = -encoderDistance;
	desiredEncVals->encoder[MecMotor_BR] = encoderDistance;
}

void driveSetMecMotorW(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 270, powerRatio, 0.0);
}
void driveSetEncoderW(DesiredEncVals *desiredEncVals, int encoderDistance) {
	driveSetEncoderE(desiredEncVals, -encoderDistance);
}

void driveSetMecMotorNE(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 45, powerRatio, 0.0);
}
void driveSetEncoderNE(DesiredEncVals *desiredEncVals, int encoderDistance) {
	desiredEncVals->encoder[MecMotor_FL] = encoderDistance;
	desiredEncVals->encoder[MecMotor_BR] = encoderDistance;
}

void driveSetMecMotorNW(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 315, powerRatio, 0.0);
}
void driveSetEncoderNW(DesiredEncVals *desiredEncVals, int encoderDistance) {
	desiredEncVals->encoder[MecMotor_FR] = encoderDistance;
	desiredEncVals->encoder[MecMotor_BR] = encoderDistance;
}

void driveSetMecMotorSE(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 135, powerRatio, 0.0);
}
void driveSetEncoderSE(DesiredEncVals *desiredEncVals, int encoderDistance) {
	driveSetEncoderNW(desiredEncVals, -encoderDistance);
}

void driveSetMecMotorSW(DesiredMotorVals *desiredMotorVals, float powerRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 225, powerRatio, 0.0);
}
void driveSetEncoderSW(DesiredEncVals *desiredEncVals, int encoderDistance) {
	driveSetEncoderNE(desiredEncVals, -encoderDistance);
}

void driveSetMecMotorRotate(DesiredMotorVals *desiredMotorVals, float rotationRatio) {
	driveSetMecMotorPolarDegrees(desiredMotorVals, 0, 0, rotationRatio);
}
void driveSetEncoderRotate(DesiredEncVals *desiredEncVals, int angle) {
	desiredEncVals->encoder[MecMotor_FL] = angle * ROTATION_ENCODER_RATIO;
	desiredEncVals->encoder[MecMotor_BL] = angle * ROTATION_ENCODER_RATIO;
	desiredEncVals->encoder[MecMotor_FR] = -angle * ROTATION_ENCODER_RATIO;
	desiredEncVals->encoder[MecMotor_BR] = -angle * ROTATION_ENCODER_RATIO;
}

#endif
