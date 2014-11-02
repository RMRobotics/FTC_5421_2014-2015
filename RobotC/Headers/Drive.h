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
        |_  45 degrees

        Projection of A onto B (because we want to find component of total speed
        in direction of mecanum wheel) = speed * cos(T)
            = 1.0 * cos(45) = cos(45)

        Note that this gives a relative_power < 1.0 for going straight! All four
        mecmotors would be set to cos(45) in this scenario, so we need to scale.
        We will assume max_power = 100:
            full_power = (max_power/(max_of_mecmotors_b4_rotation)) * 1.0 = 100

        Add scaled rotation:
            = 100 + rotation*max_power = 200

        We can do this because we bound the power in motorSetActualToDesired (so
        in reality this would translate to 100).

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

        power = ((max_power/(max_of_mecmotors_b4_rotation)) * speed * cos(45-a)) + (rotation * max_power)

   By the same reasoning, we get the following four formulas for power:
      MotorMec_FL = (speed * cos(45 - a) * (max_power/(max_of_mecmotors_b4_rotation))) + (rotation * max_power)
      MotorMec_BL = (speed * cos(45 + a) * (max_power/(max_of_mecmotors_b4_rotation))) + (rotation * max_power)
      MotorMec_FR = (speed * cos(45 + a) * (max_power/(max_of_mecmotors_b4_rotation))) - (rotation * max_power)
      MotorMec_BR = (speed * cos(45 - a) * (max_power/(max_of_mecmotors_b4_rotation))) - (rotation * max_power)

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
	//Holds cosine value for FL and BR power calculations
  float cosFLBR = cosDegrees(45 - (float)angle);
  //Holds cosine value for FR and BL power calculations
  float cosFRBL = cosDegrees(45 + (float)angle);
  //Holds max drive power
  float maxDPower = (float) motorGetMaxDrivePower();

	float unscaledPowBefRotFL = maxDPower * cosFLBR;
	float unscaledPowBefRotBL = maxDPower * cosFRBL;
	float unscaledPowBefRotFR = unscaledPowBefRotBL;
  float unscaledPowBefRotBR = unscaledPowBefRotFL;

	float maxPowBeforeRot = helpFindMaxAbsFloat(unscaledPowBefRotFL, unscaledPowBefRotBL);
	float multiplier = maxDPower / maxPowBeforeRot;
	float scaledRot = rotation * maxDPower;

	float scaledPowRotFL = (unscaledPowBefRotFL * multiplier) + scaledRot;
	float scaledPowRotBL = (unscaledPowBefRotBL * multiplier) + scaledRot;
	float scaledPowRotFR = (unscaledPowBefRotFR * multiplier) - scaledRot;
	float scaledPowRotBR = (unscaledPowBefRotBR * multiplier) - scaledRot;

	desiredMotorVals->power[MecMotor_FL] = (int)scaledPowRotFL;
	desiredMotorVals->power[MecMotor_BL] = (int)scaledPowRotBL;
	desiredMotorVals->power[MecMotor_FR] = (int)scaledPowRotFR;
	desiredMotorVals->power[MecMotor_BR] = (int)scaledPowRotBR;
}
#endif
