#include "CSplinePath.h"



const f32 CSplinePath::Mcat[] = {-.5, 1.5, -1.5, .5, 1, -2.5, 2, -.5, -.5, 0, .5, 0, 0, 1, 0, 0};
const f32 CSplinePath::Mher[] = {2, -2, 1, 1, -3, 3, -2, -1, 0, 0, 1, 0, 1, 0, 0, 0};

CSplinePath::CSplinePath() {
    lineShader = 0; 
    filename = ""; 
    Tolerance = .2; 
    tabSet = false; 
    isGhostPointMode = false; 
    limitedDrawing = false; 

    points.clear();

    posBuf.clear();
    colorBuf.clear();
    indBuf.clear();
}

CSplinePath::CSplinePath (bool ghostPoints) { 
    lineShader = 0; 
    filename = ""; 
    Tolerance = .2; 
    tabSet = false; 
    isGhostPointMode = ghostPoints; 
    limitedDrawing = false;

    posBuf.clear();
    colorBuf.clear();
    indBuf.clear();
}

f64 CSplinePath::f64Lerp(f64 input, f64 minx, f64 maxx, f64 miny, f64 maxy)
{
	return miny + (input-minx) * ((maxy-miny)/(maxx-minx));
}

void CSplinePath::deleteHeap()
{
	for(int i = 0; i < paramTable.size(); i++)
	{
		delete paramTable[i];
	}
}

void CSplinePath::initSpline(){    //reads .txt, .mat or .csv data sheets
	cout << "in init Spline" << endl;
	string end = filename.substr(filename.find_last_of('.') + 1);
	if (end == "txt") {
		//initSplineZOE(filename);
	}
	else if (end == "mat") {
		//initSplineMAT(filename);
	}
	else if (end == "csv") {
		printf("going into initSplineEXE\n");
		initSplineEXE();
		printf("success\n");
	}
	else {
		cout << "Spline file does not correspond to a supported file type: " << end << " not recognized" << endl;
	}
}


/*These functions will initialize the Spline Path given a filename to a file 
void CSplinePath::initSplineMAT(string filename)
{
	parseDataFileMAT(filename.c_str());
	gatherDTPoints();
	parameterizeSpline();
}*/

void CSplinePath::initSplineEXE()
{
   printf("parsing\n");
	parseDataFileEXE(filename.c_str());
	printf("gathering\n");
	gatherEXPoints();
	//printf("parameterizing\n");
	parameterizeSpline();
	//printf("success\n");
}

/*void CSplinePath::initSplineZOE(string filename)
{
	zoereader = ZOEreader();
	parseDataFileZOE(filename.c_str());
	gatherZOEPoints();
	parameterizeSpline();
}*/

/*void CSplinePath::gatherDTPoints()
{
	Vector3f p, q;
	f64 dt = 0;
	f32 runningTotts = 0; //running time totals per spot
	f32 prevRT = 0;
	//printf("data width %d\n", mreader.width());
	for(int id = 0; id < mreader.length(); id++)
	{
	   //f64 timeVal = mreader.gElement(0,id);
	   //printf("timeVal: %f\n", (f32) timeVal);

		dt += mreader.gElement(0,id);
		runningTotts += dt;
		p.x = mreader.gElement(1,id);
		p.y = .2 * sin(id/8.0);   	//varies the height a little over each point
		p.z = mreader.gElement(2,id);

		if(!(p.x == q.x && p.z == q.z))
		{
			//p.Print();
			isLargerPoint(p);
			isSmallerPoint(p);
			points.push_back(p);
			dts.push_back(dt);
			totts.push_back(prevRT);
			prevRT = runningTotts;
			dt = 0;

		}
		q = p;
	}
	calcRadius();
    
	initTangents();
	
	//printPs();
	printdTPs();
}*/

