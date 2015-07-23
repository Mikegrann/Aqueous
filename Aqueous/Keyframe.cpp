#include "Keyframe.h"

Keyframe::Keyframe(GLuint _shaderProg, SharkQuadObject* _sharkObject, map<glm::vec3, SharkVertex*, compareVect3> *rawVerts, vector<Quad*> *rawFaces)
{
	shaderProg = _shaderProg;
	sharkObject = _sharkObject;
	//deep copy verts over.
	map<glm::vec3, SharkVertex*, compareVect3>::iterator im;
	for(im = rawVerts->begin(); im != rawVerts->end(); im++)
	{
		pair<glm::vec3, SharkVertex*> serk = *im;
		SharkVertex * d = new SharkVertex();
		d->sNormal(serk.second->gNormal());
		d->sTransformed(serk.second->gTransformed());
		d->sLocal(serk.second->gLocal());
		uVertices.insert(pair<glm::vec3, SharkVertex*>(serk.first, d));
	}

	//create faces.
	vector<Quad*>::iterator iq;
   for(iq = rawFaces->begin(); iq != rawFaces->end(); iq++)
   {
		Quad * nRect = new Quad();
		for(int i = 0; i < 4; i++)
		{
			nRect->sVert(i, uVertices.find((*iq)->gLocalVert(i))->second);
		}
		faces.push_back(nRect);
	}

	//setNormals
	createQuads();
}

void Keyframe::draw(void)
{
	vector<Quad*>::iterator iq;
	glBegin(GL_QUADS);
	int i = 0;
	for(iq = faces.begin(); iq < faces.end(); iq++)
	{
		glColor3f((*iq)->gBoneNo()*.1, 1.0-(((float) (i%3))/10.0), 1.0-((*iq)->gBoneNo()*.1));
		
		glNormal3f((*iq)->gNormalVert(0).x, (*iq)->gNormalVert(0).y, (*iq)->gNormalVert(0).z);
		glVertex3f((*iq)->gTransformedVert(0).x, (*iq)->gTransformedVert(0).y, (*iq)->gTransformedVert(0).z);
		
		glNormal3f((*iq)->gNormalVert(1).x, (*iq)->gNormalVert(1).y, (*iq)->gNormalVert(1).z);
		glVertex3f((*iq)->gTransformedVert(1).x, (*iq)->gTransformedVert(1).y, (*iq)->gTransformedVert(1).z);
		
		glNormal3f((*iq)->gNormalVert(2).x, (*iq)->gNormalVert(2).y, (*iq)->gNormalVert(2).z);
		glVertex3f((*iq)->gTransformedVert(2).x, (*iq)->gTransformedVert(2).y, (*iq)->gTransformedVert(2).z);
		
		glNormal3f((*iq)->gNormalVert(3).x, (*iq)->gNormalVert(3).y, (*iq)->gNormalVert(3).z);
		glVertex3f((*iq)->gTransformedVert(3).x, (*iq)->gTransformedVert(3).y, (*iq)->gTransformedVert(3).z);

		i++;
	}
	
	glEnd();	
}

/*Creates quaderlaterals that make up the shark, noting each quad's side faces */
void Keyframe::createQuads(void)
{
	vector<Quad*>::iterator iq;
	map<glm::vec3, SharkVertex*, compareVect3>::iterator isv;
	//O(n squared operation)
	for(int i = 0; i < faces.size(); i++)
	{
		faces[i]->sNormalVert(0, faces[i]->gNormalVert(0) + faces[i]->gNormal());
		faces[i]->sNormalVert(1, faces[i]->gNormalVert(1) + faces[i]->gNormal());
		faces[i]->sNormalVert(2, faces[i]->gNormalVert(2) + faces[i]->gNormal());
		faces[i]->sNormalVert(3, faces[i]->gNormalVert(3) + faces[i]->gNormal());
	}

	for(isv = uVertices.begin(); isv != uVertices.end(); isv++)
	{
		(isv)->second->normal /= 4.0;
	}
}

