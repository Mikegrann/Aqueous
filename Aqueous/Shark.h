#ifndef SHARK_MAIN_SYSTEM
#define SHARK_MAIN_SYSTEM

#include "SharkQuadObject.h"
#include "glQuaternion.h"
#include "Keyframe.h"
#include "FrameSequence.h"
#include "KeyframeSystem.h"
#include "SharkLoco.h"

typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
} materialStruct;


/*Uniting class of all shark subcomponents. This is where the simulation (SharkSkeleton) and playback (KeyframeSystem) both interface.
 * It keeps track of all the controls that are set. */
class Shark
{
	public:
		Shark();
		//void drawSharkSeg(float rotate, float start, float end, int index, float length);
		void drawSkin(int frame);
		//void drawSegment(int index, float rot, GLUquadricObj *quadratic);
		//void drawSpine(int frame, GLUquadricObj *quadratic);
		void drawShark(int frame);
		void timedUpdate(int dt, int railAngle, glm::vec3 velociity);  
		void materials(materialStruct materials);
		//void segUpdate();
		KeyframeSystem genKeyframes(bool dynamicMode, SharkMesh *sharkmesh);
		void deinitialize(){kfSys.deleteFrameHeaps();}
		void updateVelocity(glm::vec3 start, glm::vec3 end, double dt);

		float scaleRatio(){return skeleton.scaleRatio();}
	
		int numSegments();

		//moved from main
		void readMovementData(const char* file, bool isDynamicMode);
		void defSequence();
		void setShader(GLuint _shaderProg) {shaderProg = _shaderProg;};


		GLfloat gSegmentRotation(int seq, int seg1, int seg2){return segmentRot[seq][seg1][seg2];}
		int gParsedSoFar(){return sequencesParsed;}		
		void incrementSequences(){sequencesParsed++;}
		void sSegmentLength(int index, int put){segLengthInput[index] = put;} 
		//void sSkeleton(SharkMesh* sk){skeleton = sk;}
		void sMesh(SharkMesh* sk){skeleton.setMesh(sk);}
		//void buildSkeleton(Mesh *mesh, int numSegs){skeleton.buildSkeleton(mesh, numSegs, (float*) segLength);}
		void buildSkeleton(string modelFilename){skeleton.buildSkeleton(modelFilename);}
		void sFrameSpeed(int frameSpeed){kfSys.setFrameSpeed(frameSpeed);}	
		void toggleSpine(bool isSpineShown){showSpine = isSpineShown;}
		void toggleSkin(bool isSkinShown){showSkin = isSkinShown;}
		void toggleMoving(bool mov){ismoving = mov;}
		void togglePlay(bool p){play = p;}
		bool isSpine(){return showSpine;}
		bool isSkin(){return showSkin;}
		bool isMoving(){return ismoving;}
		bool isPlay(){return play;}

		//static mode animation stuff
		void decreaseFrames(){kfSys.decreaseFrames();}
		void increaseFrames(){kfSys.increaseFrames();}
		void sLoopSequence(int index){kfSys.sNextSequence(index);}	
		int gLoopSequence(){return kfSys.gNextSequence();}	
		void nextLoopSequence(){kfSys.incrementNextSequence();}
		void prepareNextSeq(string seqName){kfSys.prepareNextSeq(seqName);}

        SharkQuadObject * getSharkObject() { return sharkObject; };

		//used to draw spine	
		#ifdef VAR_NOSEG
		static const int segments = 10;	//hardcode number of segments for how many segments we have data for
		#else
		static const int segments = 30;
		#endif
		static const int sequences = 10;                  //number of sequences  (animation loops) the shark has
		/* GLUI Globals */
		static const int segMin = 6;	//minimum amount of segments allowed
		static const int segMax = 30;	//maximum amount of segments allowed


	private:
		GLuint shaderProg;
		float totalLength;		//total length of shark (length of skin)
		bool showSpine;			//toggle to draw the spine
		bool showSkin;			//toggle to draw the skin
		bool ismoving;				//toggle shark movement
		bool play;				//toggle shark movement frame by frame
		glm::vec3 velocity;
		glm::vec3 lastpt;	//to help calculate velocity

		//used to draw spine
		double	segPercent;			//percent of the totalLength 1 segment is (used in segUpdate)
		double	lengthLeft;			//amount of shark not with specified length (used in segUpdate)
		int sequencesParsed;		//notes how many sequences has been parsed so far

        SharkQuadObject * sharkObject;
				
		//used in GLUI
		int tempSegments;		//used in segUpdate and GLUI interface
		int segNum;				//counter in segUpdate and GLUI
		int tempSegPercent;		//used in segUpdate

		//segment information. Passed onto the Skeleton system
		GLfloat	segmentRot[sequences][segments][segments];		//contains movement data which we get from a file
		GLfloat	segLengthInput[segMax];				//used to tell if user has specified segment lengths
		GLfloat	segLength[segMax];					//used to draw spine
		KeyframeSystem kfSys;
		//SharkSkeleton skeleton;
		SharkLoco skeleton;

		//Mesh	mesh;
		glQuaternion glQuat;
};


#endif
