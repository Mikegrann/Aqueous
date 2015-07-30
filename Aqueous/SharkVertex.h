#ifndef _3D_SHARK_VERTEX_STRUCTURE_TRACKER
#define _3D_SHARK_VERTEX_STRUCTURE_TRACKER

#include <ionEngine.h>

class SharkVertex
{
	struct SharkVertexBone {
		SharkVertexBone(string n, float w) : name(n), weight(w)
		{}

		string name;
		float weight;
	};

	public:
		glm::vec3 gLocal() { return local; }
		glm::vec3 gTransformed() { return transformed; }
		glm::vec3 gNormal() { return normal; }

		void sNormal(glm::vec3 n) { normal = n; }
		void sTransformed(glm::vec3 n) { transformed = n; }
		void sLocal(glm::vec3 n) { local = n; }

		int gNumInfluences() { return bones.size(); }
		string gBoneName(int index) { return bones[index].name; }
		float gBoneWeight(int index) { return bones[index].weight; }

		void sBoneWeight(int index, float newWeight) { bones[index].weight = newWeight; }
		void sBonePair(pair<string,float> f) { bones.emplace_back(f.first, f.second); }
		void sBonePair(string boneName, float boneWeight) { bones.emplace_back(boneName, boneWeight); }

		float checkBone(string boneName);

	//private:
		glm::vec3 local;    
		glm::vec3 transformed;  //vertex after transformation
		glm::vec3 normal;

	private:
		vector<SharkVertexBone> bones;
};

#endif

