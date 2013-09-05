#include "STable.h"


vec3d const STable::GetDataScale()
{
	Range xRange = GetFieldRange(Traits.PositionXField),
		yRange = GetFieldRange(Traits.PositionYField),
		zRange = GetFieldRange(Traits.PositionZField);

	return vec3d(xRange.second - xRange.first,
		yRange.second - yRange.first,
		zRange.second - zRange.first);
}

std::vector<STable::SRow> const & STable::GetValues() const
{
	return Rows;
}

std::vector<STable::SRow> const & STable::GetRows() const
{
	return Rows;
}

STable::SRow & STable::AddRow()
{
	Rows.push_back(SRow(* this, Rows.size()));

	for (auto it = Fields.begin(); it != Fields.end(); ++ it)
		it->second.resize(Maximum(Rows.size(), it->second.size()), 0);

	return Rows.back();
}

void STable::AddField(std::string const & Field)
{
	Fields[Field].resize(Rows.size());
}

bool STable::HasField(std::string const & Field)
{
	return Fields.find(Field) != Fields.end();
}

Range STable::GetFieldRange(std::string const & Field, double const OutlierCutoff, Range const & acceptedValues) const
{
	// Calculate mean
	double Mean = 0;
	unsigned int Count = Rows.size();
	for (auto it = Rows.begin(); it != Rows.end(); ++ it)
	{
		double const v = it->GetField(Field);
		if (! InRange(v, acceptedValues) || v != v)
			Count --;
		else
			Mean += it->GetField(Field);
	}
	Mean /= (double) (Count);

	// Calculate standard absolute value deviation
	double StdDeviation = 0;
	for (auto it = Rows.begin(); it != Rows.end(); ++ it)
	{
		double const v = it->GetField(Field);
		if (InRange(v, acceptedValues) && v == v)
			StdDeviation += abs(v - Mean);
	}
	StdDeviation /= (double) (Count - 1);

	// Find min/max
	double Min = std::numeric_limits<double>::max(), Max = -std::numeric_limits<double>::max();
	for (auto it = Rows.begin(); it != Rows.end(); ++ it)
	{
		double const v = it->GetField(Field);
		if (v < Mean + OutlierCutoff * StdDeviation && v > Mean - OutlierCutoff * StdDeviation)
		{
			if (v > Max)
				Max = v;
			if (v < Min)
				Min = v;
		}
	}

	return std::pair<double, double>(Min, Max);
}

STable::SFieldIterator STable::Begin(std::string const & Field)
{
	return SFieldIterator(Rows.begin(), Field);
}

STable::SFieldIterator STable::End(std::string const & Field)
{
	return SFieldIterator(Rows.end(), Field);
}

u32 const STable::Size() const
{
	return Rows.size();
}

void STable::Clear()
{
	Rows.clear();
}

void STable::WriteToFile(std::ofstream & File)
{
	// Write Fields
	u32 Dims = Fields.size();
	File.write((char *) & Dims, sizeof(u32));

	for (auto it = Fields.begin(); it != Fields.end(); ++ it)
	{
		// Write Label
		Dims = it->first.size();
		File.write((char *) & Dims, sizeof(u32));
		File.write(it->first.c_str(), Dims);

		// Write Data
		Dims = it->second.size();
		File.write((char *) & Dims, sizeof(u32));
		File.write((char *) & it->second.front(), Dims * sizeof(f64));
	}
	
	// Write Values
	Dims = Rows.size();
	File.write((char *) & Dims, sizeof(u32));

	for (auto it = Rows.begin(); it != Rows.end(); ++ it)
	{
		File.write((char *) & it->GetPosition().X, sizeof(f64));
		File.write((char *) & it->GetPosition().Y, sizeof(f64));
		File.write((char *) & it->GetPosition().Z, sizeof(f64));
		File.write((char *) & it->Index, sizeof(u32));
	}
}

void STable::ReadFromFile(std::ifstream & File)
{
	Rows.clear();

	u32 FieldCount;
	File.read((char *) & FieldCount, sizeof(u32));

	for (u32 i = 0; i < FieldCount; ++ i)
	{
		// Read Label
		u32 LabelLength;
		File.read((char *) & LabelLength, sizeof(u32));
		char * Buffer = new char[LabelLength + 1];
		File.read(Buffer, LabelLength);
		Buffer[LabelLength] = '\0';

		// Read Data
		u32 DataLength;
		File.read((char *) & DataLength, sizeof(u32));
		f64 * DataBuffer = new f64[DataLength];
		File.read((char *) DataBuffer, DataLength * sizeof(double));
		Fields[Buffer].reserve(DataLength);
		for (u32 t = 0; t < DataLength; ++ t)
			Fields[Buffer].push_back(DataBuffer[t]);

		delete [] Buffer;
		delete [] DataBuffer;
	}
	
	// Read Values
	u32 ValueCount;
	File.read((char *) & ValueCount, sizeof(u32));
	Rows.reserve(ValueCount);

	for (u32 i = 0; i < ValueCount; ++ i)
	{
		s32 InternalIndex;
		double X, Y, Z;

		// Need to read these for backwards compatibility
		File.read((char *) & X, sizeof(f64));
		File.read((char *) & Y, sizeof(f64));
		File.read((char *) & Z, sizeof(f64));
		File.read((char *) & InternalIndex, sizeof(s32));
	
		Rows.push_back(SRow(* this, Rows.size()));

		for (auto it = Fields.begin(); it != Fields.end(); ++ it)
			it->second.resize(Maximum(Rows.size(), it->second.size()), 0);
	}
}
