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