
#include "CSharkNodeManager.h"
#include <ionWindow.h>
#include <glm/gtc/type_ptr.hpp>

CSharkNodeManager::~CSharkNodeManager() {
	delete mesh;
}

void CSharkNodeManager::Init()
{
    currSite = nullptr;
    world = nullptr;
    isMoving = true;
    toStep = true;
	writeGif = false;

	SingletonPointer<CSceneManager> SceneManager;
	
	Node = SceneManager->GetFactory()->AddSceneNode("Shark");
    //Node = SceneManager->GetFactory()->AddSceneNode("Glyph");
    
    TestMesh = SceneManager->GetMeshLibrary()->Get("Sphere");
    
    shark.defSequence();

    string artistDrivenAnimations[5] = { "ani/ani_slowstraight_4.csv",
        "ani/ani_faststraight_1.csv",
        "ani/ani_leftturn_2.csv",
        "ani/ani_rightturn_2.csv",
        "ani/ani_right_uturn_1.csv" };

    for (int i = 0; i < 5; i++) {
        shark.readMovementData(artistDrivenAnimations[i].c_str(), true);
    }

    mesh = new SharkMesh();

    shark.sMesh(mesh);
    int tSegments = shark.segments;

    shark.buildSkeleton("Model/LeopardShark.aobj");

    shark.genKeyframes(true, mesh);
    shark.sFrameSpeed(10);

    //shark init
    shark.toggleMoving(true);
    
    prevPosSet = false;
    prevPos = glm::vec3(0.0f, 0.0f, 0.0f);
    prevPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    LoadSceneElements();

    debugPoints.clear();
    debugColors.clear();

	// DEBUG STUFFS!!!
	initGifWriting();
}

void CSharkNodeManager::LoadSceneElements()
{
    if (Node) {
        meshBuffer = new SMeshBuffer(shark.getSharkObject()->getIndBuf(), shark.getSharkObject()->getPosBuf(), shark.getSharkObject()->getNorBuf());
        Mesh = new CMesh(meshBuffer);
  
        Mesh->LoadDataIntoBuffers();
        TestMesh->LoadDataIntoBuffers();

        Node->SetMesh(Mesh);
		Node->SetMesh(TestMesh);

        for (SVertex &vert : Mesh->Buffers[0]->Vertices) {
            //vert.Position = shark.getSharkObject()->getPosBuf()[i];
            //vert.Normal = shark.getSharkObject()->getNorBuf()[i];
            vert.Color = color4f(1.0, 1.0, 0.0);
            //i++;
        }

        Node->SetScale(vec3d(SHARK_SCALE, SHARK_SCALE, SHARK_SCALE));

	}

    /*PositionBuffer = new ion::GL::VertexBuffer;
    PositionBuffer->Data<f32>(debugPoints.size() * sizeof(f32), nullptr, 3);
    ColorBuffer = new ion::GL::VertexBuffer;
    ColorBuffer->Data<f32>(debugColors.size() * sizeof(f32), nullptr, 3);
    //IndBuffer = new ion::GL::IndexBuffer;
    //IndBuffer->Data<u32>(Indices.size() * sizeof(u32), nullptr);
    //IndBuffer->Data<u32>(Indices);

    if (Node)
    {
        Node->SetVertexBuffer("vPosition", PositionBuffer);
        Node->SetVertexBuffer("vColor", ColorBuffer);
        //Node->SetVertexBuffer("vTime", TimeBuffer);
        Node->SetUniform("Model", &Node->GetTransformationUniform());
        //Node->SetUniform("timeMin", &timeUniformMin);
        //Node->SetUniform("timeMax", &timeUniformMax);
        //Node->SetPrimitiveType(ion::GL::EPrimitiveType::Lines);
        Node->SetPrimitiveType(ion::GL::EPrimitiveType::Points);
    }

    PositionBuffer->SubData(debugPoints);
    ColorBuffer->SubData(debugColors);
    //IndBuffer->SubData(Indices);
    //TimeBuffer->SubData(Times);

    if (Node)
    {
        Node->SetElementCount(debugPoints.size());
        Node->SetVisible(true);
    }*/
}

