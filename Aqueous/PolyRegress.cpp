#include "PolyRegress.h"

// Least Squares Regression based on following discussion:
// http://stackoverflow.com/questions/24747643/3d-linear-regression
// coeffs = theta = inv(trans(X) * X) * trans(X) * y
// theta: vector of fit coefficients [a0 a1 a2 ... aN] for a0 + a1x + a2x^2 + ... + aNx^N
// X: matrix of inputs [[1 x1 x1^2 ... x1^N]
//                      [1 x2 x2^2 ... x2^N]
//                       ...
//                      [1 xM xM^2 ... xM^N]]
// y: vector of outputs [y1 y2 ... yM]
// M: number of input points
// degree = N: degree of polynomial to fit
PolyRegress::PolyRegress(vector<float> const &xVals, vector<float> const &yVals, int degree, bool useLog) {
	logVals = useLog;
	deg = degree;
	success = false;

	int M = yVals.size();
	if (M != xVals.size()) {
		return;
	}

	ColumnVector y(M);
	Matrix X(M, degree + 1);

	for (int i = 1; i <= M; ++i) {
		y(i) = yVals[i - 1];

		int j = 1;
		float curVal = 1;
		do {
			if (useLog && j > 1) {
				X(i, j) = log(curVal);
			}
			else {
				X(i, j) = curVal;
			}
			curVal *= xVals[i - 1];
		} while (j++ < degree + 1);
	}

	coeffs = (X.t() * X).i() * X.t() * y; //inv(trans(X) * X) * trans(X) * y
	
	success = true;
}

vector<float> PolyRegress::getCoeffs() {
	vector<float> result(deg + 1);

	for (int i = 0; i < deg + 1; ++i) {
		result[i] = coeffs(i + 1);
	}

	return result;
}

float PolyRegress::interpolate(float x) {
	float result = 0;
	float curX = 1;

	for (int i = 0; i < deg + 1; ++i) {
		if (logVals) {
			result += log(curX) * coeffs(i + 1);
		}
		else {
			result += curX * coeffs(i + 1);
		}
		curX *= x;
	}

	return result;
}