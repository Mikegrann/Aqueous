#pragma once

#include "EXEreader.h"
#include "ParamFunctions.h"
#include "ParamStructures.h"
#include "CSplineTable.h"
#include "GPSconverter.h"
#include "CSplineFunctions.h"
#include "CProgramContext.h"
#include "CDataSet.h"
#include "CLocation.h"


/*Holds data for a catmull-rom path. This class can take in points read in from a file, and also reparamaterize teh curve by arc-length   */
class CSplinePath
{
public:
    //initialization functions
    CSplinePath();
    CSplinePath(bool ghostPoints);
														    //set true to make the first and 
														    //last points invisible
    ~CSplinePath(){}
    void initSpline();    //reads .txt or .mat or .csv data sheets 
    //void initSplineMAT(string matfilename);     //opens mat files
    void initSplineEXE();     //opens csv files
    //void initSplineZOE(string zoefilename);     //opens txt files

    void setFileName(string _filename) { filename = _filename; };
    void setDataSet(CDataSet* ds) { DataSet = ds; }
    void setLocation(CLocation* l) { Location = l; }
    void setContext(CProgramContext* c) { Context = c; }
    void gatherDTPoints();
    void gatherEXPoints();
    void gatherZOEPoints();
    void parameterizeSpline();
	void normalizeCoords();
    void transformCoords();
    void initTangents();
    void calcRadius();
    void deleteHeap();
    int size(){return points.size();}
    void drawPoints(int curPoint, bool usingGhostPointsAsEndpoints);
    bool pointInFrustum(glm::vec3 v);

    void setLineShader(GLuint _lineShader) { lineShader = _lineShader; }
 

    //interpolation functions
    f64 StoU(f64, int);
    glm::vec3 splineLocation(f32 u, int startpoint);	
    glm::vec3 getNearbyPoint(f64 distanceAhead, int startPoint, f64 startU);
    f64 convertTimestampToU(f32 u, int currentLocation);
		
    //displayFunctions
    //void drawPoints(f32** frustum);

    //points
    int gPointCount(){ return points.size(); }
    glm::vec3 gPoint(int index){return points[index];}
    //void parseDataFileMAT(const char* filename){mreader.parseFile(filename);}	
    void parseDataFileEXE(const char* filename){ereader.parseFile(filename);}	
    //void parseDataFileZOE(const char* filename){zoereader.parseFile(filename);}	
    f64 gDTS(int index){return dts[index];} //time delta distance between two points
    f64 gTotalDTS(int index){return totts[index];} //time delta distance between two points
    bool sGhostPoints(bool b){isGhostPointMode = b;}
    bool sLimitedDrawing(bool drawingStatus){ limitedDrawing = drawingStatus;}

    glm::vec3 gNextPoint() { return nextPoint; }

    //VBO stuff
    vector<f32> getPositionBuffer() { return posBuf; };
    vector<f32> getColorBuffer() { return colorBuf; };
    vector<u32> getIndexBuffer() { return indBuf; };

	// Extents
	SRange<f32> GetXRange() { return XRange; }
	SRange<f32> GetYRange() { return YRange; }
	SRange<f32> GetZRange() { return ZRange; }

	SLongitudeLatituded OffsetPos;

private:	
    CDataSet* DataSet;
    CLocation* Location;
    CProgramContext* Context;

    glm::vec3 nextPoint;
    bool ranOnce;

    GLuint lineShader;
    //data
    vector<CSplineTable*> paramTable;

    string filename;

    //original data read
    vector<glm::vec3> points;   //vector stores x, y, z(where z is water depth, unlike rendering)
    vector<glm::vec3> tangents; //tangents for the curve as it hits each point
    vector<glm::vec3> tTangents; //tangents for the time curve as it hits each point
    vector<f64> dts; //difference in time (seconds) between one point and the next
    vector<f64> totts; //total running time at each point 
    glm::vec3 maxPt; //records the maximum point the shark traveled
    glm::vec3 minPt; //minimum point
    glm::vec3 midPt;

	// Extents
	SRange<f32> XRange;
	SRange<f32> YRange;
	SRange<f32> ZRange;

    // VBO stuff
    vector<f32> posBuf;
    vector<f32> colorBuf;
    vector<u32> indBuf;

	f64 SplineDepth;

    u32 currIndex;
    glm::vec3 prev_p;
    glm::vec3 currColor;

    bool saveTime;
    float savedTime, offsetTime;

    //running totals of timestamp for catmull interpolation
    //f64 timePast;
    //f64 timeCurrent;
    //f64 timeNext;
    f64 curTimeSpline;

    bool limitedDrawing; 	//sets the spline to consider at max 600 points when going to draw. 

    EXEreader ereader;
    f64 radius;
    ParamFunctions paramfuncs;

    void isLargerPoint(glm::vec3 cur);
    void isSmallerPoint(glm::vec3 cur);
    f64 f64Lerp(f64 input, f64 minx, f64 maxx, f64 miny, f64 maxy);
    glm::vec3 hermiteMatrix(f32 u, glm::vec3 startLocation, glm::vec3 endLocation, glm::vec3 startTangent, glm::vec3 endTangent);
    glm::vec3 catmullMatrix(f32 u, int currentLocation);
		
    f64 HmInt(f32 U[4], const f32 M[16], f32 B[4]);
    bool tabSet;
		
    void drawPointLine(int i);

    bool isGhostPointMode;
    f32 static const Mcat[16];// = {-.5, 1.5, -1.5, .5, 1, -2.5, 2, -.5, -.5, 0, .5, 0, 0, 1, 0, 0};
    f32 static const Mher[16];// = {2, -2, 1, 1, -3, 3, -2, -1, 0, 0, 1, 0, 1, 0, 0, 0};
    f64 Tolerance;
    int static const totalSlices = 100; //determines how finely to sample the curve

    bool static const isCatmullMode = false; //TODO handle this better...
    /*#ifdef VAR_CATMULL	
    bool static const isCatmullMode = true;
		    #else
		    bool static const isCatmullMode = false;
		    #endif*/
		
    void printDTs() {
        for (int i=0; i<dts.size(); ++i) {
            printf("%lf\n",dts[i]);
        }
    };

    void printPs() {
        for (int i=0; i<points.size(); ++i) {
            printf("(%f, %f, %f)\n", points[i].x, points[i].y, points[i].z);
        }
    };

    void printdTPs() {
        for (int i=0; i<dts.size()-1; ++i) {
            f32 dist = glm::distance(glm::vec3((f32)points[i].x,(f32)points[i].y,(f32)points[i].z),glm::vec3((f32)points[i+1].x, (f32)points[i+1].y, (f32)points[i+1].z));
            printf("dist: %f, time: %lf\n", dist, dts[i+1]);
        }	
    };

};