void CSharkNodeManager::Update(f32 const Elapsed)
{ 
    //printf("Elapsed: %f\n", (float)Elapsed);
    //update calls 
    glm::vec3 currPosition;
    if (world != nullptr) {// && toStep == true) {
        world->setSplinePath(currSite->GetTracks()[0]);
        currPosition = world->updateWorld((int)(Elapsed * 1000.0f));
        if (shark.isMoving()) {					//increment movement frame
            //if (showWorld)
            //{

            //printf("comparing %d with %d\n", world->gCurPoint(), currSite->GetTracks()[0]->gPointCount());
            if (world->gCurPoint() >= currSite->GetTracks()[0]->gPointCount() - 2) {
                // end of all things!
                printf("looping around to the start\n");
                world->reset();
                world->resetTime();
                shark.reset();

                Update(Elapsed);
                return;
            }
            glm::vec3 transVec = currPosition;// -prevPosition;

            printf("translating to (%f, %f, %f)\n", transVec.x, transVec.y, transVec.z);

            //Node->SetTranslation(vec3f(transVec.x, transVec.y, transVec.z));
            toStep = false;
        }
    //}
        TestMesh->Update();

    //if (world != nullptr) {
        //world->setSplinePath(currSite->GetTracks()[0]);

        if (shark.isMoving()) {					//increment movement frame
            //if (showWorld)
            //{
            //glm::vec3 currPosition = world->updateWorld((int)(Elapsed * 1000.0f));

            //printf("comparing %d with %d\n", world->gCurPoint(), currSite->GetTracks()[0]->gPointCount());
            /*if (world->gCurPoint() >= currSite->GetTracks()[0]->gPointCount() - 2) {
                // end of all things!
                printf("looping around to the start\n");
                world->reset();
                world->resetTime();
                shark.reset();
                
                Update(Elapsed);
            }*/

            shark.updateVelocity(world->gCurrentPoint(), world->gNextPoint(),  //TODO ???????? put Shark update in one call w/ dt
                world->gCurrentDTS());
            shark.prepareNextSeq(world->gAnimationLoop());
            //b1.update(dt/1000.0);
            // }
            //yaw rotation	

            // handle shark rotation
            //glm::vec3 ax = world->gRotationAxis();
            //Quat.CreateFromAxisAngle(ax.x, ax.y, ax.z, world1.gRotationDegrees());

            // estimate the tangent...
           // glm::vec3 nextPoint = world->gNext();
            if (!prevPosSet) {
                prevPos = currPosition;
                prevPosSet = true;
            }
            else {

                // debug stuff... 
                debugPoints.clear();
                debugColors.clear();
                debugPoints.push_back(currPosition.x);
                debugPoints.push_back(currPosition.y);
                debugPoints.push_back(currPosition.z);
                debugPoints.push_back(prevPos.x);
                debugPoints.push_back(prevPos.y);
                debugPoints.push_back(prevPos.z);
                debugColors.push_back(1.0f);
                debugColors.push_back(0.0f);
                debugColors.push_back(0.0f);
                debugColors.push_back(1.0f);
                debugColors.push_back(0.0f);
                debugColors.push_back(0.0f);

                debugColors.push_back(0.0f);
                debugColors.push_back(1.0f);
                debugColors.push_back(0.0f);
                debugColors.push_back(0.0f);
                debugColors.push_back(0.0f);
                debugColors.push_back(1.0f);

                float rotAngle = atan2(currPosition.z - prevPos.z, currPosition.x - prevPos.x);
                //rotAngle = rotAngle * (180.0f / PI);

                glm::vec3 diffAngle = currPosition - prevPos;

                //diffAngle = glm::normalize(diffAngle);
                //float rotAngle = ArcTan((float)(diffAngle.z / diffAngle.x)); // expecting this to be in radians
                if (diffAngle.x < 0) {
                    //rotAngle = PI + rotAngle;
                }
                //rotAngle = PI/2.0f;

                rotAngle = -1.0f * rotAngle * (180.0f / PI);

                //printf("looking from (%f, %f, %f) to (%f, %f, %f)\n", currPosition.x, currPosition.y, currPosition.z, nextPoint.x, nextPoint.y, nextPoint.z);
                printf("diffAngle: (%f, %f, %f)\n", diffAngle.x, diffAngle.y, diffAngle.z);
                printf("rotAngle: %f, rotationDegrees: %f\n", rotAngle, world->gRotationDegrees());

                //glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));

                glQuaternion Quat;
                Quat.CreateFromAxisAngle(0, 1, 0, rotAngle);// world->gRotationDegrees());
                GLfloat Matrix[16];
                Quat.CreateMatrix(Matrix);

                //shark.timedUpdate((int)(Elapsed * 1000.0f), world->deriveRailAngle(), world->gVelocity());  //TODO factor in dt in Shark update  
                shark.timedUpdate((int)(Elapsed * 1000.0f), rotAngle, world->gVelocity());  //TODO factor in dt in Shark update  


                glm::mat4 rotMat = glm::make_mat4(Matrix);
                glm::vec3 transVec = prevPos;// -prevPosition;
                Node->SetTranslation(vec3f(transVec.x, transVec.y, transVec.z));

                glm::mat4 transMat = glm::translate(glm::mat4(1.0f), transVec);

                glm::mat4 transformMatrix = transMat * rotMat;//rotMat*transMat;transMat * rotMat;

                Node->SetRotation(rotMat);

                glm::vec4 debugPos1 = glm::vec4(-0.05f, 0.0f, 0.0f, 1.0f);
                glm::vec4 debugPos2 = glm::vec4(0.05f, 0.0f, 0.0f, 1.0f);
                debugPos1 = transformMatrix * debugPos1;
                debugPos2 = transformMatrix * debugPos2;
                debugPoints.push_back(debugPos1.x);
                debugPoints.push_back(currPosition.y);
                debugPoints.push_back(debugPos1.z);
                debugPoints.push_back(debugPos2.x);
                debugPoints.push_back(currPosition.y);
                debugPoints.push_back(debugPos2.z);

                prevPos = currPosition;
            }

        }
        shark.drawShark(0);
    }

    vector<SVertex> tempVerts = shark.getSharkObject()->getVertBuf();

	// Proof of concept: modification of mesh buffers after initial load
	// For this example, turning all vertices yellow
    //Mesh->Buffers
   // for (int i = 0; i < )
    /*if (Node) {
        LoadSceneElements();
    }*/
    if (Node) {
        delete meshBuffer;
        delete Mesh;
        meshBuffer = new SMeshBuffer(shark.getSharkObject()->getIndBuf(), shark.getSharkObject()->getPosBuf(), shark.getSharkObject()->getNorBuf());
        Mesh = new CMesh(meshBuffer);
        Node->SetMesh(Mesh);

        int i = 0;
        for (SVertex &vert : Mesh->Buffers[0]->Vertices) {
            //vert.Position = shark.getSharkObject()->getPosBuf()[i];
            //vert.Normal = shark.getSharkObject()->getNorBuf()[i];
            vert.Color = color4f(1.0, 1.0, 0.0);
            i++;
        }
    }
	Mesh->Update();
}

CSceneNode * CSharkNodeManager::GetNode()
{
	return Node;
}

CSceneNode const * CSharkNodeManager::GetNode() const
{
	return Node;
}

void CSharkNodeManager::setCurrentSite(CSite * site) { // TODO : implement support for multiple sharks
    if (currSite == nullptr && site != nullptr) {
        currSite = site;
        //vector<CSplinePath*> paths = site->GetTracks();
        //path = paths[0];
       // splines = currSite->GetTracks();
        //splines[0]->drawPoints(0, false);
        //needsUpdate = true;
        world = new SharkWorld(currSite->GetTracks()[0]);
    }
    else if (currSite != nullptr && site != nullptr) {
        if (site->GetName() != currSite->GetName()) {
            currSite = site;
            world = new SharkWorld(currSite->GetTracks()[0]);
            //splines = currSite->GetTracks();
            //splines[0]->drawPoints(0, false);
            //needsUpdate = true;
        }
    }
}

void CSharkNodeManager::toggleAnimation(bool moving) {
    isMoving = moving;
    shark.toggleMoving(isMoving);
}

void CSharkNodeManager::initGifWriting() {
	writeGif = true;

}


void CSharkNodeManager::writeFrameToGif() {

}