#ifndef InvDistWeight_H
#define InvDistWeight_H

#include <ionMath/ionMath.h>
#include <map>

class InvDistWeight
{
public:
	static float interpolate(std::map<vec3f, float> const &known, vec3f const &position, int power);
	static float weight(vec3f const &x, vec3f const &x_i, int power);

};

#endif