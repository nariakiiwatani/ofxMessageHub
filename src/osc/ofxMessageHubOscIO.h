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
								  [](Buffer &buffer, ofJson &json){
									  Json2Osc converter;
									  ofxOscMessage msg = converter.convert(json["data"]);
									  buffer.insert(std::make_pair(json["time"], msg));
									  return buffer;
								  });
	}
};

class JsonRecorder : public BufferRecorder<TimeCounter, ofxOscMessage>
{
public:
	void save(const std::string &filepath) const {
		ofJson json = std::accumulate(std::begin(buffer_), std::end(buffer_),
									  ofJson(),
									  [](ofJson &json, const Buffer::value_type &buffer) {
										  Osc2Json converter;
										  ofJson single;
										  single["time"] = buffer.first;
										  single["data"] = converter.convert(buffer.second);
										  json.push_back(single);
										  return json;
									  });
		ofBufferToFile(filepath, json.dump(4), false);
	}
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
		Osc2Json encoder;
		ofJson json;
		json["time"] = time;
		json["data"] = encoder.convert(msg);
		if(!is_first_record_) {
			file_ << "," << endl;
		}
		is_first_record_ = false;
		file_ << json.dump(4);
	}
protected:
	bool is_first_record_ = false;
};

}}}