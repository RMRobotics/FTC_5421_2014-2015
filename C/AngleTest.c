#include <stdio.h>
#include <math.h>

#define M_PI 3.141592654

//Helper function for calculating ABSOLUTE maximum of a list of floats
//This function ignores sign
float helpFindMaxAbsFloat(float a, float b, float c, float d) {
	float values[] = {a, b, c, d};
	float max = a;
	for (int z=1; z<(sizeof(values)/sizeof(float)); z++) {
		if (fabs(values[z]) > max) {
			max = fabs(values[z]);
		}
	}
	return max;
}

void drive(int angle, float speed, float rotation) {
	//TODO check parameter fit constraints

	//Holds cosine value for FL and BR power calculations
	float temp = (45.0 - (float)angle) * M_PI / 180.0;
	float cosFLBR = cos(temp);
	//Holds cosine value for FR and BL power calculations
	temp = (45.0 + (float)angle) * M_PI / 180.0;
	float cosFRBL = cos(temp);
	//Holds max drive power
	float maxDPower =  100.0;

	float unscaledPowBefRotFL = maxDPower * cosFLBR;
	float unscaledPowBefRotBL = maxDPower * cosFRBL;
	float unscaledPowBefRotFR = unscaledPowBefRotBL;
	float unscaledPowBefRotBR = unscaledPowBefRotFL;

	float maxPowBeforeRot = helpFindMaxAbsFloat(unscaledPowBefRotFL, unscaledPowBefRotBL);
	float multiplier = maxDPower * speed / maxPowBeforeRot;
	float scaledRot = rotation * maxDPower;
	
	printf("Multiplier: %f\n", multiplier);
	printf("Scaled rotation: %f\n", scaledRot);

	float scaledPowRotFL = (unscaledPowBefRotFL * multiplier) + scaledRot;
	float scaledPowRotBL = (unscaledPowBefRotBL * multiplier) + scaledRot;
	float scaledPowRotFR = (unscaledPowBefRotFR * multiplier) - scaledRot;
	float scaledPowRotBR = (unscaledPowBefRotBR * multiplier) - scaledRot;

	printf("Scaled Power FL: %f\n", scaledPowRotFL);
	printf("Scaled Power BL: %f\n", scaledPowRotBL);
	printf("Scaled Power FR: %f\n", scaledPowRotFR);
	printf("Scaled Power BR: %f\n", scaledPowRotBR);
}

int angle(int x, int y) {
	float atand = atan2(y,x) * 180.0 / M_PI;
	int angle = (int) atand;
	printf("Atan2: %f\n", atand);
	printf("Atan2 (int): %d\n", angle);
	if (angle >= 0 && angle <= 90) {
		printf("aaa");
		angle = 90 - angle;
	} else if (angle > 90 && angle <= 180) {
		printf("bbb");
		angle = 450 - angle;
	} else if (angle < 0 && angle >= -90) {
		printf("ccc");
		angle = -angle + 90;
	} else {
		printf("ddd");
		angle = -angle + 90;
	}
	return angle;
}

int main() {
	int x = -80;
	int y = 100;
	int angleVal = angle(x, y);
	printf("Angle for X:%d, Y:%d = %d\n", x, y, angleVal);
	drive(angleVal, 1.0, -0.5);
	return 0;
}
