/*
 *  elasticLoop.h
 *  laserBlobPure
 *
 *  Created by CASSINELLI ALVARO on 5/20/11.
 *  Copyright 2011 TOKYO UNIVERSITY. All rights reserved.
 *
 */

#ifndef ELASTIC_LOOP
#define ELASTIC_LOOP

#include "classPointMass.h"
#include "classSpring.h"

#define MAX_NUM_MASSES  150

class elasticLoop {
	
public:
	
	elasticLoop();
    
    void init(float _posX, float _posY, float _radius);
	
	void update(ofImage& imgBackground);
    
    void draw(bool _filled);
    
    void setInternalPressure(float _pressure);
    
    void setRadiusCollision(float _radius);

	pointMass massesLoop[MAX_NUM_MASSES];
	spring loopSpringArray[MAX_NUM_MASSES];
	
    float forcePressure;
    float grayLevelThresholdCollision; // from 0 to 1 (will be scaled to 0-255)
    float radiusPixelCollision;
	
};



#endif
