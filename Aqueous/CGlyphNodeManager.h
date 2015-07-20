
#pragma once
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"


class CDataSet;

class CGlyph
{
	
public:

	vec3f GetPosition() const;
	color3f GetColor() const;
	std::time_t GetTime() const;

private:

	friend class CGlyphNodeManager;

	vec3f Position;
	color3f Color;
	std::time_t Time;

};

class CGlyphNodeManager : public Singleton<CGlyphNodeManager>, public IEventListener
{

public:

	void Init();
	void LoadGlyphs(CDataSet * DataSet, IColorMapper * ColorMapper);
	void EmitParticles(int Count, vec3f const & Position, color3f const & Color);
	
	CSceneNode * GetNode();
	CSceneNode const * GetNode() const;

	void UpdateTime(std::time_t);

protected:

	void LoadSceneElements();
	void LoadSceneElementsAtTime(std::time_t curTime);

	vector<CGlyph *> Glyphs;
	CSceneNode * Node;
	
	vector<f32> Positions;
	vector<f32> Colors;
	vector<std::time_t> Times;
	ion::GL::VertexBuffer * PositionBuffer = nullptr;
	ion::GL::VertexBuffer * ColorBuffer = nullptr;
	ion::GL::VertexBuffer * TimeBuffer = nullptr;

	//CUniformValue<int> timeUniformMin;
	//CUniformValue<int> timeUniformMax;

};
