//
//  Runner.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

template<typename Timer>
class Runner
{
public:
	void start() {
		timer_.reset();
		timer_.start();
		is_running_ = true;
	}
	void stop() {
		timer_.stop();
		is_running_ = false;
	}
	bool isRunning() const { return is_running_; }
	typename Timer::CounterType getTime() const { return timer_.get(); }
protected:
	bool is_running_=false;
	Timer timer_;
};