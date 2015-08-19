
#pragma once
#include <ionCore.h>
#include <ionScene.h>

#include "CProgramContext.h"
#include "CSite.h"
#include "Shark.h"
#include "SharkMesh.h"
#include "SharkWorld.h"
#include "CSplinePath.h"

#define SHARK_SCALE 0.05f

class CSharkNodeManager : public Singleton<CSharkNodeManager>, public IEventListener
{

public:

	void Init();
	void Update(f32 const Elapsed);
	
	CSceneNode * GetNode();
	CSceneNode const * GetNode() const;

	~CSharkNodeManager();
    void setCurrentSite(CSite * site);

    void toggleAnimation(bool moving);
    void step() { toStep = true; }

protected:

	void LoadSceneElements();
	CSceneNode * Node;
	CMesh * Mesh;
    CMesh * TestMesh;
    CSite * currSite;
    CSplinePath * path;
    SharkWorld * world;

    glm::vec3 prevPosition;

    Shark shark;
    SharkMesh * mesh;
    SMeshBuffer * meshBuffer;

    ion::GL::VertexBuffer * PositionBuffer = nullptr;
    ion::GL::VertexBuffer * ColorBuffer = nullptr;
    ion::GL::IndexBuffer * IndBuffer = nullptr;

private:
    bool toStep;
    bool isMoving;
    ion::GL::VertexBuffer * createVertexBuffer();
    string artistDrivenAnimations[5];
};