//csv
void CSplinePath::gatherEXPoints()
{
   glm::vec3 p, q;
	f64 dt = 0;
	f32 runningTotts = 0; //running time totals per spot
	
	// get scale
	glm::vec3 sc1 = ereader.gCoordinate(0);
	glm::vec3 sc2 = ereader.gCoordinate(ereader.size()-1);
	int i = 1;
    while (CSplineFunctions::fEquals(sc1, sc2, 0.001f)){  //program will NaN out if these points are the same
		sc2 = ereader.gCoordinate(i);
		i++;	
	}
	f64 scale = GPSconverter().findScaleDifference(sc1, sc2);
	
	for(int id = 0; id < ereader.size(); id++)
	{
		glm::vec3 p = ereader.gCoordinate(id);
		//p = p * scale;
		p = p * 6.0f;
        f64 deltaTime = ereader.gDTS(id) *6.0;

		if(!(p.x == q.x && p.y == q.y && p.z == q.z))
		{
			isLargerPoint(p);
			isSmallerPoint(p);
			points.push_back(p);
			dts.push_back(deltaTime);
			totts.push_back(runningTotts+deltaTime);
			runningTotts += deltaTime;

		}
		q = p;
	}
	normalizeCoords();

	calcRadius();

	initTangents();
	
	//printPs();
	//printdTPs();
}

/*void CSplinePath::gatherZOEPoints()
{
	sGhostPoints(true);
	f32 runningTotts = 0;  //running time total
	for(int id = 0; id < zoereader.size(); id++)
	{
		Vector3f p = zoereader.gCoordinate(id);
		//p.x = f64Lerp(ereader.gCoordinate(id).x, ereader.gMinLatitude(), ereader.gMaxLatitude(), 0, 100);
		//p.y = f64Lerp(ereader.gCoordinate(id).y, ereader.gMinLongitude(), ereader.gMinLongitude(), 0 , 100) ;
		//p.z = ereader.gCoordinate(id).z;

		isLargerPoint(p);
		isSmallerPoint(p);
		points.push_back(p);
		dts.push_back(zoereader.gDTS(id));
		totts.push_back(runningTotts+zoereader.gDTS(id));
		runningTotts += zoereader.gDTS(id);
	}
	calcRadius();


	initTangents();
}*/

/*notes upper corner of world while parsing */
void CSplinePath::isLargerPoint(glm::vec3 cur)
{
	if(cur.x > maxPt.x)
	{
		maxPt.x = cur.x;
	}
	if(cur.y > maxPt.y)
	{
		maxPt.y = cur.y;
	}
	if(cur.z > maxPt.z)
	{
		maxPt.z = cur.z;
	}
}

/*Notes lower corner of world while parsin */
void CSplinePath::isSmallerPoint(glm::vec3 cur)
{
	if(cur.x < minPt.x)
	{
		minPt.x = cur.x;
	}
	if(cur.y < minPt.y)
	{
		minPt.y = cur.y;
	}
	if(cur.z < minPt.z)
	{
		minPt.z = cur.z;
	}
}

/*Used to calculate the size of the whole world */
void CSplinePath::calcRadius()
{
	midPt = maxPt - minPt;
    radius = glm::length(maxPt - midPt);
}

void CSplinePath::normalizeCoords() {
	glm::vec3 min(std::numeric_limits<float>::max());
	glm::vec3 max(std::numeric_limits<float>::min());
	for (int i = 0; i < points.size(); i++)
	{
		glm::vec3 p = points[i];
		if (p.x < min.x) {
			min.x = p.x;
		}
		if (p.y < min.y) {
			min.y = p.y;
		}
		if (p.z < min.z) {
			min.z = p.z;
		}

		if (p.x > max.x) {
			max.x = p.x;
		}
		if (p.y > max.y) {
			max.y = p.y;
		}
		if (p.z > max.z) {
			max.z = p.z;
		}
	}

	XRange.Minimum = min.x;
	XRange.Maximum = max.x;
	YRange.Minimum = min.y;
	YRange.Maximum = max.y;
	ZRange.Minimum = min.z;
	ZRange.Maximum = max.z;

	for (int i = 0; i < points.size(); i++)
	{
		points[i] = ((points[i] - min) / (max - min)) - glm::vec3(0.5);
	}
}

