
#include "CSplineNodeManager.h"
#include <ionWindow.h>

#include "CProgramContext.h"
//#include "EXEreader.h"
/*#include "CDataSet.h"
#include "SciDataParser.h"*/

void CSplineNodeManager::Init()
{
	printf("Initing CSplineNodeManager!\n");
	SingletonPointer<CSceneManager> SceneManager;

	/*SciDataParserCSV csvParser = SciDataParserCSV();
	csvParser.FileName = SPLINE_FILE;
	csvParser.Load();*/
//    EXEreader* parser = new EXEreader();
  //  parser->parseFile(SPLINE_FILE);

	Node = SceneManager->GetFactory()->AddSceneNode("Spline");
	LoadSceneElements();
}

void CSplineNodeManager::LoadSceneElements()
{
	/*if (Node) {
		Mesh = CMesh::Load("Meshes/Spline.obj");
		Node->SetMesh(Mesh);
	}*/
}

void CSplineNodeManager::Update(f32 const Elapsed)
{
	// Proof of concept: modification of mesh buffers after initial load
	// For this example, turning all vertices yellow
	/*for (SVertex &vert : Mesh->Buffers[0]->Vertices) {
		vert.Color = color4f(1.0, 1.0, 0.0);
	}
	Mesh->Update();*/
}

CSceneNode * CSplineNodeManager::GetNode()
{
	return Node;
}

CSceneNode const * CSplineNodeManager::GetNode() const
{
	return Node;
}