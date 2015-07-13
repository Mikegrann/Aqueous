
#pragma once
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"

class CSharkNodeManager : public Singleton<CSharkNodeManager>, public IEventListener
{

public:

	void Init();
	void Update(f32 const Elapsed);
	
	CSceneNode * GetNode();
	CSceneNode const * GetNode() const;

protected:

	void LoadSceneElements();
	CSceneNode * Node;
	CMesh * Mesh;

};
