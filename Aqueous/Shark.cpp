#include "Shark.h"

Shark::Shark()
{
	this->showSkin = true;
	this->showSpine = false;
	this->ismoving = false;
	this->play = 0;

	this->segPercent = 0;
	this->totalLength = 4;
	this->lengthLeft = 4;

    this->sequencesParsed = 0;
	this->tempSegments = 10;
	this->segNum = 1;
	this->tempSegPercent = 0;

	//mesh.init_blender(); //global var
	//this->totalLength = mesh.blenderTotalLength;
}

materialStruct RedFlat = {
	{0.3f, 0.0f, 0.0f, 1.0f},
	{0.9f, 0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f}
};
materialStruct White = {
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{0.0}
};
materialStruct Black = {
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0.0}
};
materialStruct GreenFlat = {
	{0.0f, 0.3f, 0.0f, 1.0f},
	{0.0f, 0.9f, 0.0f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f}
};
materialStruct BlueFlat = {
	{0.0f, 0.0f, 0.3f, 1.0f},
	{0.0f, 0.0f, 0.9f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f}
};
materialStruct PurpleFlat = {
	{0.3f, 0.0f, 0.3f, 1.0f},
	{0.9f, 0.0f, 0.9f, 1.0f},
	{0.0f, 0.0f, 0.0f, 1.0f},
	{0.0f}
};


materialStruct Grey = {
	{0.25f, 0.25f, 0.25f, 0.25f},
	{0.25f, 0.25f, 0.25f, 0.25f},
	{0.25f, 0.25f, 0.25f, 0.25f},
	{0.0}
};

/*void Shark::drawSharkSeg(float rotate, float start, float end, int index, float length)
  {
  GLfloat Matrix[16];

  glQuat.CreateFromAxisAngle(0,1,0,rotate);
  glQuat.CreateMatrix(Matrix);
  glMultMatrixf(Matrix);

  glTranslatef(-segLength[index], 0, 0);
  glPushMatrix();
  glTranslatef(-start, 0, 0);
  mesh.drawBetween(start, end);

  glPopMatrix();
  }*/

KeyframeSystem Shark::genKeyframes(bool dynamicMode, SharkMesh *shm)
{
	if(dynamicMode)
	{
      printf("in nondynamic mode\n");
      SharkQuadObject* sharkObject = new SharkQuadObject(shaderProg, shm);
      sharkObject->init();   

		kfSys = KeyframeSystem(shaderProg, sharkObject, true);
		kfSys.setSharkMesh(shm);
		//animation sequence #1 is the slow swim. TODO, less magic nubmers
		int numAngles = segments;
		skeleton.buildAnimations(numAngles, *(segmentRot[1]), numAngles);
		skeleton.update(0, 0, glm::vec3(0,0,0)); //force one update cycle to happen before startup, initializes the system this way.
		kfSys.initFrames();
	}
	else
	{
      //printf("holy shit!!!!\n");
		/*kfSys = KeyframeSystem(shaderProg, false);

		int segs = segments;

		for(int k = 0; k < sequencesParsed; k++)
		{
			GLfloat segRot2[segments];
			FrameSequence curSeq;
			for(int j = 0; j < segments; j++)
			{
			   Keyframe curFrame = Keyframe(shaderProg);
            //Keyframe curFrame = Keyframe();
            //curFrame.setShader(shaderProg);

				for(int i = 0; i < segments;i++)
				{
					segRot2[i] = segmentRot[k][i][j];
				}
				curFrame.gatherTransformData(segRot2, 
						segLength, 
						&mesh, 
						segs, 
						&glQuat);
				curSeq.sFrame(curFrame);
			}
			kfSys.insertStaticSequence(curSeq);
		}*/
	}

	return kfSys;	
}

/*Draws the outside shark shape of the shark, with the option to show its skeleton instead =* */
void Shark::drawSkin(int frame)
{
	//float startSeg = mesh.lengthMax, endSeg = mesh.lengthMax, length = 0;
	GLfloat rotate;
	glPushMatrix();

	if(showSkin)
	{
		materials(Grey);
		kfSys.draw();
	}
	else
	{
		materials(White);
		skeleton.draw();
	}

	glPopMatrix();
}

void Shark::drawShark(int frame, GLUquadricObj *quadratic) 
{
	drawSkin(frame);
}

/*The general update function that is to be called regularly and predicitibly.
 * It needs to know what the curvature of the path is. */
//void Shark::timedUpdate(int railAngle)
void Shark::timedUpdate(int dt, int railAngle, glm::vec3 vel)
{
	kfSys.update();
	velocity = vel;
	skeleton.update(dt, railAngle, velocity); 
}

void Shark::materials(materialStruct materials) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}

double doubleLerpFrames(double input, double minx, double maxx)
{
	double maxy = 20.0;
	return 4.0 + (input-minx) * ((maxy-4.0)/(maxx-minx));
}


void Shark::updateVelocity(glm::vec3 start, glm::vec3 end, double dt)
{
	//velocity = start-end;
	//double dist = (end-start).Magnitude();
    double dist = glm::length(end - start);
	double unitTime = dt / dist;
	int Lframe = doubleLerpFrames(unitTime, 0.1, 10.0); //+1;

	kfSys.adjNumFrames(Lframe);
	//printf("%f\n", unitTime);

}

/* parses CSV files containing animation data. Not all CSV's are animation. Some of them are world data points. */
void Shark::readMovementData(const char* file, bool dynaMode)
{
	FILE * fp;
	fp = fopen(file, "r");
	if(fp == NULL)
	{
		printf("can't open file %s\n", file);
		exit(-1);
	}

    for (int ind = 0; ind < sequences; ++ind) {
        for (int j = 0; j < segments; ++j) {
            for (int k = 0; k < segments; ++k) {
                segmentRot[ind][j][k] = 0.0f;
            }
        }
    }

	for(int i = 0; i < segments; i++)
	{
		for(int j = 0; j < segments; j++)
		{
            printf("trying to force stuff into [%d][%d][%d]\n", gParsedSoFar(), i, segments - 1 - j);
			float segr;
			//read in file. Note that CALShark writes segment data (j value) backwards
			if(!dynaMode)
			{
				fscanf(fp, "%f,", &segmentRot[gParsedSoFar()][i][segments-1-j]);
			}
			else
			{
				fscanf(fp, "%f,", &segmentRot[gParsedSoFar()][j][segments-1-i]);
			}
		}
	}
	fclose (fp);
	incrementSequences();
}

/*default shark animation sequence creator. Makes a shark that stays stiff */
void Shark::defSequence()
{
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < segments; j++)
		{
			segmentRot[0][i][j] = 0;
		}
	}
	incrementSequences();

}