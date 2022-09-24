/*
 *  pointMass.h
 *  laserBlob
 *
 *  Created by CASSINELLI ALVARO on 5/19/11.
 *  Copyright 2011 TOKYO UNIVERSITY. All rights reserved.
 *
 */

#ifndef POINTMASS_H
#define POINTMASS_H

#include "myVectorClass.h"

#define VERLET_METHOD // comment this to have EULER method

class pointMass
{
public:
	
	// ==================================== METHODS ====================================
	pointMass();
	virtual ~pointMass(){};
	
	// Adding forces to total force:
	void resetForce();
	void addForce(float fx, float fy);
	void addForce(vector2D force);
    
	void addDampingForce();
	void addInvSquareForce(float x, float y, float radiusMax, float radiusMin, float scale);
	void addInterInvSquareForce(pointMass &p, float radiusMin, float radiusMax, float scale);
		
	// (a blob object could be defined by a "cord" of chained particles, plus a center)
	void addSpringForce(float x, float y,  float radius, float scale);
	void addInterSpringForce(pointMass &p, float radius, float scale);
	//void addClockwiseForce(particle &p, float radius, float scale);
	//void addCounterClockwiseForce(particle &p, float radius, float scale);
	
	//void addDampingForce(); // this work in the case of the euler integration; in case of Verlet, we need to do pseudo-damping while calculating
	// the acceleration... 
	
	// Set parameters:
	void setInitialCondition(float px, float py, float vx, float vy);
	void setIntegrationStep(float _dt);
	
	void setPos(float px, float py); // assuming the speed is unchanged (must do some tweaking in case of Verlet integration)
	
	// dynamic update:
	void update();
	
	// kinematic constraints (could be based on a force too...)
	void setWallLimits(float maxx, float maxy, float minx, float miny);
	void bounceOffWalls();
    
    
    // Collision detection:
    // 1) With borders of the screen:
    bool didCollideBorders();
    
    // 2) With anything in the background (framebuffer!!)
    bool didCollideDrawing(float _grayLevelThresholdCollision, ofImage& _imgBackground, float _radiusPixelCollision);
	
	vector2D getSpeed(); // get an estimation of the speed (also update speed variable - this variable is not needed in case of VERLET)
	void setSpeed(const vector2D& vel);
	void setSpeed(float vx, float vy);
	
	// ==================================== VARIABLES ====================================
	
	int identifier; // this may be needed in particular in case we don't use vector<> (case of poor C Arduino compiler)
	
	// kinematic variables:
	vector2D  pos, posOld; // I will use verlet integration (perhaps we could have a switch to choose the integration method?)
	//vector2D  speed; // speed at time t (this is not explicitly calculated in case of verlet method, HENCE PRIVATE)
	vector2D  acc;   // Acceleration at time t (equal to the total force divided by the mass). No real need to have it here, but convenient to check. 
	vector2D  totalForce; // this is just for convenience and speeding up calculation when adding forces, before computing acc. 
	
	// integration step:
	float dt; 
	
	// physical parameters:
	float dampMotion;
	float dampBorder;
	vector2D maxWall, minWall; 
	float mass;
	bool  bFixed; // these could act as control points that could be loaded (letters...). In fact, we could use mass to set this (like mass=-1)

	
protected:
	
private:
	vector2D  speed; // speed at time t (this is not explicitly calculated in case of verlet method, HENCE PRIVATE)
	
};



#endif //POINTMASS_H
