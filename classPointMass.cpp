/*
 *  pointMass.cpp
 *  laserBlob
 *
 *  Created by CASSINELLI ALVARO on 5/19/11.
 *  Copyright 2011 TOKYO UNIVERSITY. All rights reserved.
 *
 */

#include "classPointMass.h"

//------------------------------------------------------------
pointMass::pointMass(){
	setIntegrationStep(.01); // default in case we don't call integration step setting
	// NOTE: it is important to set dt before inital conditions in case of VERLET integration,
    // because we need the integration step for properly setting the initial speed.
    
	setInitialCondition(0,0,0,0);// default in case we don't call to initial conditions.
    
	setWallLimits(2000, 2000, 300, 300);
	mass=1.0; 
	dampMotion = 0.08f;
	dampBorder = 0.07f;
	bFixed = false;
}

//------------------------------------------------------------
void pointMass::resetForce(){
    totalForce.set(0,0);
}

//------------------------------------------------------------
void pointMass::addForce(float x, float y){
	totalForce.x = totalForce.x + x;
    totalForce.y = totalForce.y + y;
}

//------------------------------------------------------------
void pointMass::addForce(vector2D forceToAdd){
	totalForce+=forceToAdd;
}

//------------------------------------------------------------
void pointMass::addInvSquareForce(float x, float y, float radiusMin, float radiusMax, float scale){
    
	vector2D posOfForce;
	posOfForce.set(x,y);
		
	vector2D diff	= pos - posOfForce; // note: we use the position AT TIME T, so this force is at time t
	float length	= diff.length();
	
	// check close enough and far enough (to avoid singularities for example):
	if ((length>radiusMin)&&(length<radiusMax)) {
        diff.normalize();
		totalForce += diff * scale * 1.0/(length*length+1);
    }
}

void pointMass::addInterInvSquareForce(pointMass &theOtherParticle, float radiusMin, float radiusMax, float scale){
    
	vector2D posOfForce;
	posOfForce.set(theOtherParticle.pos);
	
	vector2D diff	= pos - posOfForce; // note: we use the position AT TIME T, so this force is at time t
	float length	= diff.length();
	
	// check close enough and far enough (to avoid singularities for example):
	if ((length>radiusMin)&&(length<radiusMax)) {
        diff.normalize();
		totalForce += diff * scale * 1.0/(length*length+1);
		theOtherParticle.totalForce -= diff * scale * 1.0/(length*length+1);
    }
}


//------------------------------------------------------------
void pointMass::addSpringForce(float centerx, float centery, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	vector2D posOfForce;
	posOfForce.set(centerx, centery);
	
	// ----------- (2) calculate the difference & length 
	
	vector2D diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		totalForce += diff * scale * pct;
	}
}

void pointMass::addInterSpringForce(pointMass &theOtherParticle, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	vector2D posOfForce;
	posOfForce.set(theOtherParticle.pos);
	
	// ----------- (2) calculate the difference & length 
	
	vector2D diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		totalForce += diff * scale * pct;
		theOtherParticle.totalForce -= diff * scale * pct;
		//theOtherParticle.frc.x = p.frc.x - diff.x * scale * pct;
        //theOtherParticle.frc.y = p.frc.y - diff.y * scale * pct;
    }
}



//------------------------------------------------------------
void pointMass::addDampingForce(){ // NOTE: use only in case of EULER intgration!
    totalForce-= speed* dampMotion;
}

//------------------------------------------------------------
void pointMass::setIntegrationStep(float _dt){
	dt=_dt;
}

//------------------------------------------------------------
void pointMass::setInitialCondition(float px, float py, float vx, float vy){
#ifndef VERLET_METHOD
	pos.set(px,py);
	speed.set(vx,vy);
#else
	// In case of Verlet method, setting the speed is a little more complicated. It involves in particular the integration step
	// through the approximation formula: 
	// speed = (posNew-posOld)/(2*dt), or speed=(pos-posOld)/dt. Hence:
	posOld.set(px, py); 
	setSpeed(vx, vy); // this assumes posOld known
#endif	
}

//-------------------------------------------------------
vector2D pointMass::getSpeed() {
	// this will give an estimate of the speed (not computed explicitly using the Verlet integration):
	//speed=(posNew-posOld)/(2*dt); // the variable speed is also updated (note: it is private)
	speed=(pos-posOld)/dt; // less approximate than the above, but we avoid having a global posNew variable 
	return(speed); 
}

