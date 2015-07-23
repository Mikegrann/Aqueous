#pragma once

#include <ionEngine.h>
#include "SharkQuadObject.h"
#include "Keyframe.h"

class FrameSequence
{
	public:
		FrameSequence(){shaderProg = 0; numInBetweens = 10;}
		FrameSequence(GLuint _shaderProg, SharkQuadObject* _sharkObject, bool mode){shaderProg = _shaderProg; sharkObject = _sharkObject; numInBetweens = 10; isDynamicMode = mode; }
		~FrameSequence(){}	
		void drawFrame(int frameNo);
		void drawDynamicFrame(int frameNo);
		void dynamicAdvancement(map<glm::vec3, SharkVertex*, compareVect3> *sVertices, vector<Quad*> *quads );
		void deleteFrameHeaps();	
		void sNumInBetweens(int d){numInBetweens = d;}
		int gNumInBetweens(){return numInBetweens;}
		bool inDynamicMode(){return isDynamicMode;}
		Keyframe gFrame(int index){return *frames[index];}
		void sFrame(Keyframe frame){frames.push_back(&frame);}
		void sFrame(Keyframe *frame){frames.push_back(frame);}
		size_t size(){return frames.size();}
		void setShaderProg(GLuint _shaderProg) {shaderProg = _shaderProg;}

	private:	
		GLuint shaderProg;
		SharkQuadObject* sharkObject;
		vector<Keyframe*> frames;
		int numInBetweens;
		bool isDynamicMode;	
};
