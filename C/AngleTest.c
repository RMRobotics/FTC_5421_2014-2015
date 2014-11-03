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
	float cosFLBR = cos((45.0 - (float)angle) * M_PI / 180.0);
	//Holds cosine value for FR and BL power calculations
	float cosFRBL = cos((45.0 + (float)angle) * M_PI / 180.0);
	//Holds max power
	float maxPow = 100.0;

	//Formulae (from above)
	float unscaledPowFL = (speed * cosFLBR) + rotation;
	float unscaledPowBL = (speed * cosFRBL) + rotation;
	float unscaledPowFR = (speed * cosFRBL) - rotation;
	float unscaledPowBR = (speed * cosFLBR) - rotation;

	//Scale to maxPower
	float absHighestPow = helpFindMaxAbsFloat(unscaledPowFL, unscaledPowBL,
						  unscaledPowFR, unscaledPowBR);
	float multiplier = maxPow / absHighestPow;
	printf("Multi: %f", multiplier);

	float scaledPowFL = unscaledPowFL * multiplier;
	float scaledPowBL = unscaledPowBL * multiplier;
	float scaledPowFR = unscaledPowFR * multiplier;
	float scaledPowBR = unscaledPowBR * multiplier;

	printf("Scaled Power FL: %f\n", scaledPowFL);
	printf("Scaled Power BL: %f\n", scaledPowBL);
	printf("Scaled Power FR: %f\n", scaledPowFR);
	printf("Scaled Power BR: %f\n", scaledPowBR);
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
		drive(angleVal, 1.0, 0.0);
	}
	return 0;
}
