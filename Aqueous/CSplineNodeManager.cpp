
#include "CSplineNodeManager.h"
#include <ionWindow.h>

#include "CSplinePath.h"
//#include "EXEreader.h"
/*#include "CDataSet.h"
#include "SciDataParser.h"*/

void CSplineNodeManager::Init()
{
	printf("Initing CSplineNodeManager!\n");
	SingletonPointer<CSceneManager> SceneManager;

    //EXEreader* parser = new EXEreader();
    //parser->parseFile(SPLINE_FILE);
    //path = CSplinePath();
    //path.initSpline(SPLINE_FILE);

	Node = SceneManager->GetFactory()->AddSceneNode("Glyph");
	//LoadSceneElements();

    currSite = nullptr;
    needsUpdate = false;
}

void CSplineNodeManager::LoadSceneElements()
{
    printf("Loading scene elements (spline)\n");
    PositionBuffer = new ion::GL::VertexBuffer;
    PositionBuffer->Data<f32>(Positions.size() * sizeof(f32), nullptr, 3);
    ColorBuffer = new ion::GL::VertexBuffer;
    ColorBuffer->Data<f32>(Colors.size() * sizeof(f32), nullptr, 3);
    //IndBuffer = new ion::GL::IndexBuffer;
    //IndBuffer->Data<u32>(Indices.size() * sizeof(u32), nullptr);
    //IndBuffer->Data<u32>(Indices);

    if (Node)
    {
        Node->SetVertexBuffer("vPosition", PositionBuffer);
        Node->SetVertexBuffer("vColor", ColorBuffer);
        //Node->SetVertexBuffer("vTime", TimeBuffer);
        Node->SetUniform("Model", &Node->GetTransformationUniform());
        //Node->SetUniform("timeMin", &timeUniformMin);
        //Node->SetUniform("timeMax", &timeUniformMax);
        //Node->SetPrimitiveType(ion::GL::EPrimitiveType::Lines);
        Node->SetPrimitiveType(ion::GL::EPrimitiveType::Points);
    }

    PositionBuffer->SubData(Positions);
    ColorBuffer->SubData(Colors);
    //IndBuffer->SubData(Indices);
    //TimeBuffer->SubData(Times);

    if (Node)
    {
        Node->SetElementCount((uint)Indices.size());
        Node->SetVisible(true);
    }
}

void CSplineNodeManager::Update(f32 const Elapsed)
{
    // Proof of concept: modification of mesh buffers after initial load
    // For this example, turning all vertices yellow
    /*for (SVertex &vert : Mesh->Buffers[0]->Vertices) {
        vert.Color = color4f(1.0, 1.0, 0.0);
        }
        Mesh->Update();*/

    // load in vertex/index buffers from the shark track spline path(s)
    // TODO support multiple tracks
    if (currSite != nullptr && needsUpdate) {
        Positions = splines[0]->getPositionBuffer();
        Colors = splines[0]->getColorBuffer();
        Indices = splines[0]->getIndexBuffer();

        printf("In spline update\n");
        LoadSceneElements();

        needsUpdate = false;
    }
}

CSceneNode * CSplineNodeManager::GetNode()
{
	return Node;
}

CSceneNode const * CSplineNodeManager::GetNode() const
{
	return Node;
}

void CSplineNodeManager::setCurrentSite(CSite * site) {
    if (currSite == nullptr && site != nullptr) {
        currSite = site;
        splines = currSite->GetTracks();
        splines[0]->drawPoints(0, false);
        needsUpdate = true;
    }
    else if (currSite != nullptr && site != nullptr) {
        if (site->GetName() != currSite->GetName()) {
            currSite = site;
            splines = currSite->GetTracks();
            splines[0]->drawPoints(0, false);
            needsUpdate = true;
        }
    }
}