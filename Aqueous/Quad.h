#ifndef _QUADERLATERIAL_3D_THING
#define _QUADERLATERIAL_3D_THING
#include <ionEngine.h>
#include "SharkVertex.h"
#include "MyMat.h"
#include "CSplineFunctions.h"

/*A quad is a set of four points that make a rectangle. */
class Quad
{
	public:
		Quad(){front = NULL; back = NULL; left = NULL; right = NULL; verts[0] = NULL; 
			verts[1] = NULL; verts[2] = NULL; verts[3] = NULL; 
		}
		
		Quad(SharkVertex *one, SharkVertex *two, SharkVertex  *three, SharkVertex *four)
			{front = NULL; back = NULL; left = NULL; right = NULL; verts[0] = one; 
			verts[1] = two; verts[2] = three; verts[3] = four;
		}
		
		void locateAdjacants(vector<Quad*> lisq);
		bool compareLeft(Quad oth);
		bool compareRight(Quad oth);
		bool compareFront(Quad oth);
		bool compareBack(Quad oth);
		glm::vec3 calcNormal();	
		static double attenuate(int numedges, int iteration, int k);		

		//inside class manipulation
		SharkVertex *gVert(int index){return verts[index];}   //index is the vertex number on the quad
		void sVert(int index, SharkVertex* n){verts[index] = n;}   //index is the vertex number on the quad
		glm::vec3 gLocalVert(int index){return verts[index]->gLocal();}
		glm::vec3 gTransformedVert(int index){return verts[index]->gTransformed();}
		glm::vec3 gNormalVert(int index){return verts[index]->gNormal();}
		void sNormalVert(int index, glm::vec3 vert){verts[index]->sNormal(vert);}
		void sTransformedVert(int index, glm::vec3 vert){verts[index]->sTransformed(vert);}
		void sLocalVert(int index, glm::vec3 vert){verts[index]->sLocal(vert);}
	
		void matrixTransform(MyMat stackmatri);  //rigid body transformation
		//void linearBlendTransform(MyMat stackmatri, string boneName);   //skinned transformation 
		void restPosition();  //returns quad to rest position (clears transformations). 
		

		glm::vec3 gNormal(){return faceNormal;}	
		void sNormal(glm::vec3 newNormal){faceNormal = newNormal;}

		int gBoneNo(){return boneNo;}
		void sBoneNo(int bone){boneNo = bone;}
		float checkBone(string boneName);

		Quad* gFront(){return front;}	
		Quad* gBack(){return back;}	
		Quad* gLeft(){return left;}	
		Quad* gRight(){return right;}	
		void sFront(Quad* n){front = n;}	
		void sBack(Quad* n){back = n;}	
		void sLeft(Quad* n){left = n;}	
		void sRight(Quad* n){right = n;}	

	private:		
		SharkVertex* verts[4];
		glm::vec3 faceNormal;
		int boneNo;
		Quad* front;
		Quad* back;
		Quad* left;
		Quad* right;
};

#endif
