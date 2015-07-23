#ifndef _3D_SHARK_KEYFRAME_SYSTEM_MOOSE
#define _3D_SHARK_KEYFRAME_SYSTEM_MOOSE

#include <ionEngine.h>
#include "Quad.h"
#include "SharkQuadObject.h"
#include "MyMat.h"
//#include "Mesh.h"
#include "glQuaternion.h"
#include "SharkMesh.h"

#define SHARK_COL glm::vec3(0.7f, 0.1f, 0.3f)

/*A single keyframe in a series of animation */
class Keyframe
{
	public:
		Keyframe(){}
		Keyframe(GLuint p){shaderProg = p;}
		Keyframe(glm::vec3* rawVerts);
		Keyframe(GLuint _shaderProg, SharkQuadObject* _sharkObject, map<glm::vec3, SharkVertex*, compareVect3> *rawVerts, vector<Quad*> *quads); 
		~Keyframe(){}
		
		//void setShader(GLuint shaderProg);
		void draw(void);
		//void gatherTransformData(GLfloat segmentRot[], GLfloat segLengthInput[], Mesh *mesh, int numsegments, 
		//			glQuaternion *glQuat);
		void deleteHeap(void);
		static void drawInBetween(Keyframe start, Keyframe end, int step, int max) {start.drawInBetween(end, step, max);}	
		void drawInBetween(Keyframe end, int step, int max);

	private:
		// shader program PID
		GLuint shaderProg;
		
		// modern OpenGL draw object
		SharkQuadObject* sharkObject;

		vector<Quad*> faces;
		map<glm::vec3, SharkVertex*, compareVect3> uVertices;
		Keyframe *nextFrame;
		int frameNo;
		int followingBtwns;
		static int defaultBtwns;
		
		void createQuads(void);
		void multiBoneAttenuate();	
		glm::vec3 interpolateVertices(glm::vec3 first, glm::vec3 second, int step,int max);
		//void segmentMatrixMake( GLfloat segmentRot[], GLfloat segLength[], Mesh *mesh, int segments, 
			//	glQuaternion *glQuat, int curSegment,  MyMat *stackmatrix, float start, float end, int iD);
		//void transformHeirarchy(int startingPoint, int isDownstream, GLfloat segmentRot[], 
		//			GLfloat segLength[], Mesh *mesh, int segments,
      //  				glQuaternion *glQuat, int curSegment, MyMat stackMatrix, float start, float end);


};
#endif
