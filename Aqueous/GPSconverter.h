#pragma once

#include <ionEngine.h>

#define EARTH_RADIUS_KM 6371.0
#define EARTH_RADIUS_M 6371000.0

/*measures the distance in meters between two GPS coordinates */
/*For the shark application, the scaling difference is the ratio between the GPS coordinate and 
 *their distance.
* Only meant to be used for small distances. Assumes the earth is spherical. */
class GPSconverter
{
	public:
		GPSconverter(){}
		~GPSconverter(){}
		f64 GPSdistance(glm::vec3 p1, glm::vec3 p2);  //great-circle distance between two points
		f64 haversine(f64 theta);
		f64 inverseHaversine(f64 h);
		f64 findScaleDifference(glm::vec3 p, glm::vec3 q);   //return the ratio of greatCircle distance to coordinate distance 
		
};