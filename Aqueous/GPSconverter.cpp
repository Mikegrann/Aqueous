#include "GPSconverter.h"

/*assuming that latitude and longitude are stored in the x and z coordinates of a point */
f64 GPSconverter::GPSdistance(glm::vec3 p, glm::vec3 q)
{
	f64 h = haversine(q.x - p.x) + cos(p.x)*cos(q.x)*haversine(q.z - p.z); 
	return inverseHaversine(h);
}

f64 GPSconverter::haversine(f64 theta)
{
	return (1.0 - cos(theta))/2.0;
}

f64 GPSconverter::inverseHaversine(f64 h)
{
	return 2.0*EARTH_RADIUS_M * asin(sqrt(h));
}

f64 GPSconverter::findScaleDifference(glm::vec3 p, glm::vec3 q)
{
	f64 cirDist = GPSdistance(p,q);
	//f64 coorDist = p.EuclDistance(q);
    f64 coorDist = glm::distance(p, q);
	return cirDist / coorDist;
}