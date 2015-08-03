#include "SharkMesh.h"

string SharkMesh::nextToken(char delimit, FILE* readFile)
{
	string numb;
	while(!feof(readFile))
	{
		if(ferror(readFile)){ printf("1Error reading FILE\n"); exit(-1); }
		char cur = fgetc(readFile);
		if(ferror(readFile)){ printf("2Error reading FILE\n"); exit(-1);}
		if(cur == delimit || cur == '\n') {break;}
		numb.append(1, cur);
	}
	return numb;
}

//FILE* SharkMesh::buildAOBJ(string filename)
string SharkMesh::buildAOBJ(ifstream& readFile)
{
	vector<glm::vec3> localVerts = vector<glm::vec3>();   //in order to use OBJs as keys, need to remember their order.
	//while(!feof(readFile))
    string line = " ";
    while (getline(readFile, line))
    //while (readFile.good())
	{
        //readFile >> 
		//tokenize the line identifier. It's only one character and 
		//it should be on the first on the line
		//char identifier = fgetc(readFile);
        //char identifier = line.c_str()[0];
        istringstream iss(line);
        char identifier = ' ';
        iss >> identifier;

		///if(ferror(readFile)){ printf("888888888Error reading FILE\n"); exit(-1);}


		//load in the vertices
		//v x y z nx ny nz boneName/weight boneName/weight ...
		if(identifier == 'v')
		{
			//char cur = fgetc(readFile);  //space
			//if(ferror(readFile)){ printf("0Error reading FILE\n"); exit(-1);}
			//whsile(cur != '\n')  //per line
			//{
			SharkVertex* sv = new SharkVertex();
            float pos_x = 0.0f;
            float pos_y = 0.0f;
            float pos_z = 0.0f;
            float nor_x = 0.0f;
            float nor_y = 0.0f;
            float nor_z = 0.0f;
            string bone_pair = " ";
            //char 
            //float bone_weight = 0.0f;

            iss >> pos_x >> pos_y >> pos_z >> nor_x >> nor_y >> nor_z;
            
            while (iss >> bone_pair) {
                
                string bone_name = " ";
                string weight_string = " ";
                float bone_weight = 0.0f;

                bone_name = strtok((char*)bone_pair.c_str(), " /\n");
                weight_string = strtok(NULL, " /\n");
                bone_weight = atof(weight_string.c_str());

                if (bone_name != "" && weight_string != "") {
                    sv->sBonePair(bone_name, bone_weight);
                }
            }
                
               // sscanf(line.c_str(), "v %f %f %f %f %f %f %s/%f\n", &pos_x, &pos_y, &pos_z, &nor_x, &nor_y, &nor_z, &bone_name, &bone_weight);
                
            glm::vec3 vert = glm::vec3(pos_x, pos_y, pos_z);
            sv->sLocal(vert);
             
            glm::vec3 nor = glm::vec3(nor_x, nor_y, nor_z);
            sv->sNormal(nor * -1.0f);

            sv->sTransformed(glm::vec3(0, 0, 0));

            localVerts.push_back(vert);
            insertVec(pair<glm::vec3, SharkVertex*>(vert, sv));


				/*//location of vertex    
				glm::vec3 vert;
				vert.x = atof(nextToken(' ', readFile).c_str());
				vert.y = atof(nextToken(' ', readFile).c_str());
				vert.z = atof(nextToken(' ', readFile).c_str());
				sv->sLocal(vert);

				glm::vec3 nor; //normal
				nor.x = atof(nextToken(' ', readFile).c_str());
				nor.y = atof(nextToken(' ', readFile).c_str());
				nor.z = atof(nextToken(' ', readFile).c_str());
				sv->sNormal(nor*-1.0f);  //TODO magic.  fix blender normals. shouldn't need to reverse them.

				sv->sTransformed(glm::vec3(0,0,0));
				//bone / weight repeats
				fseek(readFile, -1, SEEK_CUR);
				cur = fgetc(readFile);
				while(cur != '\n')
				{
					string boneName = nextToken('/', readFile);
					float weight = atof(nextToken(' ', readFile).c_str());
					sv->sBonePair(boneName, weight);
					fseek(readFile, -1, SEEK_CUR);
					cur = fgetc(readFile);
				}
				localVerts.push_back(vert);
				insertVec(pair<glm::vec3, SharkVertex*>(vert, sv));
			}*/
		}
		//faces
		//f ... vertices in mesh .....
		else if(identifier == 'f')
        {
            int vertex1 = 0;
            int vertex2 = 0;
            int vertex3 = 0;
            int vertex4 = 0;

            iss >> vertex1 >> vertex2 >> vertex3 >> vertex4;

            Quad *curQuad = new Quad();
            curQuad->sNormal(glm::vec3(0, 0, 0));

            curQuad->sVert(0, gVertex(localVerts[vertex1 - 1]));
            curQuad->sVert(1, gVertex(localVerts[vertex2 - 1]));
            curQuad->sVert(2, gVertex(localVerts[vertex3 - 1]));
            curQuad->sVert(3, gVertex(localVerts[vertex4 - 1]));
            curQuad->calcNormal();

            pushFace(curQuad);
            //neighboring quads are to be found later, after parsing is done.

			/*char cur = fgetc(readFile); //space
			if(ferror(readFile)){ printf("4Error reading FILE\n"); exit(-1);}
			Quad *curQuad = new Quad();
			curQuad->sNormal(glm::vec3(0,0,0));

			//caution. Vertices listed in mesh may not be consistant with other quads 
			int vertex1 = atoi(nextToken(' ', readFile).c_str());
			int vertex2 = atoi(nextToken(' ', readFile).c_str());
			int vertex3 = atoi(nextToken(' ', readFile).c_str());
			int vertex4 = atoi(nextToken(' ', readFile).c_str());

			curQuad->sVert(0, gVertex(localVerts[vertex1-1]));
			curQuad->sVert(1, gVertex(localVerts[vertex2-1]));
			curQuad->sVert(2, gVertex(localVerts[vertex3-1]));
			curQuad->sVert(3, gVertex(localVerts[vertex4-1]));
			curQuad->calcNormal();

			pushFace(curQuad);	
			//neighboring quads are to be found later, after parsing is done.

			fseek(readFile, -1, SEEK_CUR);
			cur = fgetc(readFile);
			while(cur != '\n')
			{
				cur = fgetc(readFile);
				if(ferror(readFile)){ printf("5Error reading FILE\n"); exit(-1);}
			}*/
		}		
		//bones
		//b name headRestArmature tailRestArmature ... child names ...
		//read outside of this function	
		else if(identifier == 'b')
		{
			return line;
		}
	}
}


