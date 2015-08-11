
#pragma once

#include <ionCore.h>
#include <ionScene.h>
#include "Interp.h"

class CVolumeNodeManager : public Singleton<CVolumeNodeManager>
{

public:
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

	void UpdateTime(std::time_t t);

	vec3i Resolution;

	// Interpolations
	void SetInterpMode(Interp::Mode mode) {
		interp.mode = mode;
	}
	Interp::Mode GetInterpMode() const {
		return interp.mode;
	}

	void SetInterpFunc(Interp::RadialFunc func) {
		interp.func = func;
	}
	Interp::RadialFunc GetInterpFunc() const {
		return interp.func;
	}

	void SetInterpExp(int exponent) {
		interp.exponent = exponent;
	}
	int GetInterpExp() {
		return interp.exponent;
	}

	void SetInterpLog(bool useLog) {
		interp.useLog = useLog;
	}
	int GetInterpLog() {
		return interp.useLog;
	}

	Interp GetInterp() {
		return interp;
	}

protected:

	SingletonPointer<CSceneManager> SceneManager;

	CMesh * Cube = nullptr;
	CShader * Shader = nullptr;
	CSceneNode * Node = nullptr;

	CUniformValue<glm::mat4> InvModelMatrix;

private:
	
	friend class Singleton<CVolumeNodeManager>;
	CVolumeNodeManager();

	Interp interp;
};
