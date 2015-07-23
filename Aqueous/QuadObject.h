#ifndef __QuadObject__
#define __QuadObject__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <map>
#include "GLSL.h"
#include "Quad.h"
#include "SharkMesh.h"

class QuadObject {
public:
   QuadObject(GLuint _shadeProg, SharkMesh* sMesh);
   QuadObject(GLuint _shadeProg, vector<Vector3f> positions, vector<Vector3f> normals, vector<Vector2f> texCoords);
   ~QuadObject() {};

   void init();
   void update(vector<Vector3f> newPositions, vector<Vector3f> newNormals);
   void setTexCoords(vector<Vector3f> texCoords);
   void draw(GLint h_pos, GLint h_nor) const;


private:
   GLuint shadeProg;

   unsigned posBufID;
   unsigned norBufID;
   unsigned texBufID;
   unsigned indBufID;

   vector<float> posBuf;
   vector<float> norBuf;
   vector<float> texBuf;
   vector<unsigned int> indBuf;

};

#endif