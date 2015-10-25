/* 
 * File 		: transmitter.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Header of transmitter
 */ 

#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "ack.h"
#include "frame.h"
#include "queue.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

class Transmitter {
public:
	Transmitter();
	Transmitter(char* IP, char* portNo, char* file);
	~Transmitter();

private:
	// transmitter execution parameters
	char *receiverIP;
	FILE *tFile;
	int port;

	// socket attribute
	static int sockfd;
	struct hostent *server;
	static struct sockaddr_in receiverAddr;
	static bool isSocketOpen;	

	Frame *frameStorage;
	int fcount;
	pthread_t thread[1];
	static Queue window;
	static int *timeout;
	static bool *isAck;
	//Queue slidingWindow(WINSIZE);

	void initializeTransmitter();
	void readFile();
	void sendFrames();
	static void* childProcessACK(void *threadid);
	static void error(const char *message);
	static void initTimeoutAck();
};

#endif