
#include "CDataSet.h"

#include "ColorMappers.h"
#include "CGlyphNodeManager.h"
#include "CVolumeNodeManager.h"
#include "SciDataParser.h"
#include "CSite.h"
#include "RBFInterpolator/RBFInterpolator.h"
#include "PolyRegress.h"
#include "InvDistWeight.h"
#include "Interp.h"
#include "CWorldTime.h"


CDataSet::CDataSet(CSite * Site)
	: VolumeHandle()
{
	this->Site = Site;
}

void CDataSet::Load(IProgressBar::CTask * Task)
{
	std::ifstream PointFile, VolumeFile;
	PointFile.open(PointFileName);
	VolumeFile.open(VolumeFileName);

	if (PointFile.is_open())
		Points.ReadFromFile(PointFile);
	if (VolumeFile.is_open())
		Volume.ReadFromFile(VolumeFile);

	int Done = 0;
	f64 Total = Assets.size();
	for (auto Asset : Assets)
	{
		SciDataParser * Parser = nullptr;
		if (Asset.Parser == "SimpleTXT")
			Parser = new SciDataParserSimpleTXT();
		else if (Asset.Parser == "Grid1")
			Parser = new SciDataParserGrid1();
		else if (Asset.Parser == "CSV")
		{
			SciDataParserCSV * CSV = new SciDataParserCSV();
			CSV->FieldDelim = Asset.FieldDelimiter;
			CSV->ValueDelim = Asset.ValueDelimiter;

			Parser = CSV;
		}
		else {
			cout << "Invalid Parser Specified: " << Asset.Parser << " (File: " << Asset.File << ")" << endl;
		}

		if (Parser) {
			Parser->DataSet = this;
			Parser->FileName = Site->GetPath() + Asset.File;
			Parser->Load();
			Task->Update(++Done / Total);
		}
	}
}

void CDataSet::InitTextures() {
	glGenTextures(1, &VolumeHandle);
	glGenTextures(1, &ProximityTextureHandle);
}

void CDataSet::ConcurrentLoad()
{
	InitTextures();
	auto firstPoint = Points.begin();
	GenerateVolume((std::time_t)firstPoint->GetField(Traits.TField), Interp((Interp::Mode)0, (Interp::RadialFunc)0, 2, true));
}

void CDataSet::GenerateVolume(std::time_t targetTime, Interp interp) {
	IColorMapper * Mapper = nullptr;

	if (interp.mode == Interp::Mode::Density)
		Mapper = new CDensityMapper(ColorField);
	else if (VolumeColorMapper == "Spectrum")
		Mapper = new CSpectrumColorMapper(ColorField);
	else if (VolumeColorMapper == "Oxygen")
		Mapper = new COxygenColorMapper();

	GenerateVolumeFromPointData(targetTime, interp);

	Volume.MakeOpenGLVolume(VolumeHandle, Mapper);
	
	vec3u Dimensions = Volume.Dimensions; // (10);
	u8 * const VolumeData = new u8[Dimensions.X * Dimensions.Y * Dimensions.Z]();

	SRange<f64> XRange = Points.GetFieldRange(Traits.PositionXField, 15.0);
	SRange<f64> YRange = Points.GetFieldRange(Traits.PositionYField, 15.0);
	SRange<f64> ZRange = Points.GetFieldRange(Traits.PositionZField, 15.0);

	if (XRange.IsEmpty())
		XRange = SRange<f64>(-1, 1);
	if (YRange.IsEmpty())
		YRange = SRange<f64>(-1, 1);
	if (ZRange.IsEmpty())
		ZRange = SRange<f64>(-1, 1);

	for (u32 k = 0; k < Dimensions.Z; ++ k)
		for (u32 j = 0; j < Dimensions.Y; ++ j)
			for (u32 i = 0; i < Dimensions.X; ++ i)
			{
				u32 const Index = i + j * Dimensions.X + k * Dimensions.X * Dimensions.Y;

				//f32 MinDistance = 1.0;
				//for (auto Point : Points.GetValues())
				//{
				//	f32 X = (f32) XRange.Normalize(Point.GetField(Traits.PositionXField));
				//	if (XRange.IsEmpty())
				//		X = 0.f;

				//	f32 Y = (f32) YRange.Normalize(Point.GetField(Traits.PositionYField));
				//	if (Traits.InvertY)
				//		Y = 1.f - Y;
				//	if (YRange.IsEmpty())
				//		Y = 0.f;

				//	f32 Z = (f32) ZRange.Normalize(Point.GetField(Traits.PositionZField));
				//	if (ZRange.IsEmpty())
				//		Z = 0.f;

				//	f32 Distance = vec3f(X, Y, Z).GetDistanceSqFrom(vec3f(i / (f32) Dimensions.X, j / (f32) Dimensions.Y, k / (f32) Dimensions.Z));
				//	if (Distance < MinDistance)
				//		MinDistance = Distance;
				//}

				//f64 Value = 1.0 - 24.0 * MinDistance;
				//if (Value < 0)
				//	Value = 0;
				//VolumeData[Index] = Clamp<u32>(Value * 255.0, 0, 255);
				VolumeData[Index] = 255;
			}
			
	glBindTexture(GL_TEXTURE_3D, ProximityTextureHandle);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, Dimensions.X, Dimensions.Y, Dimensions.Z, 0, GL_RED, GL_UNSIGNED_BYTE, VolumeData);
	glBindTexture(GL_TEXTURE_3D, 0);

	delete Mapper;
}