/*Restores vertex transformation back to rest pose (joint space) */
void SharkMesh::restPosition()
{
	map<glm::vec3, SharkVertex*, compareVect3>::iterator im;
	for(im = vertices.begin(); im != vertices.end(); im++ )
	{
		//im->second->transformed = im->second->local;
		im->second->transformed = glm::vec3(0,0,0);//im->second->local;
	}

	skinTransforms = map<string, MyMat>();  //wipe out bone information
}

/*Skinning transform of a quad with a transformation and the name of the bone that created that transformation.
 *  * Vertex weights taken from the bone name affect the strength of the  */
void SharkMesh::linearBlendTransform(MyMat matrix, string boneName)
{
	map<glm::vec3, SharkVertex*, compareVect3>::iterator im;
	for(im = vertices.begin(); im != vertices.end(); im++ )
	{
		float weight = (*im).second->checkBone(boneName);
		if(weight > 0.001)
		{
			(*im).second->transformed += 
				matrix.multScalar(weight).multVec((*im).second->local, true);
		}
	}
}

/*Skinning transform of a quad with a transformation and the name of the bone that created that transformation.
 *  * Vertex weights taken from the bone name affect the strength of the  */
void SharkMesh::linearBlendTransform()
{
	map<glm::vec3, SharkVertex*, compareVect3>::iterator im;
	for(im = vertices.begin(); im != vertices.end(); im++ )
	{
		//float bbr = 0;
		string boneName;
		map<string, MyMat>::iterator ib;
		for(ib = skinTransforms.begin(); ib != skinTransforms.end(); ib++)
		{
			float weight = (*im).second->checkBone((*ib).first);
			//if(weight > 0.0001)
			//{
		    MyMat matrix = gSkinMatrix(ib->first); 
			MyMat scale = MyMat(); //weight mess ups
			//float sk = 1.0/matrix.diagonalMagnitude();
			//scale.makeScale(glm::vec3(sk,sk,sk));
			//matrix.multRight(scale);
			(*im).second->transformed += 
			    matrix.multScalar(weight).multVec((*im).second->local, true);
				//bbr += weight;
			//}
		}
		//printf("%f\n", bbr);
	}
}

/*Adjusts bone weights so they always add up to one */
void SharkMesh::countWeights()
{
	map<glm::vec3, SharkVertex*, compareVect3>::iterator im;
	for(im = vertices.begin(); im != vertices.end(); im++ )
	{
		float summ = 0;
		for(int i = 0; i < im->second->gNumInfluences(); i++) //count weights
		{
			summ += im->second->gBoneWeight(i);
		}
		for(int i = 0; i < im->second->gNumInfluences(); i++) //normalize weights
		{
			im->second->sBoneWeight(i, im->second->gBoneWeight(i)/summ);
		}
	}
}


//TODO delete Heap properly.
void SharkMesh::deleteHeap()
{
	map<glm::vec3, SharkVertex*, compareVect3>::iterator im;
	for(im = vertices.begin(); im != vertices.end(); im++ )
	{
		//delete each element
		delete im->second;
	}
}