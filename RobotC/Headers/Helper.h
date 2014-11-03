#ifndef HELPER_H
#define HELPER_H

//Helper function for calculating ABSOLUTE maximum of two floats
//Will return maximum absolute value (ignores sign)
float helpFindMaxAbsFloat(float a, float b) {
	float aAbs = abs(a);
	float bAbs = abs(b);
	if (aAbs > bAbs) {
		return aAbs;
	} else {
		return bAbs;
	}
}

//Helper function for calculating ABSOLUTE minimum of two floats
//Will return minimum absolute value (ignores sign)
float helpFindMinAbsFloat(float a, float b) {
	float aAbs = abs(a);
	float bAbs = abs(b);
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
	if (x > 0) {
		return (int)(x+0.5);
	} else {
		return (int)(x-0.5);
	}
}

#endif
