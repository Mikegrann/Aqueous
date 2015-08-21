#include "EXEreader.h"
#include <algorithm>

EXEreader::EXEreader() {
    dt = prevTime = minlat = minlong = maxlat = maxlong = 0.0;

    minlat = DBL_MAX;
    minlong = DBL_MAX;
    maxlat = -DBL_MAX;
    maxlong = -DBL_MAX;
}

void EXEreader::parseFile(const char* filename)
{
    ifstream trackFile;
    trackFile.open(filename);
    string line;
   
    if (!trackFile.is_open()) {
        printf("csv file not found or could not be opened\n");
        exit(-1);
    }
    int i = 0;
    while (getline(trackFile, line)) {
        f64 timestamp = 0.0;
        f64 auvX = 0.0;
        f64 auvY = 0.0;
        f64 auvZ = 0.0;
        f64 auvHeadingX = 0.0;
        f64 auvHeadingZ = 0.0;
        f64 auvHeadingY = 0.0;
        f64 angle = 0.0;
        f64 distance = 0.0;
        f64 sharkDepth = 0.0;
        f64 estX = 0.0;	// these values estimated by auv
        f64 estY = 0.0;
        f64 estZ = 0.0;
        f64 sharkX = 0.0;	// true x
        f64 sharkY = 0.0; // true y
        f64 sharkHeading = 0.0; // TODO make sure if this is angle (radians) from north
        f64 sharkSpeed = 0.0;

        // csv format: Time,AUV x,AUV y,AUV z,AUV heading x,AUV heading y,AUV heading z,Angle,Distance,Depth,Estimate x,Estimate y,Estimate z,True x,True y,Heading,Speed,Acceleration,Mag_uy,Mag_uz,Mag_cy,Mag_cz
        sscanf(line.c_str(), "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &timestamp, 
      	    &auvX, &auvY, &auvZ, &auvHeadingX, &auvHeadingY, &auvHeadingZ, &angle, &distance, &sharkDepth, &estX, &estY, 
      	    &estZ, &sharkX, &sharkY, &sharkHeading, &sharkSpeed);

        //printf("timestamp: %lf, latitude: %f, longitude: %f\n", timestamp, latitude, longitude);

        if (prevTime > 0.0) {
            dt = timestamp - prevTime;
        }
        else {
            dt = 0.0;
        }

		if(!(estX >= -0.001 && estX <= 0.001 && estY >= -0.001 && estY <= 0.001)) //some data entries are blank
	    {
            latLong.push_back(glm::vec3(estX, 0.0, estY));
            dts.push_back(dt);
            prevTime = timestamp; 
            if (estY < minlong) {
	            minlong = estY;
            }
            if (estY > maxlong) {
                maxlong = estY;
            }
            if (estX < minlat) {
                minlat = estX;
            }
            if (estX > maxlat) {
                maxlat = estX;
            }
        }
        else {
            prevTime = 0.0;
        }
      
        i++;
    }
    trackFile.close();
}
