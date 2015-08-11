
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include <ionScience.h>

class CDensityMapper : public IColorMapper
{

public:

	std::string Field;
	double ValueCutoff;
	SRange<f64> AcceptedRange;
	float AlphaIntensity;

	SRange<f64> FieldRange;

	void InitialValues()
	{
		ValueCutoff = std::numeric_limits<f64>::max();
		AcceptedRange = SRange<f64>::Full;
		AlphaIntensity = 1.f;
	}

	CDensityMapper(std::string const inField)
	{
		InitialValues();
		Field = inField;
	}

	static color4f MapColor(f32 const v)
	{
		return SColorAf(v);
	}

	virtual SColorAf const GetColor(IDataRecord<f64> const & d)
	{
		f64 const r = d.GetField(Field);
		f32 const v = (f32)FieldRange.Normalize(r);

		return MapColor(v);
	}

	virtual void PreProcessValues(IDatabase<f64> & s)
	{
		FieldRange = s.GetFieldRange(Field, ValueCutoff, AcceptedRange);
	}

};
