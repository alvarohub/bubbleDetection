#ifndef SPRING_H
#define SPRING_H

// Springs between particles of the active contour:

#include "classPointMass.h"

class spring {

	public:

		spring();
		
		pointMass * massA;
		pointMass * massB;
		
		float distance;
		float springiness;	 // this is the k, springiness constant
	
		void update();
	
};


#endif
