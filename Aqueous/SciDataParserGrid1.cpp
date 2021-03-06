
#include "SciDataParser.h"
#include "CDataSet.h"

#include <mat.h>

#include <ionScene.h>


void SciDataParserGrid1::Load()
{
	MATFile * File = matOpen(FileName.c_str(), "r");

	if (! File)
		return;

	mxArray * var1 = matGetVariable(File, "var1");
	mxArray * var2 = matGetVariable(File, "var2");
	mxArray * var3 = matGetVariable(File, "var3");
	mxArray * var4 = matGetVariable(File, "var4");
	mxArray * pointO1 = matGetVariable(File, "pointO1");
	mxArray * pointO2 = matGetVariable(File, "pointO2");
	mxArray * pointO3 = matGetVariable(File, "pointO3");
	mxArray * pointO4 = matGetVariable(File, "pointO4");
	mxArray * pointX = matGetVariable(File, "pointX");
	mxArray * pointY = matGetVariable(File, "pointY");
	mxArray * pointZ = matGetVariable(File, "pointZ");
	
	if (mxGetNumberOfDimensions(pointO1) != 3 || 
		mxGetNumberOfDimensions(pointO2) != 3 || 
		mxGetNumberOfDimensions(pointO3) != 3 || 
		mxGetNumberOfDimensions(pointO4) != 3 || 
		mxGetNumberOfDimensions(pointX) != 3 || 
		mxGetNumberOfDimensions(pointY) != 3 || 
		mxGetNumberOfDimensions(pointZ) != 3 || 
		mxGetNumberOfDimensions(var1) != 3 || 
		mxGetNumberOfDimensions(var2) != 3 || 
		mxGetNumberOfDimensions(var3) != 3 || 
		mxGetNumberOfDimensions(var4) != 3)
	{
		printf("Unexpected number of dimensions of input data.\n");
		return;
	}

	size_t const * Dimensions = mxGetDimensions(pointO1);
	vec3i Dims((int) Dimensions[0], (int) Dimensions[1], (int) Dimensions[2]);
	DataSet->Volume.Dimensions = Dims;
	double * pointO1Data = mxGetPr(pointO1);
	double * pointO2Data = mxGetPr(pointO2);
	double * pointO3Data = mxGetPr(pointO3);
	double * pointO4Data = mxGetPr(pointO4);
	double * var1Data = mxGetPr(var1);
	double * var2Data = mxGetPr(var2);
	double * var3Data = mxGetPr(var3);
	double * var4Data = mxGetPr(var4);
	double * pointXData = mxGetPr(pointX);
	double * pointYData = mxGetPr(pointY);
	double * pointZData = mxGetPr(pointZ);
	
	DataSet->Volume.Dimensions.X = (int) Dimensions[0];
	DataSet->Volume.Dimensions.Y = (int) Dimensions[2];
	DataSet->Volume.Dimensions.Z = (int) Dimensions[1];
	DataSet->Volume.Allocate();

	for (int k = 0; k < Dimensions[2]; ++ k)
	{
		for (int j = 0; j < Dimensions[1]; ++ j)
		{
			for (int i = 0; i < Dimensions[0]; ++ i)
			{
				size_t index = i + j * Dimensions[0] + k * Dimensions[0] * Dimensions[1];

				SVolumeDataRecord<f64> & Row = DataSet->Volume[i][k][j];
				Row.GetField("o1") = pointO1Data[index];
				Row.GetField("o2") = pointO2Data[index];
				Row.GetField("o3") = pointO3Data[index];
				Row.GetField("o4") = pointO4Data[index];
				Row.GetField("var1") = var1Data[index];
				Row.GetField("var2") = var2Data[index];
				Row.GetField("var3") = var3Data[index];
				Row.GetField("var4") = var4Data[index];
				Row.GetField("x") = pointXData[index];
				Row.GetField("y") = pointZData[index];
				Row.GetField("z") = pointYData[index];
			}
		}
	}
}
