//
//  Counter.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

#include "ofEvents.h"
#include "ofAppRunner.h"

template<typename T>
class Counter
{
public:
	using CounterType = T;
	virtual void reset(){}
	virtual void count()=0;
	virtual T get() const=0;
};

template<typename T>
class CountEveryFrame : public Counter<T>
{
public:
	void start() {
		if(!is_enabled_) {
			ofAddListener(ofEvents().update, this, &CountEveryFrame::update);
			is_enabled_ = true;
		}
	}
	void stop() {
		if(is_enabled_) {
			ofRemoveListener(ofEvents().update, this, &CountEveryFrame::update);
			is_enabled_ = false;
		}
	}
	void update(ofEventArgs&){ this->count(); }
protected:
	bool is_enabled_=false;
};

class TimeCounter : public CountEveryFrame<float>
{
public:
	void reset() { time_ = 0; }
	void count() { time_ += ofGetLastFrameTime(); }
	float get() const { return time_; }
private:
	float time_ = 0;
};

class FrameCounter : public CountEveryFrame<int>
{
public:
	void reset() { frame_ = 0; }
	void count() { ++frame_; }
	int get() const { return frame_; }
private:
	int frame_ = 0;
};