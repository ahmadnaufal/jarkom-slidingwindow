all: transmitter receiver

transmitter: main-transmitter.cpp
	g++ main-transmitter.cpp transmitter.cpp frame.cpp ack.cpp queue.cpp checksum.cpp window.cpp -lpthread -std=gnu++11 -o transmitter

receiver: main-receiver.cpp
	g++ main-receiver.cpp receiver.cpp window.cpp frame.cpp ack.cpp queue.cpp checksum.cpp -lpthread -std=gnu++11 -o receiver

clean:
	$(RM) transmitter receiver