void CSplinePath::initTangents()
{
	tTangents = vector<glm::vec3>();
	tangents = vector<glm::vec3>();

	if(!isCatmullMode)
	{
		//first value needs to be initilized 
        glm::vec3 tan = glm::vec3(points[1] - (points[2] - points[1]) - points[0]);
        tan *= 0.9f;

		tangents.push_back(tan);

		//loop over each of the middle points
		for(int i = 1; i < points.size()-1; i++)
		{
			//first create the direction of the tangent. then scale it to the needed magnitude
			glm::vec3 postPt = points[i-1];  //posterior
			glm::vec3 antPt = points[i+1];   //anterior
			glm::vec3 tarPt = points[i];     //center target
			glm::vec3 before = tarPt - postPt;
			before.y = 0;
			glm::vec3 after = tarPt - antPt;
			after.y = 0;
            before /= glm::length(before);
            after /= glm::length(after);
			glm::vec3 edgeDiff;	

            if (glm::dot(before, after) >= 0.8f)
			{
				//case: need perpendicular tangent
				//180 degree turn case is discontinious. This is a fix
				//correct 180 case before cross multiplying
				int j = 3;
				while((glm::dot(before, after) >= .9999)){// || (points[i].fEquals(points[i+j], 0.01))) {
					if((i+j) >= points.size())
					{
						after = glm::vec3(0,0,1);  //patch for broken spline case 
						break;
					}
					antPt = points[i+j];
					after = tarPt - antPt;
					after.y = 0;
					j++;
				}
				glm::vec3 axis;
                axis = glm::cross(before, after);   //axis of rotate
                glm::vec3 perpen = glm::cross(before, after);  //right angle turn.	
				
                if (glm::dot(perpen, after))
				{
					perpen *= -1;  //perpen should point in same general direction as the next point
				}
				edgeDiff = perpen;
                edgeDiff /= glm::length(edgeDiff);
			}
			else{  //case: need a tangent that points to the next location
				edgeDiff = antPt-postPt;  //chord
                edgeDiff /= glm::length(edgeDiff);
			}	

			f32 rotationAngle = CSplineFunctions::AngleCos(tarPt, before, after);  

			//scale magnitude
            f32 postMag = glm::length(tarPt - postPt);
            f32 antMag = glm::length(tarPt - antPt);
			f32 Mag = postMag < antMag? postMag : antMag;  //magnitude of shortest distance to point
			Mag /= 2.0;
			f32 minMag = .2;   //minimum magnitude for the thing.
			f32 scale = minMag + ((Mag-minMag) * (((3.14159265-rotationAngle)*(3.14159265-rotationAngle)) / 3.14159265 )); //Lerp ease scale

			//tangent calculation
			tan = edgeDiff * scale;
			if(!CSplineFunctions::fEquals(tan, tan, 0.1))  //NaN bandAid TODO
			{
				tan = glm::vec3(0,0,2);
			}
			//tan.Print(); 
			tangents.push_back(tan);
		}

		//init the last point
		tan = points[points.size()-1] - points[points.size()-2];
		tangents.push_back(tan);
	}

	//Fritsch-Carlson
	f32 hm1 = 0.0;  //d_{i-1}
	f32 hm1x = 0.0;  //d_{i-1}
	f32 dm1 = 0.0; //h_{i-1}
	f32 dm1x = 0.0; //h_{i-1}

	glm::vec3 endTangent = glm::vec3(0, 0, 0);

	tTangents.push_back( endTangent );  //tangent at first point
	//Time-space tangent generation
	for(int i = 1; i < points.size()-1; i++)
	{
		glm::vec3 ttan;
		//first create the direction of the tangent. then scale it to the needed magnitude
		glm::vec3 antPt = glm::vec3(totts[i+1], 0, i+1);   //anterior
		glm::vec3 tarPt = glm::vec3(totts[i],   0, i);   //center target

		f32 hi = antPt.x - tarPt.x;
		f32 di = (antPt.z - tarPt.z)/hi;

		f32 yTan;
		if((dm1 > 0 && di > 0) || (dm1 < 0 && di < 0) || (dm1 == 0 && dm1 == di) )  //same sign
		{
			yTan = 3.0*(hm1 + hi);
			yTan *= (1.0 / (((2.0*hi+hm1)/dm1) + ((2.0*hm1+hi)/di) ));
		}
		else
		{
			yTan = 0;
		}

		f32 hix = antPt.z - tarPt.z;
		f32 dix = (antPt.x - tarPt.x)/hix;

		f32 xTan;
		if((dm1x > 0 && dix > 0) || (dm1x < 0 && dix < 0) || (dm1x == 0 && dm1x == dix) )  //same sign
		{
			xTan = 3.0*(hm1x + hix);
			xTan *= (1.0 / (((2.0*hix+hm1x)/dm1x) + ((2.0*hm1x+hix)/dix) ));
		}
		else
		{
			xTan = 0;
		}

		ttan = glm::vec3(xTan, 0, yTan);
		//ttan.Print(); 
		tTangents.push_back(ttan);
		hm1 = hi;
		hm1x = hix;
		dm1 = di;
		dm1x = dix;
	}

	//init the last point
	tTangents.push_back(endTangent);


}


