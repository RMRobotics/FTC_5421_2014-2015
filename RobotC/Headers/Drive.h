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

   Consider the front left mecanum wheel (ALL TRIG IS DONE IN DEGREES):
   Target: (0 degree, 1.0 speed, 1.0 rotation):
			A: Target vector
			B: FL Mecanum vector
			T: Variable (in this case 45 degrees)

			A       B
			|      _
			|    _
			|T _
			|_	45 degrees

			Projection of A onto B (because we want to find component of total speed
			in direction of mecanum wheel) = speed * cos(T)
				= 1.0 * cos(45) = cos(45)

			Since we're going forward, you can extrapolate and we get cos(45) for all four wheels.

			Now we need to add rotation, but rotation is now at a different maximum (1.0) than the
			wheels (cos(45))!

			So scale rotation by the maximum of the wheels and then add:
				= cos(45) + 1.0(cos(45)/1.0) = 2cos(45) = relative_FL_power

			Note that this is not final power! The highest mecmotor would be set to 2cos(45)
			in this scenario, so we need to scale by the absolute highest power of the four motors.
			We will assume max_power = 100:
				full_FL_power = (max_power/(abs_highest_power_of_four_motors)) * relative_FL_power
					= (100 / (cos(45)+1.0)) * (cos(45)+1.0) = 100

  Generalization:
			A: Target vector
			B: FL Mecanum vector
			a: Angle
			T: Variable

			     A    B
			    `    _
			a  `T  _
			  `  _
			 ` _
			`_   45 deg

			Projection of A onto B = speed * cos(T)
			  = speed * cos(90 - a - 45) = speed * cos(45 - a)

			Scale and add rotation:
				powerBeforeRot = speed * cos(45-a)
				power = powerBeforeRot + rotation*highest_of_four_powerBeforeRot

   By the same reasoning, we get the following four formulas for relative_power:
      MotorMec_FL = (speed * cos(45 - a) + rotation*highest_of_four_powerBeforeRot
      MotorMec_BL = (speed * cos(45 + a) + rotation*highest_of_four_powerBeforeRot
      MotorMec_FR = (speed * cos(45 + a) - rotation*highest_of_four_powerBeforeRot
      MotorMec_BR = (speed * cos(45 - a) - rotation*highest_of_four_powerBeforeRot

   Note that MotorMec_FL = MotorMec_BR and MotorMec_BL = MotorMec_FR!
   This should make sense if you think about the direction that each mecmotor applies force in.

   Then scale all powers by: max_power/abs_highest_power_of_four_powers

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


//Set desired motor values based on polar coordinates and rotation:
//  angle: int value of angle IN DEGREES from 0 to 360
//  speed: float value from 0 to 1
//  rotation: float value from -1 to 1
void driveSetMecMotorPolarDegrees(DesiredMotorVals *desiredMotorVals, int angle,
												 float speed, float rotation) {
	//TODO check parameter fit constraints

	//Holds cosine value for FL and BR power calculations
  float cosFLBR = cosDegrees(45.0 - (float)angle);
  //Holds cosine value for FR and BL power calculations
  float cosFRBL = cosDegrees(45.0 + (float)angle);
  //Holds max power
  float maxPow = (float) motorGetMaxReferencePower();

  //Formulae
	float unscaledPowBefRotFL = (speed * cosFLBR);
	float unscaledPowBefRotBL = (speed * cosFRBL);
	float unscaledPowBefRotFR = unscaledPowBefRotBL;
  float unscaledPowBefRotBR = unscaledPowBefRotFL;

  //Scale and add rotation
  float absHighestPowBefRot = helpFindMaxAbsFloat(unscaledPowBefRotFL,
  																								unscaledPowBefRotBL);
  float scaledRotation = rotation * absHighestPowBefRot;
  float unscaledPowFL = unscaledPowBefRotFL + scaledRotation;
	float unscaledPowBL = unscaledPowBefRotBL + scaledRotation;
	float unscaledPowFR = unscaledPowBefRotFR - scaledRotation;
  float unscaledPowBR = unscaledPowBefRotBR - scaledRotation;

  //Scale to maxPower
	float absHighestPow = helpFindMaxAbsFloat(unscaledPowFL, unscaledPowBL,
																							unscaledPowFR, unscaledPowBR);
	float multiplier = maxPow / absHighestPow;

	float scaledPowFL = unscaledPowFL * multiplier;
	float scaledPowBL = unscaledPowBL * multiplier;
	float scaledPowFR = unscaledPowFR * multiplier;
	float scaledPowBR = unscaledPowBR * multiplier;

	desiredMotorVals->power[MecMotor_FL] = helpRoundFloat(scaledPowFL);
	desiredMotorVals->power[MecMotor_BL] = helpRoundFloat(scaledPowBL);
	desiredMotorVals->power[MecMotor_FR] = helpRoundFloat(scaledPowFR);
	desiredMotorVals->power[MecMotor_BR] = helpRoundFloat(scaledPowBR);
}

//Zero all mecanum motors
void driveZeroMecMotor(DesiredMotorVals *desiredMotorVals) {
	desiredMotorVals->power[MecMotor_FL] = 0;
	desiredMotorVals->power[MecMotor_BL] = 0;
	desiredMotorVals->power[MecMotor_FR] = 0;
	desiredMotorVals->power[MecMotor_BR] = 0;
}
#endif
