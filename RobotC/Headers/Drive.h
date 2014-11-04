#ifndef DRIVE_H
#define DRIVE_H
#pragma systemFile

#include "Motor.h"
#include "Helper.h"


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
		for rightside motors (remember, we choose movement over rotation):
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
	//TODO check parameter fit constraints

	writeDebugStreamLine("Drive power: %f", powerRatio);

	//Holds max motor powers
	float maxPowFLBR = cosDegrees(45.0 - (float)angle);
	float maxPowFRBL = cosDegrees(45.0 + (float)angle);

	float powFL = (powerRatio * maxPowFLBR) + (rotationRatio * abs(maxPowFLBR));
	float powBL = (powerRatio * maxPowFRBL) + (rotationRatio * abs(maxPowFRBL));
	float powFR = (powerRatio * maxPowFRBL) - (rotationRatio * abs(maxPowFRBL));
	float powBR = (powerRatio * maxPowFLBR) - (rotationRatio * abs(maxPowFLBR));

	writeDebugStreamLine("Relative power: %f, %f, %f, %f", powFL, powBL, powFR, powBR);

	//Cap motor values
	powFL = helpFindSign(helpRoundFloat(powFL)) * helpFindMinAbsFloat(powFL, maxPowFLBR);
	powBL = helpFindSign(helpRoundFloat(powBL)) * helpFindMinAbsFloat(powBL, maxPowFRBL);
	powFR = helpFindSign(helpRoundFloat(powFR)) * helpFindMinAbsFloat(powFR, maxPowFRBL);
	powBR = helpFindSign(helpRoundFloat(powBR)) * helpFindMinAbsFloat(powBR, maxPowFLBR);

	//Holds max reference power
	float maxRefPow = (float) motorGetMaxReferencePower();

	//Scale to max reference power
	float absHighestPow = helpFindMaxAbsFloat(maxPowFLBR, maxPowFRBL);
	float multiplier = maxRefPow / absHighestPow;

	float scaledPowFL = powFL * multiplier;
	float scaledPowBL = powBL * multiplier;
	float scaledPowFR = powFR * multiplier;
	float scaledPowBR = powBR * multiplier;

	desiredMotorVals->power[MecMotor_FL] = helpRoundFloat(scaledPowFL);
	desiredMotorVals->power[MecMotor_BL] = helpRoundFloat(scaledPowBL);
	desiredMotorVals->power[MecMotor_FR] = helpRoundFloat(scaledPowFR);
	desiredMotorVals->power[MecMotor_BR] = helpRoundFloat(scaledPowBR);

	//Drive debug
	writeDebugStreamLine("Drive: %f, %f, %f, %f", scaledPowFL, scaledPowBL, scaledPowFR, scaledPowBR);
}

//Zero all mecanum motors
void driveZeroMecMotor(DesiredMotorVals *desiredMotorVals) {
	desiredMotorVals->power[MecMotor_FL] = 0;
	desiredMotorVals->power[MecMotor_BL] = 0;
	desiredMotorVals->power[MecMotor_FR] = 0;
	desiredMotorVals->power[MecMotor_BR] = 0;
}
#endif
