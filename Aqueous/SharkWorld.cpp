#include "SharkWorld.h"


double doubleLerp(double input, double minx, double maxx, double miny, double maxy)
{
	return miny + (input-minx) * ((maxy-miny)/(maxx-minx));
}

//Looks at the curvature of the rail path, and return an angle (degrees) representing the current angle *
int SharkWorld::deriveRailAngle()
{
	return traveler.deriveRailAngle(lookAhead, frontby, behindby);
}

glm::vec3 SharkWorld::updateWorld(int dt)
{
	//location = upCurrentLocation();
	return traveler.update(dt);
	//traveler.gLocation().Print();
}
void SharkWorld::displayWorld()
{
	//glPushMatrix();
	//{
		//glm::vec3 center = traveler.gLocation();
		//glm::vec3 center = location;
		//glPushMatrix();
		//{
			//glTranslatef(-center.x, -center.y, -center.z );	
			//drawSkybox();
			//drawPoints();
			//traveler.drawStatic();
			traveler.drawAndMoveCamera();
		//}glPopMatrix();
	//}glPopMatrix();
}

/*void SharkWorld::drawSkybox()
{
	glPushMatrix();
	glm::vec3 location = traveler.gLocation();
	double xx = 90; //location.x + 90; //maxPt.x + 20;
	double xy = 90; //location.y + 90; //maxPt.y + 20;
	double xz = 90; //location.z + 90; //maxPt.z + 20;
	double nx = -90; //location.x - 90; //minPt.x - 20;
	double ny = -90; //location.y - 90; //minPt.y - 20;
	double nz = -90; //location.z - 90; //minPt.z - 20;
	//double step = ((maxPt.y+10) - (minPt.y-10))/3.0;

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();{
		if(skyboxrotation> 2.0*3.1415926) {skyboxrotation = 0;}
		else {skyboxrotation += 1.0*(3.1415926/180.0);}
		glRotatef(skyboxrotation, 0,1,0);
		glBegin(GL_QUADS);
		{
			//top
			glBindTexture(GL_TEXTURE_2D, 0);
			glColor3f(0, .6, 1);
			glNormal3f(0, -1, 0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xx, xy, xz);	
			glTexCoord2f(0.0, 1.0);
			glVertex3f(nx, xy, xz);	
			glTexCoord2f(1.0, 1.0);
			glVertex3f(nx, xy, nz);	
			glTexCoord2f(1.0, 0.0);
			glVertex3f(xx, xy, nz);
			
			//side 1
			glBindTexture(GL_TEXTURE_2D, 1);
			glNormal3f(0, 0, -1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xx, xy, xz);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(xx, xy, nz);
			glColor3f(0,0,.5);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(xx, ny, nz);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(xx, ny, xz);

			//side 2
			glNormal3f(-1,0,0);
			glColor3f(0, .6, 1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(xx, xy, xz);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(nx, xy, xz);
			glColor3f(0,0,.5);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(nx, ny, xz);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(xx, ny, xz);
				
			//side 3
			glNormal3f(1,0,0);
			glColor3f(0, .6, 1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(nx, xy, nz);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(nx, xy, xz);
			glColor3f(0,0,.5);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(nx, ny, xz);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(nx, ny, nz);

			//side4 
			glNormal3f(0,0,1);
			glColor3f(0, .6, 1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(nx, xy, nz);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(xx, xy, nz);
			glColor3f(0,0,.5);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(xx, ny, nz);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(nx, ny, nz);

			//bottom
			glBindTexture(GL_TEXTURE_2D, 2);
			glNormal3f(0,1,0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(nx, ny, xz);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(xx, ny, xz);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(xx, ny, nz);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(nx, ny, nz);

		}glEnd();
	}glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}*/
/*
 void SharkWorld::drawPointLine(int i)
{
	glPushMatrix();
	{
		glTranslatef(path.gPoint(i).x, path.gPoint(i).y, path.gPoint(i).z);
		if(pointInFrustum(path.gPoint(i))){
		       	glutSolidSphere(.1, 3, 2);
		}
	}glPopMatrix();
	if(i > 0)
	{
		if(pointInFrustum(path.gPoint(i)) || pointInFrustum(path.gPoint(i-1)))
		{
			//splined points
			glm::vec3 p1 = path.splineLocation(.1, i-1);
			glm::vec3 p2 = path.splineLocation(.2, i-1);
			glm::vec3 p3 = path.splineLocation(.3, i-1);
			glm::vec3 p4 = path.splineLocation(.4, i-1);
			glm::vec3 p5 = path.splineLocation(.5, i-1);
			glm::vec3 p6 = path.splineLocation(.6, i-1);
			glm::vec3 p7 = path.splineLocation(.7, i-1);
			glm::vec3 p8 = path.splineLocation(.8, i-1);
			glm::vec3 p9 = path.splineLocation(.9, i-1);
	
			glBegin(GL_LINES);
			glVertex3f(path.gPoint(i-1).x, path.gPoint(i-1).y, path.gPoint(i-1).z);
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
			glVertex3f(path.gPoint(i).x, path.gPoint(i).y, path.gPoint(i).z);
			glEnd();
		}
	}
}
void SharkWorld::drawPoints()
{
	int step = 1;
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	float blue = 1.0;
	float red = 1.0;
	float green = 1.0;
	int i;
	int chunk = path.size()*.1 / 4;
	ExtractFrustum();

	//TODO: delete these testing spheres. 
	glPushMatrix();
        {
		glColor3f(0,.52,.86);
		float u;
		if(curPoint == 0){ u = 0;}
		else { u = (float)steps/(float)totalSteps; }
		glm::vec3 testAhead = path.getNearbyPoint(-.3 , curPoint, u );
		glTranslatef(testAhead.x, testAhead.y, testAhead.z);
		glutSolidSphere(.1, 3, 2);
	}glPopMatrix();
	glPushMatrix();
        {
		glColor3f(0,.52,.86);
		float u;
		if(curPoint == 0){ u = 0;}
		else { u = (float)steps/(float)totalSteps; }
		glm::vec3 testAhead = path.getNearbyPoint(.3 , curPoint, u );
		glTranslatef(testAhead.x, testAhead.y, testAhead.z);
		glutSolidSphere(.1, 3, 2);
	}glPopMatrix();
	glPushMatrix();
        {
		glColor3f(1,.52,.86);
		float u;
		if(curPoint == 0){ u = 0;}
		else { u = (float)steps/(float)totalSteps; }
		glm::vec3 testAhead = path.getNearbyPoint(.01 , curPoint, u );
		glTranslatef(testAhead.x, testAhead.y, testAhead.z);
		glutSolidSphere(.1, 3, 2);
	}glPopMatrix();

	//Close future points are drawn in a gradient going from white to yellow to green to black
	for(i = curPoint+1; i < (curPoint+(path.size()*.1)); i += step)
	{
		glColor3f(red,green,blue);
		drawPointLine(i);
		if(i > curPoint+chunk) red -= .02;
		if(i > curPoint+(3.0*chunk)){ red += .02; green -=.01; }
		if(i < curPoint+(chunk*2)){ blue -= .02; }
		if(i > curPoint+(chunk*2)){ blue += .02; }
		
	}
	
	//Past points drawn in red
	glColor3f(1,0,0);
	for(i = 0; i <= curPoint; i++)
	{
		drawPointLine(i);	
	}

	//Far future points drawn in black
	glColor3f(0,0,0);
	for(i = (curPoint+(path.size()*.1)) ;i < path.size(); i ++)
	{
		drawPointLine(i);	
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

*/


