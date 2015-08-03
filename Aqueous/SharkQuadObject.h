#pragma once

#include <ionEngine.h>
#include "SharkMesh.h"

class SharkQuadObject {
public:
   SharkQuadObject(GLuint _shadeProg, SharkMesh* sMesh);
   SharkQuadObject(GLuint _shadeProg, vector<glm::vec3> positions, vector<glm::vec3> normals, vector<glm::vec2> texCoords);
   ~SharkQuadObject() {};

   void init();
   void update(vector<glm::vec3> newPositions, vector<glm::vec3> newNormals);
   void setTexCoords(vector<glm::vec3> texCoords);
   void draw(GLint h_pos, GLint h_nor) const;

   inline vector<SVertex> getVertBuf() {  return vertBuf; };
   inline vector<float> getPosBuf() { return posBuf; };
   inline vector<float> getNorBuf() { return norBuf; };
   inline vector<float> getTexBuf() { return texBuf; };
   inline vector<unsigned int> getIndBuf() { return indBuf; };

private:
   GLuint shadeProg;

   unsigned posBufID;
   unsigned norBufID;
   unsigned texBufID;
   unsigned indBufID;

   vector<SVertex> vertBuf;

   vector<float> posBuf;
   vector<float> norBuf;
   vector<float> texBuf;
   vector<unsigned int> indBuf;

};