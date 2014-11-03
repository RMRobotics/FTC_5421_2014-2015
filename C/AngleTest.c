#include <stdio.h>
#include <math.h>

#define M_PI 3.141592654

//Helper function for calculating ABSOLUTE maximum of two floats
//Will return maximum absolute value (ignores sign)
float helpFindMaxAbsFloat(float a, float b) {
	float aAbs = fabs(a);
	float bAbs = fabs(b);
	if (aAbs > bAbs) {
		return aAbs;
	} else {
		return bAbs;
	}
}

//Helper function for calculating ABSOLUTE minimum of two floats
//Will return minimum absolute value (ignores sign)
float helpFindMinAbsFloat(float a, float b) {
	float aAbs = fabs(a);
	float bAbs = fabs(b);
	if (aAbs < bAbs) {
		return aAbs;
	} else {
		return bAbs;
	}
}

//Returns sign of float
int helpFindSign(float x) {
	if (x > 0.0) {
		return 1;
	} else if (x < 0.0) {
		return -1;
	} else {
		return 0;
	}
}


int helpRoundFloat(float x) {
	if (x > 0.0) {
		return (int)(x+0.5);
	} else {
		return (int)(x-0.5);
	}
}

void drive(int angle, float powerRatio, float rotationRatio) {
	//TODO check parameter fit constraints

	//Holds max motor powers
	float maxPowFLBR = cos((45.0 - (float)angle) * M_PI / 180.0);
	float maxPowFRBL = cos((45.0 + (float)angle) * M_PI / 180.0);
 
	float powFL = (powerRatio * maxPowFLBR) + (rotationRatio * fabs(maxPowFLBR));
	float powBL = (powerRatio * maxPowFRBL) + (rotationRatio * fabs(maxPowFRBL));
	float powFR = (powerRatio * maxPowFRBL) - (rotationRatio * fabs(maxPowFRBL));
	float powBR = (powerRatio * maxPowFLBR) - (rotationRatio * fabs(maxPowFLBR));
  
	//Cap motor values 
	powFL = helpFindSign(powFL) * helpFindMinAbsFloat(powFL, maxPowFLBR);
	powBL = helpFindSign(powBL) * helpFindMinAbsFloat(powBL, maxPowFRBL);
	powFR = helpFindSign(powFR) * helpFindMinAbsFloat(powFR, maxPowFRBL);
	powBR = helpFindSign(powBR) * helpFindMinAbsFloat(powBR, maxPowFLBR);

	//Holds max reference power
	float maxRefPow = 100.0;
    
	//Scale to max reference power
	float absHighestPow = helpFindMaxAbsFloat(maxPowFLBR, maxPowFRBL);
	float multiplier = maxRefPow / absHighestPow;

	float scaledPowFL = powFL * multiplier;
	float scaledPowBL = powBL * multiplier;
	float scaledPowFR = powFR * multiplier;
	float scaledPowBR = powBR * multiplier;

	printf("Scaled Power FL: %d\n", helpRoundFloat(scaledPowFL));
	printf("Scaled Power BL: %d\n", helpRoundFloat(scaledPowBL));
	printf("Scaled Power FR: %d\n", helpRoundFloat(scaledPowFR));
	printf("Scaled Power BR: %d\n", helpRoundFloat(scaledPowBR));
}

int angle(int x, int y) {
	float atand = atan2(y,x) * 180.0 / M_PI;
	int angle = (int) atand;
	//printf("Atan2: %f\n", atand);
	//printf("Atan2 (int): %d\n", angle);
	if (angle >= 0 && angle <= 90) {
		printf("aaa");
		angle = 90 - angle;
	} else if (angle > 90 && angle <= 180) {
		printf("bbb");
		angle = 450 - angle;
	} else if (angle < 0 && angle >= -90) {
		printf("ccc");
		angle = fabs(angle) + 90;
	} else {
		printf("ddd");
		angle = fabs(angle) + 90;
	}
	return angle;
}


int main() {
	for (int i=0; i<360; i+=15) {
		int x = cos(i * M_PI / 180.0) * 10.0;
		int y = sin(i * M_PI / 180.0) * 10.0;
		int angleVal = angle(x, y);
		printf("Angle for X:%d, Y:%d = %d\n", x, y, angleVal);
		drive(angleVal, 1.0, 1.0);
	}
	return 0;
}
