//////////////////////////////////////////////////////////////////////////
//
// RBFInterpolator : interpolation by radial basis functions
// 
// 2009 Karsten Noe
//
// Read the blog at cg.alexandra.dk for more information
//
//////////////////////////////////////////////////////////////////////////

#include "RBFInterpolator.h"
#include <ionScience.h>
#include <math.h>


RBFInterpolator::RBFInterpolator()
{
	successfullyInitialized = false;
}

RBFInterpolator::RBFInterpolator(vector<float> x, vector<float> y, vector<float> z, vector<float> f, RBFunc basis)
{


	successfullyInitialized = false; // default value for if we end init prematurely.

	M = (unsigned int) f.size();

	// all four input vectors must have the same length.
	if ( x.size() != M || y.size() != M || z.size() != M )
		return;	

	basis_func = basis;
	ColumnVector F = ColumnVector(M + 4);
	P = Matrix(M, 3);

	Matrix G(M + 4,M + 4);

	// copy function values
	for (unsigned int i = 1; i <= M; i++)
		F(i) = f[i-1];
	
	F(M+1) = 0;  F(M+2) = 0;  F(M+3) = 0;  F(M+4) = 0;

	// fill xyz coordinates into P 
	for (unsigned int i = 1; i <= M; i++)
	{
		P(i,1) = x[i-1];
		P(i,2) = y[i-1];
		P(i,3) = z[i-1];
	}

	// the matrix below is symmetric, so I could save some calculations Hmmm. must be a todo
	CPrintProgressBar p;
	printf("Creating RBF Interpolator...\n");
	p.BeginProgress();
	for (unsigned int i = 1; i <= M; i++)
	for (unsigned int j = 1; j <= M; j++)
	{
		p.SetProgress(i / (f64) M);
		float dx = x[i-1] - x[j-1];
		float dy = y[i-1] - y[j-1];
		float dz = z[i-1] - z[j-1];

		float distance_squared = dx*dx + dy*dy + dz*dz;

		G(i,j) = basis_func(distance_squared);
	}

	//Set last 4 columns of G
	for (unsigned int i = 1; i <= M; i++)
	{
		G( i, M+1 ) = 1;
		G( i, M+2 ) = x[i-1];
		G( i, M+3 ) = y[i-1];
		G( i, M+4 ) = z[i-1];
	}
	
	for (unsigned int i = M+1; i <= M+4; i++)
	for (unsigned int j = M+1; j <= M+4; j++)
	{
		G( i, j ) = 0;
	}

	//Set last 4 rows of G
	for (unsigned int j = 1; j <= M; j++)
	{
		G( M+1, j ) = 1;
		G( M+2, j ) = x[j-1];
		G( M+3, j ) = y[j-1];
		G( M+4, j ) = z[j-1];
	}

	p.EndProgress();

	Try 
	{ 
		printf("Inverting Matrix...\n");
		Ginv = G.i(); 

		printf("Multiplying Matrix...\n");
		A = Ginv*F;

		successfullyInitialized = true;
	}
    CatchAll { cout << BaseException::what() << endl; }
	printf("Interpolator created!\n");
}

RBFInterpolator::~RBFInterpolator()
{

}

float RBFInterpolator::interpolate(float x, float y, float z)
{
	if (!successfullyInitialized)
		return 0.0f;

	float sum = 0.0f;

	// RBF part
	for (unsigned int i = 1; i <= M; i++)
	{
		float dx = x - (float) P(i,1);
		float dy = y - (float) P(i,2);
		float dz = z - (float) P(i,3);

		float distance_squared = dx*dx + dy*dy + dz*dz;

		sum += (float)(A(i) * basis_func(distance_squared));
	}
	
	//affine part
	sum += (float) (A(M+1) + A(M+2)*x + A(M+3)*y + A(M+4)*z);

	return sum;
}

//note: assuming the input is t squared
//sqrt(log10(r^2 + B^2))
float RBFInterpolator::log_shift(float t_squared)
{	
	return sqrt(log10(t_squared + 1.0f));
}

//r^2 * ln(r)
float RBFInterpolator::thin_spline(float t_squared)
{
	float t = sqrt(t_squared);
	return t ? t_squared * log(t) : 0;
}

void RBFInterpolator::UpdateFunctionValues(vector<float> f)
{
	successfullyInitialized = false;

	ColumnVector F(M+4);

	// copy function values
	for (unsigned int i = 1; i <= M; i++)
		F(i) = f[i-1];
	
	F(M+1) = 0;  F(M+2) = 0;  F(M+3) = 0;  F(M+4) = 0;

	Try 
	{ 
		A = Ginv*F;
		successfullyInitialized = true;
	}
    CatchAll { cout << BaseException::what() << endl; }

}
