#pragma once

#include <ionEngine.h>

//structures from Parent Animation book. These structures are used to calculate Arc-Length

typedef struct tableEntryStructure
{
	f64 u;
	f64 length;
}tableEntryTd;


typedef struct intervalStructure
{
	f64 u1;
	f64 u2;
	f64 length;
}intervalTd;

typedef struct cubicCurveStructure
{
	f64 ax;
	f64 bx;
	f64 cx;
	f64 dx;
	f64 ay;
	f64 by;
	f64 cy;
	f64 dy;

}cubicCurveTd;

typedef struct polynomialStructure
{
	f64 *coeff;
	int degree;
}polynomialTd;