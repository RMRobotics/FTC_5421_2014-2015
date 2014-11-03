#ifndef HELPER_H
#define HELPER_H

//Helper function for calculating ABSOLUTE maximum of a list of floats
//Will return maximum absolute value (ignores sign)
float helpFindMaxAbsFloat(float a, float b, float c = (0), float d = (0), float e = (0)) {
	float values[] = {a, b, c, d, e};
	float max = a;
	for (int z=1; z<5; z++) {
		if (abs(values[z]) > max) {
			max = abs(values[z]);
		}
	}
	return max;
}

int helpRoundFloat(float x) {
	if (x > 0) {
		return (int)(x+0.5);
	} else {
		return (int)(x-0.5);
	}
}

#endif
