#include "SharkQuadObject.h"

SharkQuadObject::SharkQuadObject(GLuint _shadeProg, SharkMesh* sMesh) {

   // initialize shader program id
   shadeProg = _shadeProg;

   // initialize IDs
   posBufID = 0;
   norBufID = 0;
   indBufID = 0;

   // initialize buffer objects
   vertBuf.clear();
   posBuf.clear();
   norBuf.clear();
   texBuf.clear();
   indBuf.clear();
   // load in buffer objects
   int cursor = 0;
   for (int i = 0; i < (int)sMesh->faces.size(); ++i) {
       for (int j = 0; j < 4; ++j) {
         SVertex vert = SVertex();
         // push positions
         vert.Position = glm::vec3(sMesh->faces[i]->gLocalVert(j).x, sMesh->faces[i]->gLocalVert(j).y, sMesh->faces[i]->gLocalVert(j).z);
         posBuf.push_back(sMesh->faces[i]->gLocalVert(j).x);
         posBuf.push_back(sMesh->faces[i]->gLocalVert(j).y);
         posBuf.push_back(sMesh->faces[i]->gLocalVert(j).z);
         // push normals
         vert.Normal = glm::vec3(sMesh->faces[i]->gNormalVert(j).x, sMesh->faces[i]->gNormalVert(j).y, sMesh->faces[i]->gNormalVert(j).z);
         norBuf.push_back(sMesh->faces[i]->gNormalVert(j).x);
         norBuf.push_back(sMesh->faces[i]->gNormalVert(j).y);
         norBuf.push_back(sMesh->faces[i]->gNormalVert(j).z);

         vertBuf.push_back(vert);
      }

      // triangulate quad
      indBuf.push_back((unsigned int)cursor);
      indBuf.push_back((unsigned int)(cursor + 1));
      indBuf.push_back((unsigned int)(cursor + 3));
      indBuf.push_back((unsigned int)(cursor + 1));
      indBuf.push_back((unsigned int)(cursor + 2));
      indBuf.push_back((unsigned int)(cursor + 3));

      cursor += 4;
   }
} 

SharkQuadObject::SharkQuadObject(GLuint _shadeProg, vector<glm::vec3> positions, vector<glm::vec3> normals, vector<glm::vec2> texCoords) {

   // initialize shader program id
   shadeProg = _shadeProg;

   // initialize IDs
   posBufID = 0;
   norBufID = 0;
   indBufID = 0;

   // initialize buffer objects
   posBuf.clear();
   norBuf.clear();
   texBuf.clear();
   indBuf.clear();
   // load in buffer objects
   /*int cursor = 0;
   for (int i = 0; i < (int)sMesh->faces.size(); ++i) {
      for (int j = 0; j < 4; ++j) {
         // push positions
         posBuf.push_back(sMesh->faces[i]->gLocalVert(j).x);
         posBuf.push_back(sMesh->faces[i]->gLocalVert(j).y);
         posBuf.push_back(sMesh->faces[i]->gLocalVert(j).z);
         // push normals
         norBuf.push_back(sMesh->faces[i]->gNormalVert(j).x);
         norBuf.push_back(sMesh->faces[i]->gNormalVert(j).y);
         norBuf.push_back(sMesh->faces[i]->gNormalVert(j).z);
      }

      // triangulate quad
      indBuf.push_back((unsigned int)cursor);
      indBuf.push_back((unsigned int)(cursor + 1));
      indBuf.push_back((unsigned int)(cursor + 3));
      indBuf.push_back((unsigned int)(cursor + 1));
      indBuf.push_back((unsigned int)(cursor + 2));
      indBuf.push_back((unsigned int)(cursor + 3));

      cursor += 4;
   }*/
} 

void SharkQuadObject::init() {
   // generate buffer IDs and objects
   glGenBuffers(1, &posBufID);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);

   if (!norBuf.empty()) {
      glGenBuffers(1, &norBufID);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID);
      glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_DYNAMIC_DRAW);
   }
   else {
      norBufID = 0;
   }

   // Send the texture coordinates array (if it exists) to the GPU
   if(!texBuf.empty()) {
      glGenBuffers(1, &texBufID);
      glBindBuffer(GL_ARRAY_BUFFER, texBufID);
      glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
   }
   else {
      texBufID = 0;
   }

   glGenBuffers(1, &indBufID);
   glBindBuffer(GL_ARRAY_BUFFER, indBufID);
   glBufferData(GL_ARRAY_BUFFER, indBuf.size()*sizeof(float), &indBuf[0], GL_STATIC_DRAW);

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   assert(glGetError() == GL_NO_ERROR);
}

void SharkQuadObject::update(vector<glm::vec3> newPositions, vector<glm::vec3> newNormals) {
    vertBuf.clear();
   posBuf.clear();
   norBuf.clear();
   for (int i = 0; i < (int)newPositions.size(); ++i) {
       SVertex vert = SVertex();
       vert.Position = glm::vec3(newPositions[i].x, newPositions[i].y, newPositions[i].z);
       vert.Normal = glm::vec3(newNormals[i].x, newNormals[i].y, newNormals[i].z);
       vertBuf.push_back(vert);

      // push positions
      posBuf.push_back(newPositions[i].x);
      posBuf.push_back(newPositions[i].y);
      posBuf.push_back(newPositions[i].z);
      // push normals
      norBuf.push_back(newNormals[i].x);
      norBuf.push_back(newNormals[i].y);
      norBuf.push_back(newNormals[i].z);
   }

   // send VBOs to the GPU
  /* glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, norBufID);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_DYNAMIC_DRAW);*/
}

void SharkQuadObject::draw(GLint h_pos, GLint h_nor) const
{
   // Enable and bind position array for drawing
  /* GLSL::enableVertexAttribArray(h_pos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Enable and bind normal array (if it exists) for drawing
   if(norBufID && h_nor >= 0) {
      GLSL::enableVertexAttribArray(h_nor);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID);
      glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   }
   
   // Bind index array for drawing
   int nIndices = (int)indBuf.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
   
   // Draw
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   
   // Disable and unbind
   if(norBufID && h_nor >= 0) {
      GLSL::disableVertexAttribArray(h_nor);
   }
   GLSL::disableVertexAttribArray(h_pos);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
}