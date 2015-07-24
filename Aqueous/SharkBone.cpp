#include "SharkBone.h"

/*builds a bone from an aobj file. Copies over the vertices. Does not set child bones; That has to be done separate 
 * The shark mesh should be loaded already*/
void SharkBone::buildBoneAOBJ(string bName, glm::vec3 headpt, glm::vec3 tailpt )
{
	boneName = bName;
	headPoint = headpt;
	tailPoint = tailpt;

	//length of bone
	//boneLength = headpt.EuclDistance(tailpt);
    boneLength = glm::distance(headpt, tailpt);
	
	//get rotation
	glm::vec3 boneVec = headpt - tailpt;
	//glm::vec3 boneVec = tailpt - headpt;
	//glm::vec3 rot = boneVec.Normalize();
    glm::vec3 rot = glm::normalize(boneVec);
	//float theta = acos(rot.Dot(glm::vec3(1,0,0)));
    float theta = acos(glm::dot(rot, glm::vec3(1.0f, 0.0f, 0.0f)));
        changeAngle(theta);	
}


/*creates translation matrices for this bone and all child bones */
//void SharkBone::buildTranslation(glm::vec3 root, glm::vec3 headL, glm::vec3 tailL)
void SharkBone::buildTranslation()
{
	glm::vec3 locTrans = (headPoint - (headPoint-tailPoint))*-1.0f;
	transMatLocal.makeTranslate(locTrans);   //skin space to joint space
	
	glm::vec3 transHeir = headPoint-tailPoint;
	transHeir = transHeir * -1.0f;
	transMatHeir.makeTranslate(transHeir);   //joint space to shark space

	vector<SharkBone*>::iterator ic;
	for(ic = childBones.begin(); ic != childBones.end(); ic++)
	{
		//(*ic)->buildTranslation(root, headPoint, tailPoint);
		(*ic)->buildTranslation();
	}
}

/*Creates a translation matrix from the bonelength and direction. 
 *Because this is the CalShark loader, the translation would be in the x direction only. */
void SharkBone::boneLengthToTranslation(bool downstream)
{
	double xtrans = downstream ? boneLength : -boneLength;
	double xend = downstream ? -endB : -startB;	//start and end points on the bone.

	transMatHeir.makeTranslate(glm::vec3(xtrans, 0, 0));
	transMatLocal.makeTranslate(glm::vec3(xend, 0, 0));
}

/*This changes the angles for this bone.
 * This is the cal shark version, that needs to know whether the bone is ahead of the root */
void SharkBone::changeAngle(int newAngle, bool isAheadRoot)
{
	angleOfRot = newAngle;

	GLfloat glm[16];
	rotatQ.CreateFromAxisAngle(0,1,0, isAheadRoot ? -angleOfRot : angleOfRot);
	rotatQ.Normalize();
	rotatQ.CreateMatrix(glm);
	rotationMatrix = MyMat(glm[0], glm[4], glm[8], glm[12], glm[1], glm[5],glm[9],
			glm[13],glm[2],glm[6],glm[10],glm[14],glm[3],glm[7],
			glm[11],glm[15]);
}

/*Change the angle of this bone.
 * This is the general case of this function. Cal Shark needs the other one. */
void SharkBone::changeAngle(int newAngle)
{
	changeAngle(newAngle, false);
}

/*sets the rotation based on an existing quaternion */
void SharkBone::changeAngle(glQuaternion newAngle)
{
	GLfloat glm[16];
	newAngle.Normalize();
	newAngle.CreateMatrix(glm);	
	rotationMatrix = MyMat(glm[0], glm[4], glm[8], glm[12], glm[1], glm[5],glm[9],
			glm[13],glm[2],glm[6],glm[10],glm[14],glm[3],glm[7],
			glm[11],glm[15]);

}


/*creates the matrices. Advances matrix hierarchy and derives the combined heirarchical matrix and binding matrix
 * returns: the binding matrix that is not tied tot he hierarchy */
MyMat SharkBone::createTransforms(MyMat *stackMatrix)
{
	//current shark model segment
	MyMat Matrix = *stackMatrix;
	MyMat secondStack = MyMat();

	Matrix = Matrix.multRight(jointTrans);
	Matrix = Matrix.multRight(rotationMatrix); //roatation goes before translates
	Matrix = Matrix.multRight(transMatHeir); 
	
	*stackMatrix = Matrix; //advance heirarchy, without applying the below local translation to the whole stack
	Matrix = Matrix.multRight(transMatLocal);

	return Matrix;
}

/*Matrix multiplies the smark SharkMesh
 * pass down the hierarchical model matrix and a value  */
void SharkBone::transformBone(MyMat *stackMatrix, bool rigidBody)
{
	//current shark model segment
	MyMat Matrix = createTransforms(stackMatrix); 

	if(rigidBody){ 
		vector<Quad*>::iterator iq;
		//transform each quad in the mesh
		for(iq = quads.begin(); iq < quads.end(); iq++)
		{
			(*iq)->matrixTransform(Matrix);
			
		}
	}
	else {
		sMesh->setSkinMatrix(boneName, Matrix);
		//Matrix.printDiagonalMag();
		//cout << Matrix << endl;
	}

	//recursive transform downwards to child bones. 
	//The matrix is copied so that child changes don't propagate upstream  
	vector<SharkBone*>::iterator ib; 
	for(ib = childBones.begin(); ib != childBones.end(); ib++ )
	{
		MyMat tmp = MyMat(*stackMatrix);	
		(*ib)->transformBone(&tmp, rigidBody);
	}

}

void SharkBone::drawTri(MyMat matrix)
{
	//glm::vec3 perpen = headPoint.Cross(tailPoint);
    glm::vec3 perpen = glm::cross(headPoint, tailPoint);
	//perpen = perpen / perpen.Magnitude();
    perpen = perpen / glm::length(perpen);
	glm::vec3 tailV = glm::vec3(matrix.multVec(tailPoint, true));
	glm::vec3 headVa = glm::vec3(matrix.multVec(headPoint, true)) + (perpen*.15f);
	glm::vec3 headVb = glm::vec3(matrix.multVec(headPoint, true)) - (perpen*.15f);

	glColor3f(0,1.0,0);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(tailV.x, tailV.y, tailV.z);	
		glVertex3f(headVa.x, headVa.y, headVa.z);	
		glVertex3f(headVb.x, headVb.y, headVb.z);	
	} glEnd();
	glColor3f(1,1,1);

}

void SharkBone::draw(MyMat *stackmatrix)
{
	MyMat Matrix = createTransforms(stackmatrix);

	//current shark model segment
	drawTri(Matrix);

	//recursive transform downwards to child bones. 
	//The matrix is copied so that child changes don't propagate upstream  
	vector<SharkBone*>::iterator ib; 
	
	for(ib = childBones.begin(); ib != childBones.end(); ib++ )
	{
		MyMat tmp = MyMat(*stackmatrix);	
		(*ib)->draw(&tmp);
	}
}