/*Interpolation helper function. Computs the matrix multiplcation between a u matrix, M (spline matrix) and B (points)
 * returns a single result. Run three times to get x, y, and z (full vertex) */
f64 CSplinePath::HmInt(f32 U[4], const f32 M[16], f32 B[4])
{
	f32 UM[4];
	UM[0] = U[0]*M[0] + U[1]*M[4] + U[2]*M[8] + U[3]*M[12];
	UM[1] = U[0]*M[1] + U[1]*M[5] + U[2]*M[9] + U[3]*M[13];
	UM[2] = U[0]*M[2] + U[1]*M[6] + U[2]*M[10] + U[3]*M[14];
	UM[3] = U[0]*M[3] + U[1]*M[7] + U[2]*M[11] + U[3]*M[15];

	return UM[0]*B[0] + UM[1]*B[1] + UM[2]*B[2] + UM[3]*B[3];
}



/*matrix spline interpolation
 * * returns a vector location 
 * * It is a wrapper around catmullMatrix()
 * * The arc length u is converted into the spline's true u while interpolating*/
glm::vec3 CSplinePath::splineLocation(f32 curLocation, int startPoint)
{
	if(tabSet) {  //the very first runs of this function creates the table. So the u should not be converted
		curLocation = StoU(curLocation, startPoint); //convert arcLength based u into the spline's true u.
	}
	if(isCatmullMode)
	{
		return catmullMatrix(curLocation, startPoint);
	}
	else
	{
		int endPoint = startPoint + 1;   //TODO found this line. dangerous. set endPoint properly
		return hermiteMatrix(curLocation, points[startPoint], points[endPoint],
				tangents[startPoint], tangents[endPoint]);
	}
}

