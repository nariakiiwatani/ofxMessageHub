//
//  ofxMessageHub.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

#include "ofEvents.h"
#include "Converter.h"

namespace ofx {
namespace messagehub {
	
template<typename Message> class Client;
template<typename Message>
class Server
{
friend class Client<Message>;
public:
	Server() {
		ofAddListener(ofEvents().update, this, &Server::checkMessage);
	}
	virtual ~Server() {
		ofRemoveListener(ofEvents().update, this, &Server::checkMessage);
	}
	void dispatch(Message &msg) {
		ofNotifyEvent(dispatcher_, msg, this);
	}
protected:
	ofEvent<Message> dispatcher_;
	virtual void checkMessage(ofEventArgs&){};
};

template<typename Message>
class Client
{
public:
	virtual ~Client() {
		while(!servers_.empty()) {
			auto it = servers_.begin();
			if(!disconnect(*it)) {
				servers_.erase(it);
			}
		}
	}
	bool connect(std::weak_ptr<Server<Message>> server) {
		if(auto s = server.lock()) {
			ofAddListener(s->dispatcher_, this, &Client::onReceiveMessage);
			servers_.emplace_back(server);
			return true;
		}
		return false;
	}
	bool disconnect(std::weak_ptr<Server<Message>> server) {
		auto it = getIteratorOfConnectedServer(server);
		if(it != servers_.end()) {
			if(auto s = server.lock()) {
				ofRemoveListener(s->dispatcher_, this, &Client::onReceiveMessage);
			}
			servers_.erase(it);
			return true;
		}
		return false;
	}
	bool isConnected(std::weak_ptr<Server<Message>> server) {
		return getIteratorOfConnectedServer(server) != servers_.end();
	}
protected:
	virtual void onReceiveMessage(Message&)=0;
	std::vector<std::weak_ptr<Server<Message>>> servers_;
private:
	auto getIteratorOfConnectedServer(std::weak_ptr<Server<Message>> server) -> decltype(servers_.begin()) {
		if(auto s = server.lock()) {
			return std::find_if(servers_.begin(), servers_.end(), [&s](const std::weak_ptr<Server<Message>> ptr) {
				return s == ptr.lock();
			});
		}
		return servers_.end();
	}
};
	
template<typename InMessage, typename OutMessage=InMessage, typename Converter=NoConverter<InMessage>>
class Bus : public Server<OutMessage>, public Client<InMessage>
{
protected:
	void onReceiveMessage(InMessage &msg) {
		OutMessage m;
		if(conv_.convert(msg,m)) {
			this->dispatch(m);
		}
	}
	Converter conv_;
};

}}