//
//  ofxMessageHubOsc.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

#include "ofxOsc.h"
#include "ofxMessageHub.h"

namespace ofx { namespace messagehub {
namespace osc {

class Receiver : public Server<ofxOscMessage>, public ofxOscReceiver
{
protected:
	void checkMessage(ofEventArgs&) {
		while(hasWaitingMessages()) {
			ofxOscMessage msg;
			getNextMessage(msg);
			dispatch(msg);
		}
	}
};

class Sender : public Client<ofxOscMessage>, public ofxOscSender
{
protected:
	void onReceiveMessage(ofxOscMessage &msg) {
		sendMessage(msg, false);
	}
};
	
using Server = ofx::messagehub::Server<ofxOscMessage>;
using Client = ofx::messagehub::Client<ofxOscMessage>;
using Bus = ofx::messagehub::Bus<ofxOscMessage>;

}}}