// interpolation for the time of movement (time-space curve, rather than the space curve of the other function)
// Returns a f64 value representing the new U value to input into the space curve
// takes in the amount of time (seconds) since program start, and the current Knot.
f64 CSplinePath::convertTimestampToU(f32 timer, int curKnot )
{
	int endMark = totts.size();
	int endLocNum; //index to the end point;
	glm::vec3 startTan;
	glm::vec3 startTime;
	glm::vec3 endTime;
	glm::vec3 endTan;

	startTime = glm::vec3(totts[curKnot], 0, curKnot);
	startTan = tTangents[curKnot];
	if(curKnot+1 >= endMark) {
		endLocNum = 0;
	}
	else {
		endLocNum = curKnot+1;
	}

	endTime = glm::vec3(totts[endLocNum], 0, endLocNum); //provided it's not at the end of the spline, endLocation is start+1
	endTan = tTangents[endLocNum];

	
	f32 timeU = f64Lerp(timer, totts[curKnot], totts[endLocNum], 0.0, 1.0);
	f32 interpolated =  hermiteMatrix(timeU, startTime, endTime, startTan, endTan).z;
	return f64Lerp(interpolated, curKnot, endLocNum, 0.0, 1.0);  //convert interpolated between zero and one.
	//return f64Lerp(res, startTime, endTime, 0.0, 1.0);

}

/*igeneralized catmull-rom matrix multiplcation for complex interpolations
 * returns a vector
 * curLocation is a number between zero and one, and represents percentage of progress
 * See a math reference for how this works. */
glm::vec3 CSplinePath::hermiteMatrix(f32 curLocation, glm::vec3 startLocation, glm::vec3 endLocation,
		glm::vec3 startTangent, glm::vec3 endTangent)
{
	glm::vec3 res = glm::vec3();

	f32 dU[4] = {curLocation*curLocation*curLocation, curLocation*curLocation, curLocation, 1 };

	f32 Bx[4] = {startLocation.x, endLocation.x, startTangent.x, endTangent.x};
	f32 By[4] = {startLocation.y, endLocation.y, startTangent.y, endTangent.y};
	f32 Bz[4] = {startLocation.z, endLocation.z, startTangent.z, endTangent.z};

	res.x =  HmInt(dU, Mher, Bx);
	res.y =  HmInt(dU, Mher, By);
	res.z =  HmInt(dU, Mher, Bz);
	return res;
}

/*Given a curLocation (u) and a point to start at, find a catmull interpolation of a point's actual location past the point (knot) */
glm::vec3 CSplinePath::catmullMatrix(f32 curLocation, int curKnot) 
{
	int endMark = points.size();
	int endLocNum; //index to the end point;
	glm::vec3 historyLocation;
	glm::vec3 startLocation;
	glm::vec3 endLocation;
	glm::vec3 futureLocation;

	//Need to derive three nighboring points next to the current point (one behind, two ahead). 
	//Array bounds need to be checked, and they wrap.   
	if(curKnot > 0) {

		historyLocation = points[curKnot-1];  //not at beginning of spline
	}
	else {
		historyLocation = (points[curKnot+1]*.5f) - (points[curKnot+2]-points[curKnot+1]) - points[curKnot]; //initial tangent at beginning of spline
	}

	startLocation = points[curKnot];

	if(curKnot+1 >= endMark) {
		endLocNum = 0;
	}
	else {
		endLocNum = curKnot+1;
	}

	endLocation = points[endLocNum]; //provided it's not at the end of the spline, endLocation is start+1
	if(endLocNum+1 >= endMark) {
		futureLocation = points[0];
	}
	else {
		futureLocation = points[endLocNum+1];
	}


	//Prepare the matrices used for catmull interpolation
	glm::vec3 res = glm::vec3();
	f32 dU[4] = {curLocation*curLocation*curLocation, curLocation*curLocation, curLocation, 1};

	f32 Bx[4] = {historyLocation.x, startLocation.x, endLocation.x, futureLocation.x};
	f32 By[4] = {historyLocation.y, startLocation.y, endLocation.y, futureLocation.y};
	f32 Bz[4] = {historyLocation.z, startLocation.z, endLocation.z, futureLocation.z};

	res.x =  HmInt(dU, Mcat, Bx);
	res.y =  HmInt(dU, Mcat, By); //this spline does not reflect y axis 
	res.z =  HmInt(dU, Mcat, Bz);
	return res;
}

