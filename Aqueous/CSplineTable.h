#pragma once

#include "CProgramContext.h"

using namespace std;

class CSplineTableEntry
{
	public:
	f64 u;  // the original u valueon the spline
	f64 dist; //the approximate distance down the spline, from the last sample point in the table. 
	f64 length; //u prime, the new u
};

class CSplineTable
{
	public:
		CSplineTable(){}
		~CSplineTable(){}
		void addTableEntry(f64 u, f64 len){
			CSplineTableEntry g; 
			g.u = u; 
			g.length = len;
			bool success = false;
			//enforce that stable is ordered by u.
			for(vector<CSplineTableEntry>::iterator iv = sTable.begin(); iv != sTable.end(); iv++) {
				if(u < iv->u){
					sTable.insert(iv, g);
					success = true;
				}
			}
			if(!success){	
				sTable.push_back(g);
			}
		}

		CSplineTableEntry get(int index){return sTable.at(index);}
		f64 getU(int index){return sTable.at(index).u;}
		f64 getDist(int index){return sTable.at(index).dist;}
		f64 getLength(int index){return sTable.at(index).length;}
		void setDist(f64 ns, int index){sTable[index].dist = ns;}
		void setU(f64 newu, int index){sTable[index].u = newu;}
		void setLength(f64 uprime, int index){ sTable[index].length = uprime;}
		int getSize(){ return sTable.size();}

		/*return teh table index that is closest to a s value */
		int getIndexFromS(f64 s){
			f64 target = 0;
			int i = 0;
			while(target < s){
				target += getDist(i);
				i++;
				if(i >= sTable.size() && target < s){
					return -1;
				}
			}
			return i;
		}

		f64 calcDistance(f64 s1, f64 s2){ //TODO wrong
			int start =  getIndexFromS(s2);
		       	int end = getIndexFromS(s1);
			f64 dist = 0;
			while (start < end)
			{
				dist += getDist(start);
				start++;
			}
			return dist;
		}

		/*Given a u starting point, find a u' down the path a set distance away 
		 * return the (negative) distance travelled if the end point is reached before the goal is found,
		 * otherwise, return the (positive) u' value
		 * start1 needs to be >= 0. targetCoordDist must be positive */
		f64 getAheadU(f64 start1, f64 targetCoordDist)
		{
			f64 distanceTraveled = 0;
			int i = 0;
			int foundIndex = -1;
			bool startFound = false;
			if(targetCoordDist < 0) {
				//printf("target distance down path is negative\n");
			}

			for(i = 0; i < sTable.size(); i++) {
				if(!startFound && start1 < getLength(i) ) {
					startFound = true;
				}
				else if(startFound && distanceTraveled < targetCoordDist) {
					distanceTraveled += getDist(i);		
				}
				else if(startFound && distanceTraveled >= targetCoordDist) {
					foundIndex = i;
				        break;	
				}
			}
			//check to make sure foundIndex was really found. If it hasn't, that means that it is on the next point
			if(foundIndex < 0) {
				return -distanceTraveled; 
			}
			else
			{
				return getLength(foundIndex);
			}

		}

		/*Like getCloseU, but searches backwards on the spline.
		 * Still returns (negative) distanceTraveled if the spot isn't found
		 * return (positive) u' if it is found */
		f64 getBehindU(f64 start1, f64 targetCoordDist)
		{
			f64 distanceTraveled = 0;
			int i = 0;  
			int foundIndex = -1;
			bool startFound = false;

			if(targetCoordDist < 0) {
				//printf("target distance down path is negative\n");
			}

			for(i = sTable.size()-1; i >= 0; i--) {
				if(!startFound && start1 > getLength(i) ) {
					startFound = true;
				}
				else if(startFound && distanceTraveled < targetCoordDist) {
					distanceTraveled += getDist(i);		
				}
				else if(startFound && distanceTraveled >= targetCoordDist) {
					foundIndex = i;
				        break;	
				}
			}
			//check to make sure foundIndex was really found. If it hasn't, that means that it is on the next point
			if(foundIndex < 0) {
				return -distanceTraveled; 
			}
			else
			{
				return getLength(foundIndex);
			}
			
		}

	private:
		vector<CSplineTableEntry> sTable;
};
