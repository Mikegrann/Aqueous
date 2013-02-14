#include "SciDataManager.h"

#include "RBFInterpolator/RBFInterpolator.h"


void SciDataManager::createGridDataFromRawValues(Range AcceptedValues, double Deviations, std::string const & Field)
{
	int const Size = 20;
	GridDimensions = new int[3];
	GridDimensions[0] = Size;
	GridDimensions[1] = Size;
	GridDimensions[2] = Size;

	GridValues.clear();

	std::vector<float> X, Y, Z, F;

	Range XRange = RawValues.getValueRange("x", Deviations, AcceptedValues);
	Range YRange = RawValues.getValueRange("DFS Depth (m)", Deviations, AcceptedValues);
	Range ZRange = RawValues.getValueRange("y", Deviations, AcceptedValues);
	Range FRange = RawValues.getValueRange(Field, Deviations, AcceptedValues);

	for (auto it = ++ RawValues.getValues().begin(); it != RawValues.getValues().end(); ++ it)
	{
		float x = (float) ((it->getField("x") - XRange.first) / (XRange.second - XRange.first));
		float y = (float) ((it->getField("DFS Depth (m)") - YRange.first) / (YRange.second - YRange.first));
		float z = (float) ((it->getField("y") - ZRange.first) / (ZRange.second - ZRange.first));
		float f = (float) ((it->getField(Field) - FRange.first) / (FRange.second - FRange.first));

		//if (! inRange(f, FRan

		if (f != f ||
			x != x ||
			y != y ||
			z != z ||
			! inRange(f, Range(0, 1)) ||
			! inRange(x, Range(0, 1)) ||
			! inRange(y, Range(0, 1)) ||
			! inRange(z, Range(0, 1)))
			continue;

		X.push_back(x);
		Y.push_back(y);
		Z.push_back(z);
		F.push_back(f);
	}

	RBFInterpolator rbfi(X, Y, Z, F);
	
	for (int j = 0; j < Size; ++ j)//Size - 1; j >= 0; -- j)
	for (int k = 0; k < Size; ++ k)
	for (int i = 0; i < Size; ++ i)
	{
		SciData d(GridValues);
		d.addField(Field) = rbfi.interpolate(i / (float) Size, j / (float) Size, k / (float) Size);
		d.addField("x") = i / (float) Size;
		d.addField("y") = j / (float) Size;
		d.addField("z") = k / (float) Size;
	}
}
