all: transmitter receiver

supp = ack.cpp checksum.cpp frame.cpp queue.cpp window.cpp

transmitter: transmitter.cpp
	g++ -o transmitter main-transmitter.cpp transmitter.cpp $(supp) -lpthread -std=gnu++11

receiver: receiver.cpp
	g++ -o receiver main-receiver.cpp receiver.cpp $(supp) -lpthread -std=gnu++11

clean:
	$(RM) transmitter receiver
