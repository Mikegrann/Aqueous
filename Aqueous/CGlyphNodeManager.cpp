
#include "CGlyphNodeManager.h"
#include "CWorldTime.h"
#include <ionWindow.h>

#include "CProgramContext.h"
#include "CDataSet.h"

vec3f CGlyph::GetPosition() const
{
	return Position;
}

color3f CGlyph::GetColor() const
{
	return Color;
}

std::time_t CGlyph::GetTime() const
{
	return Time;
}


void CGlyphNodeManager::Init()
{
	SingletonPointer<CSceneManager> SceneManager;
	
	Node = SceneManager->GetFactory()->AddSceneNode("Glyph");

	GlyphSize = 0.02f;
}

void CGlyphNodeManager::UpdateTime(std::time_t t) {
	LoadSceneElementsAtTime(t);
}

void CGlyphNodeManager::LoadSceneElementsAtTime(std::time_t curTime)
{
	Positions.clear();
	Colors.clear();

	
	struct vectorMap
	{
		size_t operator()(const vec3f& v)const
		{
			return std::hash<int>()(v.X) + std::hash<int>()(v.Y) + std::hash<int>()(v.Z);
		}

		bool operator()(const vec3f& a, const vec3f& b)const
		{
			return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
		}
	};

	std::unordered_map<vec3f, int, vectorMap, vectorMap> pos_map;
	std::vector<float> interpWeights;
	for (uint i = 0; i < Glyphs.size(); ++i) {
		if (curTime - CWorldTime::GetTimeStep() < Glyphs[i]->Time && 
		 Glyphs[i]->Time < curTime + CWorldTime::GetTimeStep()) {
			auto query = pos_map.find(Glyphs[i]->Position);

			if (query == pos_map.end()) { // Not found
				// Store index
				pos_map[Glyphs[i]->Position] = Positions.size() / 3; 

				// Store data
				Positions.push_back(Glyphs[i]->Position.X);
				Positions.push_back(Glyphs[i]->Position.Y);
				Positions.push_back(Glyphs[i]->Position.Z);

				color3f interp_col = Glyphs[i]->Color;
				float weight = 1 - abs((Glyphs[i]->Time - curTime) / CWorldTime::GetTimeStep());
				interpWeights.push_back(weight);
				interp_col *= weight;

				Colors.push_back(interp_col.Red);
				Colors.push_back(interp_col.Green);
				Colors.push_back(interp_col.Blue);
			}
			else {
				int index = query->second;

				color3f interp_col = Glyphs[i]->Color;
				float weight = 1 - abs((Glyphs[i]->Time - curTime) / CWorldTime::GetTimeStep());
				interpWeights[index] += weight;
				interp_col *= weight;

				Colors[3 * index + 0] += interp_col.Red;
				Colors[3 * index + 1] += interp_col.Green;
				Colors[3 * index + 2] += interp_col.Blue;
			}
		}
	}

	// Weighted average
	for (uint i = 0; i < Colors.size() / 3; ++i) {
		Colors[3 * i + 0] /= interpWeights[i];
		Colors[3 * i + 1] /= interpWeights[i];
		Colors[3 * i + 2] /= interpWeights[i];
	}

	PositionBuffer = new ion::GL::VertexBuffer;
	PositionBuffer->Data<f32>(Positions.size() * sizeof(f32), nullptr, 3);
	ColorBuffer = new ion::GL::VertexBuffer;
	ColorBuffer->Data<f32>(Colors.size() * sizeof(f32), nullptr, 3);

	if (Node)
	{
		Node->SetVertexBuffer("vPosition", PositionBuffer);
		Node->SetVertexBuffer("vColor", ColorBuffer);
		Node->SetUniform("Model", &Node->GetTransformationUniform());
		Node->SetUniform("size", &GlyphSize);
		Node->SetPrimitiveType(ion::GL::EPrimitiveType::Points);
	}

	PositionBuffer->SubData(Positions);
	ColorBuffer->SubData(Colors);

	if (Node)
	{
		Node->SetElementCount(Positions.size() / 3);
		Node->SetVisible(!Positions.empty());
	}
}

void CGlyphNodeManager::LoadSceneElements()
{
	LoadSceneElementsAtTime(Glyphs[0]->Time);
}

void CGlyphNodeManager::LoadGlyphs(CDataSet * DataSet, IColorMapper * ColorMapper)
{
	Glyphs.clear();

	ColorMapper->PreProcessValues(DataSet->Points);

	SRange<f64> XRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionXField, 15.0);
	SRange<f64> YRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionYField, 15.0);
	SRange<f64> ZRange = DataSet->Points.GetFieldRange(DataSet->Traits.PositionZField, 15.0);
	SRange<f64> CRange = DataSet->Points.GetFieldRange(DataSet->ColorField, 15.0);

	if (XRange.IsEmpty())
		XRange = SRange<f64>(-1, 1);
	if (YRange.IsEmpty())
		YRange = SRange<f64>(-1, 1);
	if (ZRange.IsEmpty())
		ZRange = SRange<f64>(-1, 1);

	printf("built in data range is %g %g to %g %g long lat\n", XRange.Minimum, ZRange.Minimum, XRange.Maximum, ZRange.Maximum);
	printf("depth varies from %g to %g\n", YRange.Minimum, YRange.Maximum);

	for (auto Point : DataSet->Points.GetValues())
	{
		CGlyph * Glyph = new CGlyph();

		f32 X = (f32) XRange.Normalize(Point.GetField(DataSet->Traits.PositionXField));
		if (XRange.IsEmpty())
			X = 0.f;

		f32 Y = (f32) YRange.Normalize(Point.GetField(DataSet->Traits.PositionYField));
		//if (DataSet->Traits.InvertY)
		//	Y = 1.f - Y;
		if (YRange.IsEmpty())
			Y = 0.f;

		f32 Z = (f32) ZRange.Normalize(Point.GetField(DataSet->Traits.PositionZField));
		if (ZRange.IsEmpty())
			Z = 0.f;

		std::time_t T = (std::time_t) Point.GetField(DataSet->Traits.TField);

		/*
		f64 v = it->GetField(FloorLabel);
		if (v != 0)
		{
			f32 Depth = (f32) v / (f32) YRange.second;
			Glyph.FloorHeight = 1.f - Depth;
		}
		*/

		Glyph->Position = vec3f(X, Y, Z) - 0.5f;
		Glyph->Color = ColorMapper->GetColor(Point);
		Glyph->Time = T;

		Glyphs.push_back(Glyph);
	}

	/* Sort by timestamp
	std::sort(Glyphs.begin(), Glyphs.end(), [](CGlyph *a, CGlyph *b) {
		return a->GetTime() < b->GetTime();
	});
	*/

	LoadSceneElements();
}

CSceneNode * CGlyphNodeManager::GetNode()
{
	return Node;
}

CSceneNode const * CGlyphNodeManager::GetNode() const
{
	return Node;
}
