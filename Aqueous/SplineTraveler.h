/*CSC 474 lab 2 */
#ifndef SPLINE_TRAVELLER_3D_SPLINE
#define SPLINE_TRAVELLER_3D_SPLINE

#include "CProgramContext.h"
#include "CSplinePath.h"
#include "glQuaternion.h"

/* A Spline progress management class. Assumes one entity has a spline. */
class SplineTraveler
{
	public:
		SplineTraveler();
		SplineTraveler(CSplinePath* _path); 
		~SplineTraveler(){}
		glm::vec3 update(float dt);
		void drawAndMoveCamera();
		void drawStatic();
		void ExtractFrustum();
		//bool pointInFrustum(glm::vec3 v);
		glm::vec3 upCurrentLocation(int dt);
		//void setFrustum(Frustum *frust){frustum = frust;}   
		int deriveRailAngle(float lead, float frontBy, float behindBy);
		void sGhostPoints(bool b){areGhostPoints = b; resetTime();}	
		double calcRotationAngle();

        void setSplinePath(CSplinePath * _path) { path = _path; }

		void deleteHeap(){path->deleteHeap();} //delete's SplinePath heap
		glm::vec3 gPathPoint(int index){return path->gPoint(index);}
		glm::vec3 gCurrentPoint(){return path->gPoint(curPoint);}
		glm::vec3 gNextPoint(){if(curPoint < path->size()){
			return path->gPoint(curPoint+1); }
			else {
				return path->gPoint(0);}
		}
		glm::vec3 gPrevPoint(){if(curPoint == 0){
			return path->gPoint(path->size()-1); }
			else {
				return path->gPoint(curPoint-1);}
		}
        int gCurPoint(){ return curPoint; }
		double gRotationDegrees(){return rotationAngle * 180/3.14159265 ;}
		double gRotationRadians(){return rotationAngle;}
		glm::vec3 gRotationAxis(){return rotateAxis;}
		glm::vec3 gLocation(){return location;}
		glm::vec3 gNearbyPoint(float ahead, int curPoint, float u);
        glm::vec3 gNext() { return path->gNextPoint(); }
		void speedUp(){elapseRate += .5;}
		void slowDown(){if(elapseRate >= 0.5){ elapseRate -= .5;} }
		void resetTime();

		double gDTS(int index){return path->gDTS(index);} //time difference between points               
		double gCurrentDTS(){return path->gDTS(curPoint);}
		glm::vec3 gVelocity(){return velocity;}

		void setPathLineShader(GLuint lineShader) { path->setLineShader(lineShader); }


	private:

		//void drawPoints();
		//void drawPointLine(int i);
		double calcRotation(glm::vec3 pFrom, glm::vec3 pVertex, glm::vec3 pDest);
		glm::vec3 interpolateRotation();
		glm::vec3 velocity; //velocity of traveler down the spline. Measured in distance units per second.	

        glm::vec3 next;

		CSplinePath* path;
		//Frustum *frustum;

		//global animation controls
		glm::vec3 rotation; 	//current rotation
		double rotationAngle;
		glm::vec3 rotateAxis;    //axis of rotation
		glm::vec3 location; 	//current location
		glm::vec3 desiredRotation;
		glm::vec3 futureRotation;
		glm::vec3 deltaTheta; 	//difference between desired and future rotation
		bool areGhostPoints;       //showing first and last points
		
		int curPoint;           //the most recent point the traveler has passed
		float curU;        	//how far (between zero and one) the traveler is to the next point
		float timer;       	//Total time since started. Interpolate time into utime. Double check conversion from time into catmull paramaters
		float timeSinceKnot;      //the time (miliseconds) since the last knot
		float elapseRate;       //user controllable rate of time passage. Fast forward, essentially. Cannot be negative. 
		int nextPoint;

};

#endif
