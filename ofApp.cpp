#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    vidGrabber.setDeviceID(1);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(ofGetWidth(), ofGetHeight());// camWidth, camHeight);

    imgBackground.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_GRAYSCALE); // OF_IMAGE_COLOR or OF_IMAGE_COLOR_ALPHA depending on your needs.
    
    videoMode = false;
    
    ptr_bubbleArray.clear(); // ATTENTION!!! this is an array of POINTERS. So here I delete the pointer vector. However, if the pointed objects are not properly deleted. However,
    // using smart pointers there is no need to proceed with every delete (shared_ptr), as objects that are not pointed by anything are automatically deleted. Check here: https://en.wikipedia.org/wiki/Smart_pointer
    
    radiusCollision = 10;
    internalPressure = 30;
    fillMode = true;
    
    franklinBook.load("frabk.ttf", 18);
}

//--------------------------------------------------------------
void ofApp::update(){

    
    // If using video input, show the video feed as background:
    vidGrabber.update();
    
    // Grab all the pixels
    
    // update all the bubbles, passing the screen-captuder image so they can check "collision" with drawing:
    // NOTE: it would be better to use a VFO object here instead of grabbing the screen in the draw function...
    for (auto ptr_bubble : ptr_bubbleArray) {
        ptr_bubble->update(imgBackground);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // 1) Draw the background (video or static image):
    ofSetHexColor(0xffffff);
    //if(vidGrabber.isFrameNew())
    if (videoMode) vidGrabber.draw(0, 0);
    
    // 2) Draw the mouse lines (if we want):
    ofNoFill();
    ofSetColor(0,0,0);
    ofSetLineWidth(3.0);
    for (auto & line : lines) line.draw();
    // and the one being drawn:
    newMouseLine.draw();
      
    // 3) grab the pixels in the framebuffer (video background + lines from the mouse eventually):
    imgBackground.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
    
    
    // Finally, draw the bubbles:
    for (auto ptr_bubble : ptr_bubbleArray) ptr_bubble->draw(fillMode);
    
    
    /*
    ofSetColor(0xffffff);
    
    for (int i = 0; i < particles.size(); i++){
        particles[i].draw();
    }
    
    for (int i = 0; i < springs.size(); i++){
        springs[i].draw();
    }
     */
    
     
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){;
    
    switch (key){
            
        case ' ':
            
            // Create a new "bubble" in the present position ad add to the bubble vector:
            ptr_bubbleArray.push_back(make_shared<elasticLoop>());
            ptr_bubbleArray.back()->init(mouseX, mouseY, 10);
            ptr_bubbleArray.back()->setInternalPressure(internalPressure);
            
            break;
            
        case OF_KEY_RETURN: // toggle video input:
            videoMode = !videoMode;
            break;
        
        case OF_KEY_LEFT:
            internalPressure/=1.5;
            for (auto ptr_bubble : ptr_bubbleArray) ptr_bubble-> setInternalPressure(internalPressure);
            break;
            
        case OF_KEY_RIGHT:
            internalPressure*=1.5;
            for (auto ptr_bubble : ptr_bubbleArray) ptr_bubble-> setInternalPressure(internalPressure);
            break;
            
        case OF_KEY_UP:
            radiusCollision++;
            for (auto ptr_bubble : ptr_bubbleArray) ptr_bubble->setRadiusCollision(radiusCollision);
            break;
            
        case OF_KEY_DOWN:
            radiusCollision--;
            for (auto ptr_bubble : ptr_bubbleArray) ptr_bubble->setRadiusCollision(radiusCollision);
            break;
            
        case 'f':
            fillMode=!fillMode;
            break;
            
        case OF_KEY_DEL: // delete last bubble
            if (ptr_bubbleArray.size()>0) ptr_bubbleArray.pop_back();
            break;
            
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}


//--------------------------------------------------------------
// MOUSE PRESSED: start drawing a line:
void ofApp::mousePressed(int x, int y, int button) {
    newMouseLine.clear();
    newMouseLine.addVertex(x,y);
//    ofVec2f conv = 1.0*(ofVec2f(x,y) - centerScreen)/scaleFactor;
//    newMouseLine.addVertex(conv.x, conv.y);
}

//--------------------------------------------------------------
// MOUSE DRAGGED: continue drawing:
void ofApp::mouseDragged(int x, int y, int button) {
    newMouseLine.addVertex(x,y);
//    ofVec2f conv = 1.0*(ofVec2f(x,y) - centerScreen)/scaleFactor;
//    newMouseLine.addVertex(conv.x, conv.y);
}


//--------------------------------------------------------------
// MOUSE RELEASED: end of line, and save it for drawing: 
void ofApp::mouseReleased(int x, int y, int button) {
    newMouseLine.simplify();
    lines.push_back(newMouseLine);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    //oneLoop.anchorMass.pos.set(mouseX, mouseY);
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
