
#include "CSharkNodeManager.h"
#include <ionWindow.h>

#include "CProgramContext.h"
#include "CDataSet.h"

void CSharkNodeManager::Init()
{
	SingletonPointer<CSceneManager> SceneManager;
	
	Node = SceneManager->GetFactory()->AddSceneNode("Shark");
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