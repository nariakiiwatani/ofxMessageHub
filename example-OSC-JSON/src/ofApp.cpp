#include "ofApp.h"
#include "ofxMessageHubOscIO.h"

using namespace ofx::messagehub::osc;
using namespace ofx::messagehub;
//--------------------------------------------------------------
void ofApp::setup(){
	receiver_->setup(12345);
	
	bus_->connect(receiver_);
	bus_->connect(player_);

	sender_->setup("localhost", 12346);
	sender_->connect(bus_);
	
	recorder_->connect(receiver_);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case ' ': {
			if(recorder_->isRecording()) {
				recorder_->stop();
			}
			else {
				recorder_->clear();
				recorder_->start();
			}
		}	break;
		case 's': {
			recorder_->save("test.json");
		}	break;
		case 'p': {
			player_->load("test.json");
			player_->play();
		}	break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