void CDataSet::InitSceneElements(CProgramContext::SScene & Scene)
{
	Scene.Volume->VolumeData = new CTexture3D(VolumeHandle);
	Scene.Volume->ProximityTexture = new CTexture3D(ProximityTextureHandle);
	Scene.Volume->LoadTextures();


	IColorMapper * Mapper = nullptr;

	if (GlyphColorMapper == "Spectrum")
		Mapper = new CSpectrumColorMapper(ColorField);
	else if (GlyphColorMapper == "Oxygen")
		Mapper = new COxygenColorMapper();

	Scene.Glyphs->LoadGlyphs(this, Mapper);
}

void CDataSet::GenerateVolumeFromPointData(std::time_t targetTime, Interp interp)
{
	if (interp.mode == Interp::Mode::Density) {
		CountData();
	}
	else {
		InterpData(targetTime, interp);
	}
}

void CDataSet::CountData() {
	SRange<f64> XRange = Points.GetFieldRange(Traits.PositionXField, 15.0);
	SRange<f64> YRange = Points.GetFieldRange(Traits.PositionYField, 15.0);
	SRange<f64> ZRange = Points.GetFieldRange(Traits.PositionZField, 15.0);
	SRange<f64> FRange = Points.GetFieldRange(ColorField, 15.0);

	vec3f Scale = SetupVolume();
//#pragma omp parallel for schedule (dynamic)
	for (auto Point : Points) {
		f64 const X = XRange.Normalize(Point.GetField(Traits.PositionXField));
		f64 const Y = YRange.Normalize(Point.GetField(Traits.PositionYField));
		f64 const Z = ZRange.Normalize(Point.GetField(Traits.PositionZField));
		f64 const T = Point.GetField(Traits.TField);
		f64 const F = FRange.Normalize(Point.GetField(ColorField));

		//SVolumeDataRecord<f64> & Row = Volume[Y * Scale.Y][Z * Scale.Z][X * Scale.X];
		SVolumeDataRecord<f64> & Row = Volume[X * Scale.X][Y * Scale.Y][Z * Scale.Z];
//#pragma omp atomic
		Row.GetField(ColorField)++;
	}
}

