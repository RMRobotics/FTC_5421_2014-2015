#ifndef HELPER_H
#define HELPER_H
#pragma systemFile

/*Helper function for calculating ABSOLUTE maximum of two floats
  Will return maximum absolute value (ignores sign) */
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

//Helper function for calculating contraints of a float
float constrain(float x, float lowerBound, float upperBound){
	if(x<lowerBound){
		return lowerBound;
	}else if(x>upperBound){
		return upperBound;
	}else{
		return x;
	}
}

#endif