void pointMass::setSpeed(const vector2D& vel) {
	speed.set(vel); // enough for EULER METHOD
	pos=speed*dt+posOld; // NECESSARY for VERLET METHOD (we assume posOld known). 
	// no need to compute newPos
}

void pointMass::setSpeed(float vx, float vy) {
	speed.set(vx, vy); // enough for EULER METHOD
	pos=speed*dt+posOld; // NECESSARY for VERLET METHOD (we assume posOld known). 
	// no need to compute newPos
}

void pointMass::setPos(float px, float py) { // assuming the speed is unchanged (must do some tweaking in case of Verlet integration)
	pos.set(px, py);
	posOld=pos-speed*dt;
}

//------------------------------------------------------------
void pointMass::update(){	
	if (bFixed == false){
		acc=totalForce/mass; // this is the acceleration at time t
	
#ifndef VERLET_METHOD
		
		// The following equations (Euler integration) assume acceleration constant during time dt:
		speed = speed + acc*dt;
		pos = pos + speed*dt ;//+acc*dt*dt*0.5;
#else
		// The following equations are for VERLET integration with pseudo-damping:
		vector2D posNew=pos*(2-dampMotion)-posOld*(1-dampMotion)+acc*dt*dt;
		// Without damping this is just: posNew=2*posOld-1*pos+acc*dt*dt; // i.e., dampMotion=0;
		
		posOld=pos;
		pos=posNew;
		
		// NOTE: we can also estimate the speed if we want. But this may be unnecessary (call getSpeed() for that). 
		
#endif

	}
}

void pointMass::setWallLimits(float maxx, float maxy, float minx, float miny) {
	maxWall.set(maxx, maxy);
	minWall.set(minx, miny);
}

//------------------------------------------------------------
void pointMass::bounceOffWalls(){
	// NOTE: bounce is easy in case of EULER method; in case of VERLET, we need to do some hack on the positions.
	
	//Note: the walls are in (vector2D) horizontalLimits and verticalLimits 
	
#ifndef VERLET_METHOD
	
	if (pos.x > maxWall.x){
		pos.x = maxWall.x; 
		speed.x *= -1;
		bDidICollide = true;
	} else if (pos.x < minWall.x){
		pos.x = minWall.x; 
		speed.x *= -1;
		bDidICollide = true;
	}
	
	if (pos.y > maxWall.y){
		pos.y = maxWall.y; 
		speed.y *= -1;
		bDidICollide = true;
	} else if (pos.y < minWall.y){
		pos.y = minWall.y; 
		speed.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide) // damping:
		speed *=(1-dampBorder);
	
#else // THIS IS FOR VERLET METHOD:
	// we need to estimate the inverted, damped vector for bumping::
	vector2D bumpVector=getSpeed()*dt*(dampBorder-1); // assuming dampBorder<1 of course
	if (pos.x > maxWall.x){
		//posOld.x=pos.x;
		//pos.x=pos.x+bumpVector.x;
		posOld.x=maxWall.x;
		pos.x=maxWall.x+bumpVector.x;
	} else if (pos.x < minWall.x){
		posOld.x=minWall.x;
		pos.x=minWall.x+bumpVector.x;
	}
	
	if (pos.y > maxWall.y){
		posOld.y=maxWall.y;
		pos.y=maxWall.y+bumpVector.y;
	} else if (pos.y < minWall.y){
		posOld.y=minWall.y;
		pos.y=minWall.y+bumpVector.y;
	}
#endif
	
}

bool pointMass::didCollideBorders() {
    return ( pos.x > maxWall.x || pos.x < minWall.x || pos.y > maxWall.y || pos.y < minWall.y );
}

bool pointMass::didCollideDrawing(float _grayLevelThresholdCollision, ofImage& _imgBackground, float _radiusPixelCollision) {
    for (int i = 0; i < 100; i++){ // check every 10 degrees (note: mouse lines should be a little fat or it can miss points):
        int x = pos.x + _radiusPixelCollision * cos ( (1.0*i / 100) * TWO_PI);
        int y = pos.y + _radiusPixelCollision * sin ( (1.0*i / 100) * TWO_PI);
        
        // grab color at that point in the mini circle:
        float pixelBrightness = _imgBackground.getPixels().getColor(x,y).getBrightness();
        
        if (pixelBrightness < _grayLevelThresholdCollision*255) {return(true);} // stop exploring, faster}
    }
    
    return(false);
    
}
