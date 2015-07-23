
#include "ParamFunctions.h"


f64 ParamFunctions::adaptiveIntegration(cubicCurveTd *curve, f64 u1, f64 u2, f64 tolerance)
{
	polynomialTd func;
	f64 totalLength = 0;
	f64 temp = 0;
	intervalTd fullInterval;

	//initialize math coefficients
	func.degree = 4;
	func.coeff = (f64*) malloc(sizeof(f64)*5);
	func.coeff[4] = 9*(((curve->ax)*(curve->ax)) + ((curve->ay)*(curve->ay)));
	func.coeff[3] = 12*(((curve->ax)*(curve->bx)) + ((curve->ay)*(curve->by)));
	func.coeff[2] = (6*((curve->ax)*(curve->cx) + (curve->ay)*(curve->cy)) + (4*((curve->bx)*(curve->bx) + (curve->by)*(curve->by)))) ;
	func.coeff[1] = 4*((curve->bx)*(curve->cx) + (curve->by)*(curve->cy));
	func.coeff[0] = (curve->cx)*(curve->cx) + (curve->cy)*(curve->cy);

	fullInterval.u1 = u1;
	fullInterval.u2 = u2;
	temp = integrateFunc(&func, &fullInterval);

	fullInterval.length = temp;
	totalLength = subdivide(&fullInterval, &func, 0.0, tolerance);

	//free func.coeff //table of subdivision points is created
	free (func.coeff);
	return totalLength;
}

f64 ParamFunctions::subdivide(intervalTd *fullInterval, polynomialTd *func, f64 totalLength, f64 tolerance)
{
	intervalTd leftInterval;
	intervalTd rightInterval;
	f64 leftLength = 0;
	f64 rightLength = 0;
	f64 midu = 0; //middle u? where the curve is split in two.
	f64 temp = 0;

	//splits the curve in two. reassigning the end points with midu in the middle
	midu = (fullInterval->u1+fullInterval->u2)/2.00;
	leftInterval.u1 = fullInterval->u1; 
	leftInterval.u2 = midu;

	rightInterval.u1 = midu;
	rightInterval.u2 = fullInterval->u2;

	//integrate over the 
	leftLength = integrateFunc(func, &leftInterval);
	rightLength = integrateFunc(func, &rightInterval);

	

	temp = fabs(fullInterval->length - (leftLength+rightLength));

	//TODO. figure out what the hell is going on with totalLength. The right side adds way more than it should. It makes every other one wrong.
	if(temp  > tolerance) //error is too large. Need to subdivide again.
	{
		//this is not executed very often. in debugging runs, not at all. 
		//whee recursion. 
		leftInterval.length = leftLength;
		rightInterval.length = rightLength;
		//f64 totalLength_L = subdivide(&leftInterval, func, totalLength, tolerance/2.0);
		totalLength = subdivide(&leftInterval, func, totalLength, tolerance/2.0);
		//printf("%f > %f, %f ____ ", midu, leftInterval, totalLength);
		//f64 totalLength_R = subdivide(&rightInterval, func, totalLength, tolerance/2.0); //this line looks suspicious in that totalLength is immediately reqritten.
		totalLength = subdivide(&rightInterval, func, totalLength, tolerance/2.0); //this line looks suspicious in that totalLength is immediately reqritten.
		//printf("%f > %f, %f\n", rightInterval, totalLength);
		//return totalLength + leftLength + rightLength; //I dont even know if this is right.  //adds totalLengths toghether
		return totalLength; //adds totalLengths toghether
	}
	else //accuracy is adequate. No need to subdivide further.
	{
		//f64 totalLength_L = totalLength + leftLength;
		totalLength = totalLength + leftLength;
		addTableEntry(midu, totalLength);
		//printf ("%f > %f %f ", midu, leftLength, totalLength_L);
		//f64 totalLength_R = totalLength + rightLength;
		totalLength = totalLength + rightLength;
		addTableEntry(fullInterval->u2, totalLength);
		//printf ("______ %f %f\n",rightLength, totalLength_R);
		//return totalLength + leftLength + rightLength;
		return totalLength;

	}

}

void ParamFunctions::addTableEntry(f64 u, f64 length)
{
	//printf("%f %f\n", u, length);
	table->addTableEntry(u, length);
}


f64 ParamFunctions::integrateFunc(polynomialTd *func, intervalTd *interval)
{
	//magic coefficients from parent textbook. 3.2
	f64 x[5] = {.1488743389, .4333953941, .6794095682, .8650633666, .9739065285};
	f64 w[5] = {.2955242247, .2692667193, .2190863625, .1494513491, .0666713443};

	f64 length = 0.0;
	f64 midu;
	f64 dx;
	f64 diff;
	int i;
	f64 u1;
	f64 u2;

	u1 = interval->u1;
	u2 = interval->u2;

	midu = (u1+u2)/2.0;
	diff = (u2-u1)/2.0;

	for(i = 0; i<5; i++)
	{
		dx = diff*x[i];
		length += w[i]*(sqrt(evaluatePolynomial(func, midu+dx)) + sqrt(evaluatePolynomial(func, midu-dx)));
	}
	length *= diff;
	return length;
}

f64 ParamFunctions::evaluatePolynomial(polynomialTd *poly, f64 u)
{
    // Horner's method
	int            i;
	f64     value;

	value = poly->coeff[poly->degree];
	for (i=poly->degree-1; i >= 0; i--) {
		value = poly->coeff[i] + u*value;
	}
	return value;
}


