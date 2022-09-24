#pragma once

#include "ofMain.h"

#include "elasticLoop.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    vector<shared_ptr<elasticLoop>> ptr_bubbleArray;
    bool fillMode;
    float internalPressure;
    int radiusCollision;
    
    // For drawing (can be an image to load or any other method for drawing, this is just an example).
    // NOTE that the COLLISION of the bubble with the lines is NOT done by comparing point coordinates, but color pixels on the framebuffer,
    // hence anything can be used there (the drawing is NOT a set of stored paths, this is an example. I can use a pic or video camera for instance).
    ofPolyline newMouseLine; // this is for adding lines with the mouse
    vector <ofPolyline> lines;
    
    // If using a video input:
    bool videoMode;
    ofVideoGrabber vidGrabber;
    int camWidth;
    int camHeight;
    
    ofImage  imgBackground;
    
    ofTrueTypeFont  franklinBook;
		
};
