  /* 
 * File     : receiver.h
 * Author     : Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description  : Header for receiver
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
  pthread_t thread[1];
  bool endFileReceived;
  Frame frameBuffer[MAXSEQ];A  int startWindow;
  Queue tempQueue();
  string finalMessage;

  /* Socket */
  int sockfd; // listen on sock_fd
  struct sockaddr_in adhost;

  bool inWindow(int frameNo);
  void initializeReceiver();
  void receiveFrames();
  void error(const char* message);
  void childProcessFrames();
};

#endif