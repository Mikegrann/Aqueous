
#pragma once

#include <ionCore.h>
#include <ionScene.h>


class CVolumeNodeManager : public Singleton<CVolumeNodeManager>
{

public:
	enum InterpMode {
		Radial_Log,
		Radial_ThinSpline,
		Connor,
		NumModes
	};
	static std::string GetInterpName(InterpMode mode) {
		switch (mode) {
			case Radial_Log: 
				return "Radial Basis - Log Shift";
			break;

			case Radial_ThinSpline:
				return "Radial Basis - Thin Spline";
				break;

			case Connor:
				return "Depth & Inverse Dist";
			break;

			default: 
				return "";
			break;
		}
	}

	void SetInterpMode(InterpMode mode) {
		Interp = mode;
	}
	InterpMode GetInterpMode() const {
		return Interp;
	}

	struct SControl
	{
		// To Do : Provide actual ranges and use for volume control widget
		CUniformValue<int> Mode;
		CUniformValue<int> DebugLevel;
		CUniformValue<int> UseShading;
		CUniformValue<vec3f> SliceAxis;
		CUniformValue<float> LocalRange;
		CUniformValue<float> MinimumAlpha;
		CUniformValue<float> EmphasisLocation;
		CUniformValue<float> AlphaIntensity;
		
		f32 StepSize;
		CUniformValue<f32> StepSizeUniform;

		SControl();
	};
	SControl Control;
	
	CTexture3D * VolumeData = nullptr;
	CTexture3D * ProximityTexture = nullptr;
	
	CSceneNode * GetNode();

	bool Load();
	void Update();
	void LoadTextures();

protected:

	SingletonPointer<CSceneManager> SceneManager;

	CMesh * Cube = nullptr;
	CShader * Shader = nullptr;
	CSceneNode * Node = nullptr;

	CUniformValue<glm::mat4> InvModelMatrix;

private:
	
	friend class Singleton<CVolumeNodeManager>;
	CVolumeNodeManager();

	InterpMode Interp;
};