void CDataSet::InterpData(std::time_t targetTime, Interp interp) {
	SRange<f64> XRange = Points.GetFieldRange(Traits.PositionXField, 15.0);
	SRange<f64> YRange = Points.GetFieldRange(Traits.PositionYField, 15.0);
	SRange<f64> ZRange = Points.GetFieldRange(Traits.PositionZField, 15.0);
	SRange<f64> FRange = Points.GetFieldRange(ColorField, 15.0);

	CPrintProgressBar print;
	cout << "Loading interpolator values..." << endl;

	// TODO: Fix N^2 Solution (Pre-sort and skip sequential values that are equal?)
	vector<real> Xs, Ys, Zs, Fs;
	map<vec3f, float> positions;
	int Count = 0;
	print.BeginProgress();
	for (auto Point : Points)
	{
		f64 const X = XRange.Normalize(Point.GetField(Traits.PositionXField));
		f64 const Y = YRange.Normalize(Point.GetField(Traits.PositionYField));
		f64 const Z = ZRange.Normalize(Point.GetField(Traits.PositionZField));
		f64 const T = Point.GetField(Traits.TField);
		f64 const F = FRange.Normalize(Point.GetField(ColorField));

		bool Skip = ((std::time_t)T < targetTime - CWorldTime::GetTimeStep() ||
			targetTime + CWorldTime::GetTimeStep() < (std::time_t)T);
		//bool Skip = ((std::time_t)T != targetTime);

		/*
		for (auto & x : Xs)
		if (X == x)
		Skip = true;
		for (auto & y : Ys)
		if (Y == y)
		Skip = true;
		for (auto & z : Zs)
		if (Z == z)
		Skip = true;
		for (auto & f : Fs)
		if (F == f)
		Skip = true;
		*/

		for (auto pos = positions.begin(); !Skip && pos != positions.end(); pos++)
			if (pos->first == vec3f(X, Y, Z))
				Skip = true;

		if (!Skip)
		{
			Xs.push_back(X);
			Ys.push_back(Y);
			Zs.push_back(Z);
			Fs.push_back(F);

			positions[vec3f(X, Y, Z)] = F;
		}

		print.SetProgress(++Count / (f32)Points.Size());
	}
	print.EndProgress();

	cout << "Creating interpolator..." << endl;
	RBFInterpolator *rbfi = 0;
	PolyRegress *pr = 0;

	switch (interp.mode) {
	case Interp::Mode::Radial:
		rbfi = new RBFInterpolator(Xs, Ys, Zs, Fs, Interp::GetFuncPtr(interp.func));
		break;

	case Interp::Mode::Connor:
		pr = new PolyRegress(Ys, Fs, 2, interp.useLog);
		break;

	default:
		cerr << "Incorrect interpolator mode specified." << endl;
		return;
		break;
	}
	cout << "Interpolating..." << endl;

	vec3f Scale = SetupVolume();
	print.BeginProgress();
	for (int k = 0; k < Volume.Dimensions.Z; ++k)
	{
		for (int j = 0; j < Volume.Dimensions.Y; ++j)
		{
			for (int i = 0; i < Volume.Dimensions.X; ++i)
			{
				SVolumeDataRecord<f64> & Row = Volume[i][j][k];
				if (rbfi) {
					Row.GetField(ColorField) = rbfi->interpolate(i / Scale.X, j / Scale.Y, k / Scale.Z);
				}
				else if (pr) {
					Row.GetField(ColorField) = pr->interpolate(j / Scale.Y);
					Row.GetField(ColorField) += InvDistWeight::interpolate(positions, vec3f(i, 1.0 - j, k) / Scale, interp.exponent);
					Row.GetField(ColorField) /= 2.0; // average the factors
				}
			}
		}

		print.SetProgress(k / (f32)Volume.Dimensions.Z);
	}

	if (rbfi) {
		delete rbfi;
	}
	if (pr) {
		delete pr;
	}
}

vec3f CDataSet::SetupVolume() {
	vec3i Resolution = CProgramContext::Get().Scene.Volume->Resolution;

	vec3f Scale = Resolution - 1;
	Volume.Dimensions = Resolution;
	Volume.Allocate();

	for (int k = 0; k < Volume.Dimensions.Z; ++k)
	{
		for (int j = 0; j < Volume.Dimensions.Y; ++j)
		{
			for (int i = 0; i < Volume.Dimensions.X; ++i)
			{
				SVolumeDataRecord<f64> & Row = Volume[i][j][k];

				Row.GetField(ColorField) = 0;
			}
		}
	}

	return Scale;
}

f64 CDataSet::GetMinColorValue() const {
	SRange<f64> FRange = Points.GetFieldRange(ColorField, 15.0);
	return FRange.Minimum;
}

f64 CDataSet::GetMaxColorValue() const {
	SRange<f64> FRange = Points.GetFieldRange(ColorField, 15.0);
	return FRange.Maximum;
}