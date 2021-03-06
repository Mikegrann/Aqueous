
#pragma once

#include <ionEngine.h>


class CTerrainNodeManager : public Singleton<CTerrainNodeManager>
{

public:

	static u32 const Size = 512;
	static u32 const HeightmapSize = Size + 1;

	bool Load();
	
	void SetHeightMap(CTexture * heightMap);
	void SetColorMap(CTexture * colorMap);
	void SetBathymetryMap(CTexture * bathymetryMap);
	void SetNormalMap(CTexture * normalMap);

	void SetDebugHeightEnabled(bool const Enabled);
	bool IsDebugHeightEnabled();

	CSceneNode * GetNode();

	CUniformValue<int> DebugMode = 0;
	CUniformValue<int> HeightInterpolationMode = 2;

protected:

	CVertexBuffer * VertexData = nullptr;
	CIndexBuffer * IndexBuffer = nullptr;

	CTexture * HeightMap = nullptr;
	CTexture * ColorMap = nullptr;
	CTexture * BathymetryMap = nullptr;
	CTexture * NormalMap = nullptr;

	CShader * Shader = nullptr;

	SingletonPointer<CApplication> Application;
	SingletonPointer<CSceneManager> SceneManager;
	
	CUniformValue<bool> DebugHeight = false;

	CSceneNode * Node = nullptr;
	CUniformValue<f32> LayerWidth;

private:
	
	friend class Singleton<CTerrainNodeManager>;
	CTerrainNodeManager();

};
