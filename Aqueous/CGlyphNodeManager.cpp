
#include "CGlyphNodeManager.h"
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
	Times.clear();

	uint i;
	for (i = 0; i < Glyphs.size(); ++i) {
		if (Glyphs[i]->Time == curTime) {
			Positions.push_back(Glyphs[i]->Position.X);
			Positions.push_back(Glyphs[i]->Position.Y);
			Positions.push_back(Glyphs[i]->Position.Z);
			Colors.push_back(Glyphs[i]->Color.Red);
			Colors.push_back(Glyphs[i]->Color.Green);
			Colors.push_back(Glyphs[i]->Color.Blue);
			Times.push_back(Glyphs[i]->Time);
		}
	}

	PositionBuffer = new ion::GL::VertexBuffer;
	PositionBuffer->Data<f32>(Positions.size() * sizeof(f32), nullptr, 3);
	ColorBuffer = new ion::GL::VertexBuffer;
	ColorBuffer->Data<f32>(Colors.size() * sizeof(f32), nullptr, 3);
	TimeBuffer = new ion::GL::VertexBuffer;
	TimeBuffer->Data<u32>(Times.size() * sizeof(u32), nullptr, 1);

	if (Node)
	{
		Node->SetVertexBuffer("vPosition", PositionBuffer);
		Node->SetVertexBuffer("vColor", ColorBuffer);
		//Node->SetVertexBuffer("vTime", TimeBuffer);
		Node->SetUniform("Model", &Node->GetTransformationUniform());
		//Node->SetUniform("timeMin", &timeUniformMin);
		//Node->SetUniform("timeMax", &timeUniformMax);
		Node->SetUniform("size", &GlyphSize);
		Node->SetPrimitiveType(ion::GL::EPrimitiveType::Points);
	}

	PositionBuffer->SubData(Positions);
	ColorBuffer->SubData(Colors);
	TimeBuffer->SubData(Times);

	if (Node)
	{
		Node->SetElementCount((uint)Times.size());
		Node->SetVisible(Times.size() != 0);
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
