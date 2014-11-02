#ifndef HELPER_H
#define HELPER_H

//fn returns maximum absolute value (ignores sign)
//Helper function for calculating ABSOLUTE maximum of a list of floats
float helpFindMaxAbsFloat(float a, float b, float c = (0), float d = (0), float e = (0)) {
	float values[] = {a, b, c, d, e};
	float max = a;
	for (int z=1; z<5; z++) {
		if (abs(values[z]) > max) {
			max = values[z];
		}
	}
	return abs(max);
}

#endif
