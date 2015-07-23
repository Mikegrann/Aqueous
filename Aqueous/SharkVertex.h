#ifndef _3D_SHARK_VERTEX_STRUCTURE_TRACKER
#define _3D_SHARK_VERTEX_STRUCTURE_TRACKER

#include <ionEngine.h>

class SharkVertex
{
	public:
		SharkVertex(){bones = vector<pair<string,float> >();}
		~SharkVertex(){}
		glm::vec3 gLocal(){return local; }
		glm::vec3 gTransformed(){return transformed;}
		glm::vec3 gNormal(){return normal;}
		void sNormal(glm::vec3 n){normal = n;}
		void sTransformed(glm::vec3 n){transformed = n;}
		void sLocal(glm::vec3 n){local = n;}
		int gNumInfluences(){return bones.size();}
		string gBoneName(int index){return bones[index].first;}
		float gBoneWeight(int index){return bones[index].second;}
		void sBoneWeight(int index, float newWeight){bones[index].second = newWeight ;}
		void sBonePair(pair<string,float> f){bones.push_back(f);}
		void sBonePair(string boneName, float boneWeight){sBonePair(pair<string, float>(boneName, boneWeight));}
		float checkBone(string boneName);

	//private:
		glm::vec3 local;    
		glm::vec3 transformed;  //vertex after transformation
		glm::vec3 normal;
		vector<pair<string,float> > bones;
};

#endif

