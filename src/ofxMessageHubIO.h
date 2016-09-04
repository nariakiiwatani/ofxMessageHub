//
//  ofxMessageHubIO.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

#include "ofxMessageHub.h"
#include "Runner.h"

namespace ofx { namespace messagehub {

template<typename Timer, typename Message>
class Player : public Server<Message>
{
public:
	virtual void play() {
		timer_.start();
		playhead_ = timer_.getTime();
		is_playing_ = true;
	}
	virtual void stop() {
		timer_.stop();
		is_playing_ = false;
	}
	bool isPlaying() const { return is_playing_; }
protected:
	void checkMessage(ofEventArgs&) {
		if(isPlaying()) {
			typename Timer::CounterType current_time = timer_.getTime();
			std::deque<Message> que = getMessagesInRange(playhead_, current_time);
			std::for_each(std::begin(que), std::end(que), std::bind(&Player::dispatch, this, std::placeholders::_1));
			playhead_ = current_time;
		}
	}
	virtual std::deque<Message> getMessagesInRange(const typename Timer::CounterType &lower, const typename Timer::CounterType &upper)=0;
	
	Runner<Timer> timer_;
	typename Timer::CounterType playhead_;
	bool is_playing_ = false;
};

template<typename Timer, typename Message>
class BufferPlayer : public Player<Timer, Message>
{
public:
	using Buffer = std::multimap<typename Timer::CounterType, Message>;
	void clear() { buffer_.clear(); }
protected:
	std::deque<Message> getMessagesInRange(const typename Timer::CounterType &lower, const typename Timer::CounterType &upper) {
		return std::accumulate(buffer_.lower_bound(lower), buffer_.upper_bound(upper), std::deque<Message>(), [](std::deque<Message> &que, const typename Buffer::value_type &value) {
			que.push_back(value.second);
			return que;
		});
	}
	Buffer buffer_;
};

template<typename Timer, typename Message>
class StreamPlayer : public Player<Timer, Message>
{
protected:
	ofFile file_;
	bool open(const std::filesystem::path &path, ofFile::Mode mode, bool binary) {
		return file_.open(path, mode, binary);
	}
	void close() {
		file_.close();
	}
};

template<typename Timer, typename Message>
class Recorder : public Client<Message>
{
public:
	virtual void start() {
		timer_.start();
		is_recording_ = true;
	}
	virtual void stop() {
		timer_.stop();
		is_recording_ = false;
	}
	bool isRecording() const { return is_recording_; }
	virtual void record(const typename Timer::CounterType &time, const Message &msg)=0;
protected:
	void onReceiveMessage(Message &msg) {
		if(isRecording()) {
			record(timer_.getTime(), msg);
		}
	}
	Runner<Timer> timer_;
	bool is_recording_=false;
};

template<typename Timer, typename Message>
class BufferRecorder : public Recorder<Timer, Message>
{
public:
	using Buffer = std::multimap<typename Timer::CounterType, Message>;
	void record(const typename Timer::CounterType &time, const Message &msg) {
		buffer_.insert(std::make_pair(time, msg));
	}
	void clear() { buffer_.clear(); }
protected:
	Buffer buffer_;
};

template<typename Timer, typename Message>
class StreamRecorder : public Recorder<Timer, Message>
{
protected:
	ofFile file_;
	bool open(const std::filesystem::path &path, ofFile::Mode mode, bool binary) {
		return file_.open(path, mode, binary);
	}
	void close() {
		file_.close();
	}
};

}}