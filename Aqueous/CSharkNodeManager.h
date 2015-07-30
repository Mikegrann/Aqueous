
#pragma once
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"
#include "Shark.h"
#include "SharkMesh.h"

class CSharkNodeManager : public Singleton<CSharkNodeManager>, public IEventListener
{

public:

	void Init();
	void Update(f32 const Elapsed);
	
	CSceneNode * GetNode();
	CSceneNode const * GetNode() const;

	~CSharkNodeManager();

protected:

	void LoadSceneElements();
	CSceneNode * Node;
	CMesh * Mesh;

    Shark shark;
    SharkMesh * mesh;

private:

    string artistDrivenAnimations[5];
};
