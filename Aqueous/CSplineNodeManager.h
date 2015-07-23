
#pragma once
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"
#include "CSite.h"
#include "CSplinePath.h"
#include "CDataSet.h"

#define SPLINE_FILE "C:/Users/pnob32/Documents/Aqueous-Data/Sites/Catalina-2/shark_track_pout-2014-07-24_03-00-42-PM.csv"

class CSplineNodeManager : public Singleton<CSplineNodeManager>, public IEventListener
{

public:

	void Init();
	void Update(f32 const Elapsed);

	CSceneNode * GetNode();
	CSceneNode const * GetNode() const;

    void setCurrentSite(CSite * site);
    void setPositionBuffer(vector<f32> posBuf) { Positions = posBuf; };
    void setColorBuffer(vector<f32> colBuf) { Colors = colBuf; };
    void setIndBuffer(vector<u32> indBuf) { Indices = indBuf; };

protected:

	void LoadSceneElements();
	CSceneNode * Node;
    CSite * currSite;
	//CMesh * Mesh;
    CSplinePath path;

    bool needsUpdate;

    vector<CSplinePath*> splines;

    vector<f32> Positions;
    vector<f32> Colors;
    vector<u32> Indices;

    ion::GL::VertexBuffer * PositionBuffer = nullptr;
    ion::GL::VertexBuffer * ColorBuffer = nullptr;
    ion::GL::IndexBuffer * IndBuffer = nullptr;

};
