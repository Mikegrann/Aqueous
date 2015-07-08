#ifndef PolyRegress_H
#define PolyRegress_H

#define WANT_STREAM                  
#define WANT_MATH                    

#include "Util/newmat.h"
#include "Util/newmatio.h"
#include "Util/newmatap.h"
#include <vector>

class PolyRegress
{
public:
	PolyRegress(vector<float> const &x, vector<float> const &y, int degree, bool useLog);

	vector<float> getCoeffs();
	float interpolate(float x);

private:
	ColumnVector coeffs;
	bool success, logVals;
	int deg;
};

#endif