//Reparameterizes the spline so that the u value used for matrix interpolation represents distance over the xyz coordinate units.
//Both the original u, the reparameterized u (u prime), and the coordinate distance is stored in a table.
//The way RParent talks about length is confusing. In adaptive gaussian integration, the u prime value is referred to as length. It is not the coordinate distance length. 
void CSplinePath::parameterizeSpline()
{
	//for each point (except the last) on the original path, record the table inbetween each set of knots.
	for(int i = 0; i < points.size()-1; i++)
	{
		//create temp curve structure, feed it into the algorithm
		cubicCurveTd curArc;	

		//create next table to fill
		CSplineTable *curTab = new CSplineTable();
		paramfuncs = ParamFunctions(curTab);
		//Abandoning AGI. Now calculating U' after coordinate based distances are mapped to U
		//f64 oldk = 0;
		for(int kount = 0; kount < totalSlices; kount++) //divide the curve into 10 parts between each knot. 
			//Store their arc lengths (in u, not coords) in table
		{
			f64 kk = kount*(1.0/totalSlices);
			//f64 arclen = paramfuncs.adaptiveIntegration(&curArc, oldk, kk, Tolerance); //arclen supposed to be [0,1]. It is not
			//oldk = kk;
			curTab->addTableEntry(kk, -1);
		}

		f64 size = 0.0;
        glm::vec3 lastPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		lastPoint = points[i]; //= splineLocation(curTab->getU(0), i);	
		f64 totalDistance = 0; //the total coord distance to do linear interpolation over, to get u'
		//Setting distances in unit coordinates, in addition to saving the u values -li-ke-- ab-o-ve- like below.
		for(int r =0; r < totalSlices; r++){
			if(!r){
				size = 0.0;
			}
			else{
                glm::vec3 thisPoint = glm::vec3(0.0f, 0.0f, 0.0f);
                thisPoint = splineLocation(curTab->getU(r - 1), i);
				size = glm::length(thisPoint - lastPoint); 
				totalDistance += size;
				lastPoint = thisPoint;
			}
			curTab->setDist(size, r); //place coordinate distance into tables.
		}

		//and now to finally calculate u prime, the parameterized version of u.
		size = 0;
		lastPoint = points[i];
		curTab->setLength(0, 0); //first value is trivial.
		for(int r = 1; r < totalSlices; r++) {
			//do this again to get the current size
			glm::vec3 thisPoint = splineLocation(curTab->getU(r-1), i);
			size += glm::length(thisPoint - lastPoint);

			//this time, interpolate linearly to totalDistance. start interval [0, totalDistance]. end interval [0,1] 
			f64 result = f64Lerp(size, 0, totalDistance, 0.0, 1.0);
			curTab->setLength(result, r); //the way RParent talks about length makes it confusing. setLength is setting u prime, not the coordinate length.
			//printf("%f %f \n", curTab->getU(r), curTab->getDist(r), result);
			lastPoint = thisPoint;
		}

		paramTable.push_back(curTab);

	}
	tabSet = true;
}

/*gets a point a certain (positive) distance away (in unit coordinate distance), 
 * it needs teh distance, the starting point and the current u position after the point  */
