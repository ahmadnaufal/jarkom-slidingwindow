/* 
 * File 		: receiver.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Header for receiver
 */ 

#ifndef _RECEIVER_H_ 
#define _RECEIVER_H_ 

#include "queue.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "slidingwindow.h"

#define MIN_UPPERLIMIT 5
#define MAX_LOWERLIMIT 2

#define bzero(p, size) (void)memset((p), 0 , (size))
/* Delay to adjust speed of consuming buffer, in milliseconds */
#define DELAY 500

using namespace std;

class Receiver {
public:
	Receiver();
	Receiver(char* _portNo); //Ctor
	~Receiver();

private:
	int port;
	Queue *frameEater;
	pthread_t thread[1];

	Byte sent_xonxoff;
	unsigned send_xon = 0, send_xoff = 0;
	int endFileReceived;

	/* Socket */
	int sockfd; // listen on sock_fd
	struct sockaddr_in adhost;

	void initializeReceiver();
	void receiveFrames();
	static Byte *rcvframe();
	static Byte *q_get(QTYPE *, Byte *);
	void *childRProcess(void * threadid);
	void error(const char* message);
	void sendingAck();
	
};

#endif