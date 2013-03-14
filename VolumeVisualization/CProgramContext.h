#ifndef _CPROGRAMCONTEXT_H_INCLUDED_
#define _CPROGRAMCONTEXT_H_INCLUDED_

#include <ionCore.h>
#include <ionScene.h>
#include <ionWindow.h>

#include "CGUIContext.h"


class CTerrainSceneObject;
class CVolumeSceneObject;
class CGlyphSceneObject;

class SciDataManager;


class CProgramContext : public Singleton<CProgramContext>
{

public:

	//! Hold scene parameters
	struct SScene
	{
		// Cameras
		CCameraControl * Camera;
		ICameraSceneObject * OrbitCamera;

		// Orbit Camera Timer
		float Timer;

		// Lighting
		CMeshSceneObject * LightObject;
		SVector3f LightPosition;

		// Skybox object
		CMeshSceneObject * SkyBox;
		
		// Terrain
		CTerrainSceneObject * Terrain;
		
		// Glyph representation
		CGlyphSceneObject * GlyphSceneObject;
	
		// Volume
		CVolumeSceneObject * VolumeSceneObject;

		// Default cube mesh
		CMesh * Cube;


		SScene();
	};

	//! Holds shaders
	struct SShaders
	{
		CShader * Diffuse;
		CShader * DiffuseTexture;
		CShader * Terrain;
		CShader * Volume;
		CShader * Glyph;
		CShader * GlyphLines;

		SShaders();
	};

	SShaders Shaders;
	SScene Scene;

	CGUIContext * GUIContext;
	SciDataManager * DataManager;

	CProgramContext();
	void run();

};

#endif
