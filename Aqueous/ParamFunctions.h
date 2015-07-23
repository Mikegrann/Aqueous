#pragma once

#include "ParamStructures.h"
#include "CSplineTable.h"
#include <math.h>
#include <cstdlib>
#include <cstdio>

//Functions for Adaptive Gaussian Integration. From the Parent Text.
/*
 * These functions will paramaterize a curve on two dimensions. Since y is constant in Shark World, this is OK.
 */
class ParamFunctions
{
	public:
		ParamFunctions(CSplineTable *tPtr){ table = tPtr;}
		ParamFunctions(){}
		~ParamFunctions(){}
		void setTable(CSplineTable *tPtr){table = tPtr;}
		f64 adaptiveIntegration(cubicCurveTd *curve, f64 u1, f64 u2, f64 tolerance);
		f64 subdivide (intervalTd *fullInterval, polynomialTd *func, f64 totalLength, f64 tolerance);
		void addTableEntry(f64 u, f64 length);
		f64 integrateFunc(polynomialTd *func, intervalTd *interval);
		f64 evaluatePolynomial(polynomialTd *poly, f64 u);

	private:
		CSplineTable *table; 

};