glm::vec3 CSplinePath::getNearbyPoint(f64 distanceAhead, int startPoint, f64 startU)
{
	f64 curU = startU;
	f64 result = -1;
	int curPoint = startPoint;

    glm::vec3 nearbyPoint = points[startPoint];


	if(distanceAhead > 0) { //looking forward
		int i = 0;
		for(i = startPoint; i < paramTable.size(); i++) {
			result = paramTable[curPoint]->getAheadU(curU, distanceAhead);  
			if(result <= 0) {  //didn't find point yet, keep going
				curU = 0; //start at the beginning of the point from now on.
				distanceAhead = distanceAhead + result;  //this subtracts(!) the distance already traveled
				nearbyPoint = splineLocation(1,curPoint);
				curPoint++; 
			}
			else if(result > 0) {  // found the point, or as far as this can move
				nearbyPoint = splineLocation(result, curPoint);
				break;
			}
			//printf(" %f %f \n", result, distanceAhead);
		}
		if(i == paramTable.size()) { //point wasn't located. so it's at the end.
			nearbyPoint = splineLocation(1, i-1);
		}
	}
	//TODO investigate backwards anomoly when hitting the 1.000 table index on a point
	else if(distanceAhead < 0) { //looking backward 
		int i = 0;
		for(i = startPoint; i > 0 ; i--) {
			result = paramTable[curPoint]->getBehindU(curU, -distanceAhead);
			//printf("backwards %f %f (%f)\n", curU, result, distanceAhead);
			if(result <= 0) {  //didn't find point yet, keep going
				curU = 1; //start at the end of the point from now on.
				distanceAhead = distanceAhead - result;  //this subtracts(!) the distance already traveled. both nums negative
				nearbyPoint = splineLocation(0, curPoint);
				curPoint--; 
			}
			else if(result > 0) {  // found the point, or as far as this can move
				nearbyPoint = splineLocation(result, curPoint);
				break;
			}
		}
		if(i == 0) { //reached the very start without finding the point
			nearbyPoint = splineLocation(0,0);
		}
	}


	return nearbyPoint;
}

/*S to U: Find the value for the space curve u given a distance value between zero and one, between the current point and the next point */
f64 CSplinePath::StoU(f64 sDist, int curPoint)
{
	//Pthis = Plast - f(Plast)/f'(plast);
	//f = s - LENGTH(u1, U(plast)) = 0;
	//U = parametric value of point on parameterized curve

	//find the first arclength value higher than the target distance, then try and linearlly interpolate between them
	//really this s to U conversion is an approximation

	int i = -1;
	f64 aDist = 0;
	while((i<totalSlices-1) && (aDist <= sDist + .000001))	
	{
		i++; //TODO exit when curPoint breaches buffer
		aDist = paramTable[curPoint]->getLength(i);
		//printf("comparing %lf >= %lf && %lf <= %lf\n", aDist, sDist-.0000001, aDist, sDist+.0000001);
		if(aDist >= sDist-.0000001 && aDist <= sDist+.0000001)
		{
			return paramTable[curPoint]->getU(i); //exact match case
		}
	}
	int h = i-1;  //h is the last relevant distance, i is the next one.
	f64 simpleU = 0;
	if(h >= 0)
	{
		//interpolate between values in the table, since sDist is probably between table points more oftern than not. 
		simpleU = f64Lerp(sDist, paramTable[curPoint]->getLength(h), paramTable[curPoint]->getLength(i),
				paramTable[curPoint]->getU(h), paramTable[curPoint]->getU(i));
	}
	else //first entry on table being interpolated
	{
		simpleU = f64Lerp(sDist, 0, paramTable[curPoint]->getLength(i),
				0, paramTable[curPoint]->getU(i));
	}
	return simpleU;
}

