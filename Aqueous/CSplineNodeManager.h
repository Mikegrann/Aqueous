
#pragma once
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"
#include "CDataSet.h"

#define SPLINE_FILE "shark_track_pout-2014-07-24_03-00-42-PM.csv"

class CSplineNodeManager : public Singleton<CSplineNodeManager>, public IEventListener
{

public:

	void Init();
	void Update(f32 const Elapsed);

	CSceneNode * GetNode();
	CSceneNode const * GetNode() const;

protected:

	void LoadSceneElements();
	CSceneNode * Node;
	//CMesh * Mesh;

};
