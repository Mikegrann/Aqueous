#ifndef SHARK_WORLD_3D_SIMULATION
#define SHARK_WORLD_3D_SIMULATION

#include "CProgramContext.h"

#include "EXEreader.h"
#include "CSplinePath.h"
#include "SplineTraveler.h"

//represents a SINGLE world a shark inhabits
class SharkWorld
{
	public:
		SharkWorld(){}
		SharkWorld(CSplinePath* _path){ traveler = SplineTraveler(_path);  
                        path = _path;
					    updateAnimationFlag = true; 
		                lookAhead = 0.0f;
		                frontby = 0.6f;
		                behindby = 0.3f;}
		~SharkWorld(){}
		glm::vec3 updateWorld(int dt);
		void displayWorld();
		//string getSharkTurn();
		//void initialize(string splineFilename);
		int deriveRailAngle();

        void reset();

		void setPathLineShader(GLuint lineShader) { traveler.setPathLineShader(lineShader); }
        void setSplinePath(CSplinePath * path) { traveler.setSplinePath(path); }

		//lowerclass management
		void deleteHeap(){traveler.deleteHeap();} //delete's SplinePath heap
        int gCurPoint(){ return traveler.gCurPoint(); }
		glm::vec3 gPathPoint(int index){return traveler.gPathPoint(index);}
		glm::vec3 gCurrentPoint(){return traveler.gCurrentPoint();} 
		glm::vec3 gNextPoint(){ return traveler.gNextPoint();} 
		glm::vec3 gPrevPoint(){ return traveler.gPrevPoint();}
		//glm::vec3 gRotationDegrees(){return traveler.gRotationDegrees();}
		double gRotationDegrees(){return traveler.gRotationDegrees();}
		//glm::vec3 gRotationRadians(){return traveler.gRotationRadians();}
		double gRotationRadians(){return traveler.gRotationRadians();}
		glm::vec3 gRotationAxis(){return traveler.gRotationAxis();}
		
		double gDTS(int index){return traveler.gDTS(index);} //time difference between points		
		double gCurrentDTS(){return traveler.gCurrentDTS();}
		
		string gAnimationLoop(){return animationLoop;}

		glm::vec3 gVelocity(){return traveler.gVelocity();} //velocity the traveler is moving
        
        glm::vec3 gNext() { return traveler.gNext(); }

		void speedUp(){traveler.speedUp();}
		void slowDown(){traveler.slowDown();}
		void resetTime(){traveler.resetTime();}

	private:
        CSplinePath* path;
		//world data info
		SplineTraveler traveler;
		
		//global animation controls
		bool updateAnimationFlag;
		glm::vec3 deltaTheta; //difference between desired and future rotation
		string animationLoop;
		
		float skyboxrotation;

        //for calculating shark's lookahead and angle calculating range
        float lookAhead;
        float frontby;
        float behindby;

		void drawSkybox();
		void drawPrettyStuff();

};

#endif
