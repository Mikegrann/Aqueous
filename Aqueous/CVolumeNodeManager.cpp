#include "CVolumeNodeManager.h"
#include "CSite.h"
#include "CDataSet.h"

#include <ionWindow.h>

#include "CProgramContext.h"


CVolumeNodeManager::SControl::SControl()
	: Mode(0), SliceAxis(vec3f(1.f, 0.f, 0.f)),
	LocalRange(0.2f), MinimumAlpha(0.1f),
	EmphasisLocation(0.5f), AlphaIntensity(1.f),
	StepSize(100.f), DebugLevel(0), UseShading(0)
{}

CVolumeNodeManager::CVolumeNodeManager() : interp((Interp::Mode)0, (Interp::RadialFunc)0, 2, true), Resolution(28)
{}

bool CVolumeNodeManager::Load()
{
	Cube = new CMesh();
	SMeshBuffer * Mesh = new SMeshBuffer();

	Mesh->Vertices.resize(24);
	Mesh->Vertices[0].Position = SVector3f(-0.5, -0.5, -0.5);
	Mesh->Vertices[1].Position = SVector3f(-0.5, 0.5, -0.5);
	Mesh->Vertices[2].Position = SVector3f(0.5, 0.5, -0.5);
	Mesh->Vertices[3].Position = SVector3f(0.5, -0.5, -0.5);
	Mesh->Vertices[0].Color = SColorf(0, 0, 0);
	Mesh->Vertices[1].Color = SColorf(0, 1, 0);
	Mesh->Vertices[2].Color = SColorf(1, 1, 0);
	Mesh->Vertices[3].Color = SColorf(1, 0, 0);

	Mesh->Vertices[4].Position = SVector3f(0.5, -0.5, -0.5);
	Mesh->Vertices[5].Position = SVector3f(0.5, 0.5, -0.5);
	Mesh->Vertices[6].Position = SVector3f(0.5, 0.5, 0.5);
	Mesh->Vertices[7].Position = SVector3f(0.5, -0.5, 0.5);
	Mesh->Vertices[4].Color = SColorf(1, 0, 0);
	Mesh->Vertices[5].Color = SColorf(1, 1, 0);
	Mesh->Vertices[6].Color = SColorf(1, 1, 1);
	Mesh->Vertices[7].Color = SColorf(1, 0, 1);

	Mesh->Vertices[8].Position = SVector3f(0.5, -0.5, 0.5);
	Mesh->Vertices[9].Position = SVector3f(0.5, 0.5, 0.5);
	Mesh->Vertices[10].Position = SVector3f(-0.5, 0.5, 0.5);
	Mesh->Vertices[11].Position = SVector3f(-0.5, -0.5, 0.5);
	Mesh->Vertices[8].Color = SColorf(1, 0, 1);
	Mesh->Vertices[9].Color = SColorf(1, 1, 1);
	Mesh->Vertices[10].Color = SColorf(0, 1, 1);
	Mesh->Vertices[11].Color = SColorf(0, 0, 1);

	Mesh->Vertices[12].Position = SVector3f(-0.5, -0.5, 0.5);
	Mesh->Vertices[13].Position = SVector3f(-0.5, 0.5, 0.5);
	Mesh->Vertices[14].Position = SVector3f(-0.5, 0.5, -0.5);
	Mesh->Vertices[15].Position = SVector3f(-0.5, -0.5, -0.5);
	Mesh->Vertices[12].Color = SColorf(0, 0, 1);
	Mesh->Vertices[13].Color = SColorf(0, 1, 1);
	Mesh->Vertices[14].Color = SColorf(0, 1, 0);
	Mesh->Vertices[15].Color = SColorf(0, 0, 0);

	Mesh->Vertices[16].Position = SVector3f(-0.5, 0.5, -0.5);
	Mesh->Vertices[17].Position = SVector3f(-0.5, 0.5, 0.5);
	Mesh->Vertices[18].Position = SVector3f(0.5, 0.5, 0.5);
	Mesh->Vertices[19].Position = SVector3f(0.5, 0.5, -0.5);
	Mesh->Vertices[16].Color = SColorf(0, 1, 0);
	Mesh->Vertices[17].Color = SColorf(0, 1, 1);
	Mesh->Vertices[18].Color = SColorf(1, 1, 1);
	Mesh->Vertices[19].Color = SColorf(1, 1, 0);

	Mesh->Vertices[20].Position = SVector3f(0.5, -0.5, -0.5);
	Mesh->Vertices[21].Position = SVector3f(0.5, -0.5, 0.5);
	Mesh->Vertices[22].Position = SVector3f(-0.5, -0.5, 0.5);
	Mesh->Vertices[23].Position = SVector3f(-0.5, -0.5, -0.5);
	Mesh->Vertices[20].Color = SColorf(1, 0, 0);
	Mesh->Vertices[21].Color = SColorf(1, 0, 1);
	Mesh->Vertices[22].Color = SColorf(0, 0, 1);
	Mesh->Vertices[23].Color = SColorf(0, 0, 0);

	Mesh->Triangles.resize(12);
	for (int i = 0; i < 6; ++ i)
	{
		Mesh->Vertices[4 * i + 0].TextureCoordinates = SVector2f(0, 1);
		Mesh->Vertices[4 * i + 1].TextureCoordinates = SVector2f(0, 0);
		Mesh->Vertices[4 * i + 2].TextureCoordinates = SVector2f(1, 0);
		Mesh->Vertices[4 * i + 3].TextureCoordinates = SVector2f(1, 1);

		Mesh->Triangles[2 * i].Indices[0] = 4 * i + 0;
		Mesh->Triangles[2 * i].Indices[1] = 4 * i + 1;
		Mesh->Triangles[2 * i].Indices[2] = 4 * i + 2;

		Mesh->Triangles[2 * i + 1].Indices[0] = 4 * i + 0;
		Mesh->Triangles[2 * i + 1].Indices[1] = 4 * i + 2;
		Mesh->Triangles[2 * i + 1].Indices[2] = 4 * i + 3;
	}

	Cube->Buffers.push_back(Mesh);
	Cube->LoadDataIntoBuffers();

	Shader = CProgramContext::Get().Shaders.Volume;

	Node = SceneManager->GetFactory()->AddSceneNode();
	Node->SetDebugName("Volume");

	if (Node)
	{
		Node->SetShader(Shader, "Volume");
		Node->SetFeatureEnabled(ion::GL::EDrawFeature::DisableDepthTest);
		Node->SetFeatureEnabled(ion::GL::EDrawFeature::Blend);
		Node->SetFeatureEnabled(ion::GL::EDrawFeature::CullFront);

		Node->SetUniform("uModelMatrix", & Node->GetTransformationUniform());
		Node->SetUniform("uInvModelMatrix", & InvModelMatrix);

		Node->SetUniform("uAlphaIntensity", & Control.AlphaIntensity);
		Node->SetUniform("uHighlightMode", & Control.Mode);
		Node->SetUniform("uSliceAxis", & Control.SliceAxis);
		Node->SetUniform("uLocalRange", & Control.LocalRange);
		Node->SetUniform("uMinimumAlpha", & Control.MinimumAlpha);
		Node->SetUniform("uEmphasisLocation", & Control.EmphasisLocation);
		Node->SetUniform("uStepSize", & Control.StepSizeUniform);
		Node->SetUniform("uDebugLevel", & Control.DebugLevel);
		Node->SetUniform("uUseShading", & Control.UseShading);
		
		Node->SetVertexBuffer("aPosition", Cube->Buffers[0]->VertexBuffers.Positions);
		Node->SetVertexBuffer("aColor", Cube->Buffers[0]->VertexBuffers.Colors);
		Node->SetIndexBuffer(Cube->Buffers[0]->VertexBuffers.Indices);

		return true;
	}

	return false;
}

void CVolumeNodeManager::LoadTextures()
{
	if (Node)
	{
		Node->SetTexture("uVolumeData", VolumeData);
		Node->SetTexture("uProximityData", ProximityTexture);
		Node->SetTexture("uDepthTexture", CProgramContext::Get().SceneDepthBuffer);
	}
}

void CVolumeNodeManager::Update()
{
	if (Node)
	{
		InvModelMatrix = glm::inverse(Node->GetTransformationUniform().GetValue());
		Control.StepSizeUniform = 1.f / Control.StepSize;
	}
}

void CVolumeNodeManager::UpdateTime(std::time_t t)
{
	CProgramContext * Context = &CProgramContext::Get();

	Context->CurrentSite->GetCurrentDataSet()->GenerateVolume(t, GetInterp());
}

CSceneNode * CVolumeNodeManager::GetNode()
{
	return Node;
}
