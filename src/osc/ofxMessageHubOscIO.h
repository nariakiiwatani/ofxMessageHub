//
//  ofxMessageHubOscIO.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

#include "ofxMessageHubIO.h"
#include "ofxMessageHubOscToJson.h"
#include "Counter.h"

namespace ofx { namespace messagehub {
namespace osc {

class JsonPlayer : public BufferPlayer<TimeCounter, ofxOscMessage>
{
public:
	void load(const std::string &filepath) {
		ofJson json = ofJson::parse(ofBufferFromFile(filepath));
		buffer_ = std::accumulate(std::begin(json), std::end(json),
								  Buffer(),
								  [this](Buffer &buffer, ofJson &json){
									  ofxOscMessage msg;
									  if(conv_.convert(json["data"], msg)) {
										  buffer.insert(std::make_pair(json["time"], msg));
									  }
									  return buffer;
								  });
	}
protected:
	Json2Osc conv_;
};

class JsonRecorder : public BufferRecorder<TimeCounter, ofxOscMessage>
{
public:
	void save(const std::string &filepath) const {
		ofJson json = std::accumulate(std::begin(buffer_), std::end(buffer_),
									  ofJson(),
									  [this](ofJson &json, const Buffer::value_type &buffer) {
										  ofJson data;
										  if(conv_.convert(buffer.second, data)) {
											  ofJson single;
											  single["time"] = buffer.first;
											  single["data"] = data;
											  json.push_back(single);
										  }
										  return json;
									  });
		ofBufferToFile(filepath, json.dump(4), false);
	}
protected:
	Osc2Json conv_;
};

class JsonStreamRecorder : public StreamRecorder<TimeCounter, ofxOscMessage>
{
public:
	void start(const std::string &filepath) {
		if(this->open(filepath, ofFile::WriteOnly, false)) {
			file_ << "[" << endl;
			is_first_record_ = true;
			StreamRecorder::start();
		}
	}
	void stop() {
		file_ << endl << "]";
		StreamRecorder::stop();
		file_.close();
	}
	void record(const TimeCounter::CounterType &time, const ofxOscMessage &msg) {
		ofJson data;
		if(conv_.convert(msg, data)) {
			ofJson json;
			json["data"] = data;
			json["time"] = time;
			if(!is_first_record_) {
				file_ << "," << endl;
			}
			is_first_record_ = false;
			file_ << json.dump(4);
		}
	}
protected:
	bool is_first_record_ = false;
	Osc2Json conv_;
};

using Recorder = JsonRecorder;
using Player = JsonPlayer;

}}}