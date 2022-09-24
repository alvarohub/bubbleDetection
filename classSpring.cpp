#include "classSpring.h"
#include "myVectorClass.h"

//---------------------------------------------------------------------
spring::spring(){
	massA = NULL;
	massB = NULL;
}

//---------------------------------------------------------------------
void spring::update(){
	if ((massA == NULL) || (massB == NULL)){
        return;
	}
	
   // cout << massA->mass << endl;
    
	vector2D pta = massA->pos;
	vector2D ptb = massB->pos;
	
	float theirDistance = (pta - ptb).length();
    
	float springForce = (springiness * (distance - theirDistance));
	vector2D frcToAdd = (pta-ptb).normalize() * springForce;
	
    
	massA->addForce(frcToAdd);
	massB->addForce(-frcToAdd);
	//massA->totalForce.x+=frcToAdd.x;massA->totalForce.y+=frcToAdd.y;
	//massB->totalForce.x-=frcToAdd.x;massB->totalForce.y-=frcToAdd.y;
}
