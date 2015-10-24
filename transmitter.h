#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "slidingwindow.h"
#include "Frame.h"
#include "Queue.h"
#include <cstring>

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

	void initialize();
	void readFile();
	void sendFrames();
	void childProcessACK(void *threadid);
	void error(const char *message);
};

#endif