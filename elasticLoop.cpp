/*
 *  elasticLoop.cpp
 *  laserBlobPure
 *
 *  Created by CASSINELLI ALVARO on 5/20/11.
 *  Copyright 2011 TOKYO UNIVERSITY. All rights reserved.
 *
 */


#include "elasticLoop.h"

elasticLoop::elasticLoop() {
}

void elasticLoop::init(float _posX, float _posY, float _radius) {
    
    
    // Initial conditions for the loop masses:
    for (int i = 0; i < MAX_NUM_MASSES; i++){
        float x = _posX + _radius * cos ( (1.0*i / MAX_NUM_MASSES) * TWO_PI);
        float y = _posY + _radius * sin ( (1.0*i / MAX_NUM_MASSES) * TWO_PI);
        massesLoop[i].setInitialCondition(x,y ,0,0);
        massesLoop[i].setIntegrationStep(0.1);
        massesLoop[i].mass=0.6;
        
        // The following should be a method of "elasticString":
        massesLoop[i].setWallLimits(ofGetWidth(), ofGetHeight(), 0, 0);
    }
    
    // Springs for the loop:
    for (int i = 0; i<MAX_NUM_MASSES; i++){
        loopSpringArray[i].distance		= 1.0;
        loopSpringArray[i].springiness	= 1.2;
        loopSpringArray[i].massA = & (massesLoop[i  ]);
        //cout <<  loopSpringArray[i].massA->mass << endl;
        loopSpringArray[i].massB = & (massesLoop[(i+1) % MAX_NUM_MASSES]);
    }
    
    grayLevelThresholdCollision = 0.5;
    radiusPixelCollision = 10;
    forcePressure = 1.0;
    
}


void elasticLoop::update(ofImage& imgBackground) {
    
    // 1) Reset all forces:
    for (int i = 0; i < MAX_NUM_MASSES; i++){
        massesLoop[i].resetForce();
    }
    
    
    // 2) Add all forces from inter-particle springs:
    for (int i = 0; i < MAX_NUM_MASSES; i++){ // if putting -1, the loop is broken
       // cout <<  loopSpringArray[i].massA->mass << endl;
        loopSpringArray[i].update();// this add forces to the particles
    }
    
    // 3) Add internal pressure force if there is no drawing nearby the particle, otherwise NO FORCE + zero speed, or INTERNAL force!
    for (int i = 0; i < MAX_NUM_MASSES; i++){
        bool collideDrawing =  massesLoop[i].didCollideDrawing(grayLevelThresholdCollision, imgBackground, radiusPixelCollision);
        int signForce = (collideDrawing ? -0.6 : 1);
        
        //.. and/or also reset the speed:
        if (collideDrawing) massesLoop[i].setSpeed(massesLoop[i].getSpeed()*0.5);
       
            // First, compute the normal to the particle, pointing outwards (this will be approximated by taking
            // the 90 deg rotated difference note between contiguous particles positions (this gives rise to oscillations), but the one before and the one after:
            vector2D diff;
            diff.set(massesLoop[(i+1)%MAX_NUM_MASSES].pos-massesLoop[(i+MAX_NUM_MASSES-1)%MAX_NUM_MASSES].pos);
            // normalize and rotate 90 deg conterclockwise:
            vector2D hairvector=diff.getPerpendicular(CCW);
        massesLoop[i].addForce( hairvector * forcePressure * signForce);
        }
 
    
    
    // 3) Update kinematics:
    for (int i = 0; i < MAX_NUM_MASSES; i++){
        
#ifndef VERLET_METHOD
        massesLoop[i].addDampingForce(); // only in case of Euler method
#endif
        massesLoop[i].update(); // unconstrained
        massesLoop[i].bounceOffWalls(); // constrain on screen border
    }
    
    
}



void elasticLoop::draw(bool _filled) {
    ofNoFill();
    ofSetColor(255,0,0); // last value is alpha channel (0-255)
    ofBeginShape();
    for (int i = 0; i < MAX_NUM_MASSES; i++){
        ofVertex(massesLoop[i].pos.x,massesLoop[i].pos.y);
    }
    ofVertex(massesLoop[0].pos.x,massesLoop[0].pos.y);
    ofEndShape();
    
    // little circles in each point just for clarity:
//    ofNoFill();
//    ofSetColor(255,190,0);
//    for (int i = 0; i < MAX_NUM_MASSES; i++){
//        ofDrawEllipse(massesLoop[i].pos.x, massesLoop[i].pos.y, 5, 5);
//    }
}

void elasticLoop::setInternalPressure(float _pressure) {
    forcePressure = _pressure;
}

void elasticLoop::setRadiusCollision(float _radius) {
    radiusPixelCollision = _radius;
}

