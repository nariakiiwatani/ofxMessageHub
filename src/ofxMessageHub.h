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
		if(auto s = server.lock()) {
			auto it = std::find_if(servers_.begin(), servers_.end(), [&s](const std::weak_ptr<Server<Message>> ptr) {
				return s == ptr.lock();
			});
			if(it != servers_.end()) {
				ofRemoveListener(s->dispatcher_, this, &Client::onReceiveMessage);
				servers_.erase(it);
				return true;
			}
		}
		return false;
	}
protected:
	virtual void onReceiveMessage(Message&)=0;
private:
	std::vector<std::weak_ptr<Server<Message>>> servers_;
};
	
template<typename InMessage, typename OutMessage=InMessage, typename Converter=NoConverter<InMessage>>
class Bus : public Server<OutMessage>, public Client<InMessage>
{
protected:
	void onReceiveMessage(InMessage &msg) {
		Converter c;
		OutMessage &&m = c.convert(msg);
		this->dispatch(m);
	}
};

}}