/*Draws line between the previous line and the current one */
void CSplinePath::drawPointLine(int i)//, Frustum* frustum)
{
	/*glPushMatrix();
	{
		glTranslatef(gPoint(i).x, gPoint(i).y, gPoint(i).z);
		if(frustum->testPoint(gPoint(i)))
		{
			glutSolidSphere(.1, 3, 2);
		}
	}glPopMatrix();*/
	if(i > 0)
	{
		//if(frustum->testPoint(gPoint(i)) || frustum->testPoint(gPoint(i-1)))
	//	{
			//splined points for more curvature
			/*glm::vec3 p1 = splineLocation(.1, i-1);
			glm::vec3 p2 = splineLocation(.2, i-1);
			glm::vec3 p3 = splineLocation(.3, i-1);
			glm::vec3 p4 = splineLocation(.4, i-1);
			glm::vec3 p5 = splineLocation(.5, i-1);
			glm::vec3 p6 = splineLocation(.6, i-1);
			glm::vec3 p7 = splineLocation(.7, i-1);
			glm::vec3 p8 = splineLocation(.8, i-1);
			glm::vec3 p9 = splineLocation(.9, i-1);*/

            for (double ind = 0.1; ind < 1.0; ind += 0.1) {
                glm::vec3 p = splineLocation(ind, i - 1);

                //p = glm::normalize(p);

                posBuf.push_back(p.x);
                posBuf.push_back(p.y);
                posBuf.push_back(p.z);

                colorBuf.push_back(0.0f);
                colorBuf.push_back(0.0f);
                colorBuf.push_back(0.0f);

                indBuf.push_back(currIndex++);
            }

            

			/*glBegin(GL_LINES);
			glVertex3f(gPoint(i-1).x, gPoint(i-1).y, gPoint(i-1).z);
			glVertex3f(p1.x, p1.y, p1.z);

			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);

			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);

			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p4.x, p4.y, p4.z);

			glVertex3f(p4.x, p4.y, p4.z);
			glVertex3f(p5.x, p5.y, p5.z);

			glVertex3f(p5.x, p5.y, p5.z);
			glVertex3f(p6.x, p6.y, p6.z);

			glVertex3f(p6.x, p6.y, p6.z);
			glVertex3f(p7.x, p7.y, p7.z);

			glVertex3f(p7.x, p7.y, p7.z);
			glVertex3f(p8.x, p8.y, p8.z);

			glVertex3f(p8.x, p8.y, p8.z);
			glVertex3f(p9.x, p9.y, p9.z);

			glVertex3f(p9.x, p9.y, p9.z);
			glVertex3f(gPoint(i).x, gPoint(i).y, gPoint(i).z);
			glEnd();*/
		//}
	}
}



void CSplinePath::drawPoints(int curPoint, bool usesGhostPoints)
{
	int step = 1;
    currIndex = 0;

	//glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);
	f32 blue = 1.0;
	f32 red = 1.0;
	f32 green = 1.0;
	int i;
	int chunk = size()*.1 / 4;
	int endRange = limitedDrawing? curPoint + 500 : (curPoint+(size()*.1));
	int pastRange = limitedDrawing? curPoint - 100 : (usesGhostPoints? 1 : 0 );

	//frustum->extract();

    //glm::vec3 currColor;

	for(i = curPoint+1; i < endRange; i += step)
	{
		//glColor3f(red,green,blue);
        currColor = glm::vec3(red, green, blue);
        drawPointLine(i);// , frustum);
		if(i > curPoint+chunk) red -= .02;
		if(i > curPoint+(3.0*chunk)){ red += .02; green -=.01; }
		if(i < curPoint+(chunk*2)){ blue -= .02; }
		if(i > curPoint+(chunk*2)){ blue += .02; }

	}


	//Past points drawn in red
	//glColor3f(1,0,0);
    currColor = glm::vec3(1.0f, 0.0f, 0.0f);

	i = pastRange;
	if(usesGhostPoints && pastRange < 1){
		i = 1;
	}
	else if(pastRange < 0){i = 0;}

	for( ; i <= curPoint; i++)
	{
        drawPointLine(i);// , frustum);
	}

	//Far future points drawn in black, if not limiting Drawing
	if(!limitedDrawing)
	{
		//glColor3f(0,0,0);
        currColor = glm::vec3(0.0f, 0.0f, 0.0f);
		for(i = (curPoint+(size()*.1)) ;i < (usesGhostPoints?size()-1: size()); i ++)
		{
            drawPointLine(i);// , frustum);
		}
	}
	//glColor3f(1,1,1);
    currColor = glm::vec3(1.0f, 1.0f, 1.0f);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT0);
    currIndex = 0;
}