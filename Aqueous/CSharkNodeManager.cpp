
#include "CSharkNodeManager.h"
#include <ionWindow.h>

#include "CProgramContext.h"
#include "CDataSet.h"

void CSharkNodeManager::Init()
{
	SingletonPointer<CSceneManager> SceneManager;
	
	Node = SceneManager->GetFactory()->AddSceneNode("Shark");
    
    shark.defSequence();

    string artistDrivenAnimations[5] = { "ani/ani_slowstraight_4.csv",
        "ani/ani_faststraight_1.csv",
        "ani/ani_leftturn_2.csv",
        "ani/ani_rightturn_2.csv",
        "ani/ani_right_uturn_1.csv" };

    for (int i = 0; i < 5; i++) {
        shark.readMovementData(artistDrivenAnimations[i].c_str(), true);
    }

    mesh = new SharkMesh();

    shark.sMesh(mesh);
    int tSegments = shark.segments;

    shark.buildSkeleton("Model/LeopardShark.aobj");

    shark.genKeyframes(true, mesh);
    shark.sFrameSpeed(10);

	LoadSceneElements();
}

void CSharkNodeManager::LoadSceneElements()
{
	if (Node) {
		Mesh = CMesh::Load("Meshes/shark.obj");
		Node->SetMesh(Mesh);
	}
}

void CSharkNodeManager::Update(f32 const Elapsed)
{
	// Proof of concept: modification of mesh buffers after initial load
	// For this example, turning all vertices yellow
	for (SVertex &vert : Mesh->Buffers[0]->Vertices) {
		vert.Color = color4f(1.0, 1.0, 0.0);
	}
	Mesh->Update();
}

CSceneNode * CSharkNodeManager::GetNode()
{
	return Node;
}

CSceneNode const * CSharkNodeManager::GetNode() const
{
	return Node;
}