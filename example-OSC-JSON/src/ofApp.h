#pragma once

#include "ofMain.h"
#include "ofxMessageHubOsc.h"
#include "ofxMessageHubOscIO.h"

using namespace ofx::messagehub::osc;

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
private:
	std::shared_ptr<Receiver> receiver_ = std::make_shared<Receiver>();
	std::shared_ptr<Sender> sender_ = std::make_shared<Sender>();
	std::shared_ptr<JsonRecorder> recorder_ = std::make_shared<JsonRecorder>();
	std::shared_ptr<JsonPlayer> player_ = std::make_shared<JsonPlayer>();
	std::shared_ptr<ofx::messagehub::Bus<ofxOscMessage>> bus_ = std::make_shared<ofx::messagehub::Bus<ofxOscMessage>>();
};
