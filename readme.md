# ofxMessageHub

Routing, Recording and Playing any kind of messages.  

## How to use

Probably you want to use the addon to Route, Record or Play __ofxOscMessage__s in most cases.  
If so, See [for ofxOscMessage](#forosc)  



### Server and Client

```
// client1 gets messages from both servers
// client2 gets only from server2
using namespace ofx::messagehub;
class Message;	// any MessageType

std::shared_ptr<Server<Message>> server1, server2;
std::shared_ptr<Client<Message>> client1, client2;
client1->connect(server1);
client1->connect(server2);
client2->connect(server2);
```

### Bus

```
// bus works like a server group
// client receives from all servers through the bus
using namespace ofx::messagehub;
class Message;	// any MessageType

std::shared_ptr<Server<Message>> server1, server2, server3;
std::shared_ptr<Client<Message>> client;
std::shared_ptr<Bus<Message>> bus;
bus->connect(server1);
bus->connect(server2);
bus->connect(server3);
client->connect(sub);

```

### Player

```
using namespace ofx::messagehub;
class Message;	// any MessageType

class MyPlayer : public Player<TimeCounter, Message>
{
protected:
	std::deque<Message> getMessagesInRange(const typename Timer::CounterType &lower, const typename Timer::CounterType &upper) {
		std::deque<Message> ret;
		// let's make deque made from Messages in range [lower,upper)
		return ret;
	}
};

```

#### BufferPlayer

If you want to store Messages in Buffer, you should use BufferPlayer.  
To use it, declare your class extends BufferPlayer and set something to `buffer_`.  
If the messages are in a file, it's good way to implement `load()` function in your BufferPlayer class.

### Recorder

```
using namespace ofx::messagehub;
class Message;	// any MessageType

class MyRecorder : public Recorder<TimeCounter, Message>
{
public:
	void record(const typename Timer::CounterType &time, const Message &msg) {
		// store time and message wherever you want to.
	}
};
```

#### BufferRecorder

In BufferRecorder, recorded messages are automatically stored in `buffer_`.  
You may want to implement something like `save()` function to save the buffer to a file.  


## <a name ="forosc"> for ofxOscMessage </a>

This addon includes some classes for ofxOsc for instant use.(and  maybe that's enough)  

- Receiver : Receives messages sent to binded port.
- Sender : Sends messages to certain host/port.
- JsonRecorder : Records received ofxOscMessage(s) to a file in JSON format.
- JsonPlayer : Playbacks recorded JSON file as ofxOscMessage(s).
- Bus : Relays messages.