/*Rigging: Slightly adjusts quads to make sections where gaps were closed in more smooted out */
void Keyframe::multiBoneAttenuate(void)
{
	//find all the quads whose front quadrant has a different bone number, 
	//and start labeling rings from the bone edge;
	vector<Quad*> quadRing;
	vector<Quad*>::iterator iq;
	int numEdges = 5;
	int attenK = -2;
	//initializing first ring.
	for(iq = faces.begin(); iq < faces.end(); iq++)
	{
		if(*iq != NULL && (*iq)->gBack() != NULL)
		{
			if((*iq)->gBoneNo() < (*iq)->gBack()->gBoneNo())
			{
				quadRing.push_back((*iq)->gBack());
			}
		}
	}

	//main attenuate loop
	for(int i = 1; i < numEdges; i++)
	{
		for(iq = quadRing.begin(); iq < quadRing.end(); iq++)
		{
			if((*iq) != NULL)
			{
				//move vertices 1 and 2 forward
				glm::vec3 diff1 = ((*iq)->gVert(0)->transformed) - ((*iq)->gVert(1)->transformed);
				glm::vec3 diff2 = ((*iq)->gVert(3)->transformed) - ((*iq)->gVert(2)->transformed);
				double attenVal = (*iq)->attenuate(numEdges, i, attenK);	
				((*iq)->gVert(1)->transformed) = ((*iq)->gVert(1)->transformed) + (diff1 * (float)attenVal);
				((*iq)->gVert(2)->transformed) = ((*iq)->gVert(2)->transformed) + (diff2 * (float)attenVal);
			}
		}
		//move to the next ring
		for(int j = 0; j < quadRing.size(); j++)
		{
			if(quadRing[j] != NULL && quadRing[j]->gBack() != NULL)
				quadRing[j] = quadRing[j]->gBack();
		}
	}
}

/*linear interpolation funcion */
glm::vec3 Keyframe::interpolateVertices(glm::vec3 first, glm::vec3 second, int step, int max)
{
	//TODO: replace Lerp with Slerp if necessary
	glm::vec3 fin = first;
	fin = fin + ((second-first)*((float)step))/((float)max);
	return fin;	
}

/*Draws in inbetweener frame given a target keyframe, its interpolation step and the total (max) interpolation steps
* This keyframe is the starting point 
* Outputs in OpenGL to the screen*/
void Keyframe::drawInBetween(Keyframe end, int step, int max)
{
	// create buffers with updated vertex positions
	/*vector<glm::vec3> lerpPositions, lerpNormals;
	lerpPositions.clear();
	lerpNormals.clear();

	for(int i = 0; i < faces.size(); i++) { 
		for (int j = 0; j < 4; ++j) {
			lerpPositions.push_back(interpolateVertices((faces[i]->gTransformedVert(j)), (end.faces[i]->gTransformedVert(j)), step, max));
			lerpNormals.push_back(interpolateVertices((faces[i]->gNormalVert(j)), (end.faces[i]->gNormalVert(j)), step, max));
		}
	}

	// pass buffers to draw object...
	sharkObject->update(lerpPositions, lerpNormals);

	// fetch shader variable IDs
	GLuint h_MV = GLSL::getUniformLocation(shaderProg, "MV");
	GLuint h_P = GLSL::getUniformLocation(shaderProg, "P");
	GLuint h_lightPos = GLSL::getUniformLocation(shaderProg, "lightPos");
	GLuint h_ka = GLSL::getUniformLocation(shaderProg, "ka");
	GLuint h_kd = GLSL::getUniformLocation(shaderProg, "kd");
	GLuint h_ks = GLSL::getUniformLocation(shaderProg, "ks");
	GLuint h_s = GLSL::getUniformLocation(shaderProg, "s");

	// update shader variables
	glUniform3fv(h_lightPos, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
   glUniform3fv(h_ka, 1, glm::value_ptr(SHARK_COL));
   glUniform3fv(h_kd, 1, glm::value_ptr(SHARK_COL));
   glUniform3fv(h_ks, 1, glm::value_ptr(glm::vec3(1.0f, 0.9f, 0.8f)));
   glUniform1f(h_s, 200.0f);

   float proj[16];
   float modl[16];

	// Get the current PROJECTION matrix from OpenGL
	glGetFloatv( GL_PROJECTION_MATRIX, proj );

	// Get the current MODELVIEW matrix from OpenGL
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

   glUniformMatrix4fv(h_P, 1, GL_FALSE, proj);
   glUniformMatrix4fv(h_MV, 1, GL_FALSE, modl);

   GLuint h_vertPos = GLSL::getAttribLocation(shaderProg, "vertPos");
   GLuint h_vertNor = GLSL::getAttribLocation(shaderProg, "vertNor");

   // draw the shark in the draw object
   sharkObject->draw(h_vertPos, h_vertNor);*/

}

/*Deallocate heap space before exiting the program */
void Keyframe::deleteHeap()
{
	map<glm::vec3, SharkVertex*, compareVect3>::iterator iv;
	vector<Quad*>::iterator iq;

	for(iv = uVertices.begin(); iv != uVertices.end(); iv++)
	{
		delete (*iv).second;
	}
	for(iq = faces.begin(); iq < faces.end(); iq++)
	{
		delete (*iq);
	}
}
