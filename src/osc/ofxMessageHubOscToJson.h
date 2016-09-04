//
//  ofxMessageHubOscToJson.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

#include "Converter.h"
#include "ofxOscMessage.h"

namespace ofx { namespace messagehub {
namespace osc {

class Osc2Json : public Converter<ofxOscMessage, ofJson>
{
public:
	ofJson convert(const ofxOscMessage &msg) const {
		// this does a kind of re-encoding(decode and encode) osc message
		// so we should see osc::ReceivedMessage instead.
		ofJson json;
		json["address"] = msg.getAddress();
		ofJson &args = json["args"] = ofJson();
		std::string tags;
		for(int i = 0, num = msg.getNumArgs(); i < num; ++i) {
			char tag = msg.getArgType(i);
			switch(tag) {
				case OFXOSC_TYPE_INT32:				args += msg.getArgAsInt32(i);	break;
				case OFXOSC_TYPE_INT64:				args += msg.getArgAsInt64(i);	break;
				case OFXOSC_TYPE_FLOAT:				args += msg.getArgAsFloat(i);	break;
				case OFXOSC_TYPE_DOUBLE:			args += msg.getArgAsDouble(i);	break;
				case OFXOSC_TYPE_STRING:			args += msg.getArgAsString(i);	break;
				case OFXOSC_TYPE_SYMBOL:			args += msg.getArgAsSymbol(i);	break;
				case OFXOSC_TYPE_CHAR:				args += msg.getArgAsChar(i);	break;
				case OFXOSC_TYPE_MIDI_MESSAGE:		args += msg.getArgAsMidiMessage(i);	break;
				case OFXOSC_TYPE_TIMETAG:			args += msg.getArgAsTimetag(i);	break;
				case OFXOSC_TYPE_BLOB:				args += msg.getArgAsBlob(i);	break;
				case OFXOSC_TYPE_RGBA_COLOR:		args += msg.getArgAsRgbaColor(i);	break;
				// these are with no argument
				case OFXOSC_TYPE_TRUE:
				case OFXOSC_TYPE_FALSE:
				case OFXOSC_TYPE_TRIGGER:
				case OFXOSC_TYPE_NONE:
					break;
				// I don't know how to handle these tag.
				case OFXOSC_TYPE_BUNDLE:
				case OFXOSC_TYPE_INDEXOUTOFBOUNDS:
				default:
					break;
			}
			tags += tag;
		}
		json["tags"] = tags;
		return json;
	}
};
	
class Json2Osc : public Converter<ofJson, ofxOscMessage>
{
public:
	ofxOscMessage convert(const ofJson &json) const {
		ofxOscMessage msg;
		msg.setAddress(json["address"]);
		const std::string &tags = json["tags"];
		const ofJson &args = json["args"];
		std::size_t arg_index = 0;
		for(auto t : tags) {
			switch(t) {
				case OFXOSC_TYPE_INT32:				msg.addInt32Arg(args[arg_index++]);	break;
				case OFXOSC_TYPE_INT64:				msg.addInt64Arg(args[arg_index++]);	break;
				case OFXOSC_TYPE_FLOAT:				msg.addFloatArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_DOUBLE:			msg.addDoubleArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_STRING:			msg.addStringArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_SYMBOL:			msg.addSymbolArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_CHAR:				msg.addCharArg(args[arg_index++].get<char>());	break;
				case OFXOSC_TYPE_MIDI_MESSAGE:		msg.addMidiMessageArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_TIMETAG:			msg.addTimetagArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_BLOB:				msg.addBlobArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_RGBA_COLOR:		msg.addRgbaColorArg(args[arg_index++]);	break;
				case OFXOSC_TYPE_TRUE:				msg.addBoolArg(true);	break;
				case OFXOSC_TYPE_FALSE:				msg.addBoolArg(false);	break;
				case OFXOSC_TYPE_TRIGGER:			msg.addTriggerArg();	break;
				// I don't know how to handle these tags
				case OFXOSC_TYPE_NONE:
				case OFXOSC_TYPE_BUNDLE:
				case OFXOSC_TYPE_INDEXOUTOFBOUNDS:
				default:
					msg.addTriggerArg();
					break;
			}
		}
		return msg;
	}
};

}}}