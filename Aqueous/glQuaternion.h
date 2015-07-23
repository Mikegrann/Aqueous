// glQuaternion.h: interface for the glQuaternion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLQUATERNION_H__52FB84BB_6D1B_4314_833F_5A8737A35DAB__INCLUDED_)
#define AFX_GLQUATERNION_H__52FB84BB_6D1B_4314_833F_5A8737A35DAB__INCLUDED_

#include <ionEngine.h>

#define PI			3.14159265358979323846

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class glQuaternion  
{
	public:
		glQuaternion operator *(glQuaternion q);
		
		glQuaternion();
		virtual ~glQuaternion();
		void CreateMatrix(float *pMatrix);
		void CreateFromAxisAngle(float x, float y, float z, float degrees);
		void Normalize();
		float Magnitude();
		glm::vec3 gVector();
		void sVector(glm::vec3 vec);
		glQuaternion multiply(glQuaternion other);

		float gW(){return m_w;}
		void sW(float w){m_w = w;}
		float gX(){return m_x;}
	    void sX(float x){m_x = x;}
		float gY(){return m_y;}
		void sY(float y){m_y = y;}
		float gZ(){return m_z;}
		void sZ(float z){m_z = z;}

	private:
		float m_w;
		float m_z;
		float m_y;
		float m_x;
};

#endif // !defined(AFX_GLQUATERNION_H__52FB84BB_6D1B_4314_833F_5A8737A35DAB__INCLUDED_)
