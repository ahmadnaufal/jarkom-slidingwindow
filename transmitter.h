#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "slidingwindow.h"
#include "frame.h"
#include "queue.h"
#include "ack.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <sys/socket.h>
#include <pthread.h>

using namespace std;

class Transmitter {
public:
	Transmitter();
	Transmitter(char* IP, char* portNo, char* file);
	~Transmitter();

private:
	char *receiverIP;
	FILE *tFile;
	int port;
	int sockfd;
	struct hostent *server;
	struct sockaddr_in receiverAddr;
	bool isSocketOpen;	
	Frame *frameStorage;
	int fcount;
	//Queue slidingWindow(WINSIZE);

	void initializeTransmitter();
	void readFile();
	void sendFrames();
	void* childProcessACK(void *threadid);
	void error(const char *message);
};

#endif