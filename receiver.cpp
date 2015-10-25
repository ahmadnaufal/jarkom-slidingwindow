/* 
 * File     : receiver.cpp
 * Author     : Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description  : Receiver implementation
 */ 

#include "receiver.h"

using namespace std;

Receiver::Receiver() {
  port = 8080;
  startWindow = 0;
  endFileReceived = false;
}

Receiver::Receiver(char* _portNo) { //Ctor
  port = atoi(_portNo);
  startWindow = 0;
  endFileReceived = false;
  initializeReceiver();
}

Receiver::~Receiver() {
}

void Receiver::initializeReceiver() {
  printf("Creating socket to self in Port %s...\n", port);
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
     error("ERROR: Create socket failed.\n");

   bzero((char*) &adhost, sizeof(adhost));
   adhost.sin_family = AF_INET;
   adhost.sin_port = htons(port);
   adhost.sin_addr.s_addr = INADDR_ANY;

   endFileReceived = false;

   int res = bind(sockfd, (struct sockaddr*) &adhost, sizeof(adhost))
  if (res < 0)
     error("ERROR: Binding failed.\n");

   for (int i=0;i<MAXSEQ;i++) {
     sentAck[i] = false;
   }
}

void Receiver::receiveFrames() {
  std::thread child(&Transmitter::childProcessFrames, *this);

  while (!endFileReceived) {
    while (sentAck[startWindow]) {
      //tambahin kode buat nambahin frameBuffer[startWindow].getData() ke akhir string finalMessage ya mad
      string temp(frameBuffer[startWindow].getData());
      finalMessage += temp;
      sentAck[startWindow] = false;
      startWindow=(startWindow+1)%WINSIZE;
    }
    Frame *temp = tempQueue.del();   
    if (inWindow(temp->getNo())) {
       if (temp->getChecksum() == Checksum::checksum(serializedFrame, sizeof(serializedFrame))) {
         printf("[ACK] Package %d secure. Sending ACK %d...\n", temp->getNo(), temp->getNo());
         sendACK(ACK, temp->getNo());
         frameBuffer[temp->getNo()] = temp;
         sentAck[temp->getNo()] = true;
       } else {
         printf("[NAK] Package %d error or corrupt. Sending NAK...\n", temp->getNo());
         sendACK(NAK, temp->getNo());
       }
    } else {
       sendACK(ACK,temp->getNo());
    }
    //cek apakah ada karakter endfil e atau nggak
  }
}

void Receiver::childProcessFrames() {
   char* serializedFrame;
   struct sockaddr_in srcAddr; socklen_t srcLen;

   if (recvfrom(sockfd, serializedFrame, 20, 0, (struct sockaddr *) &srcAddr, &srcLen) < 0)
     error("ERROR: Failed to receive character from socket\n");

  tempQueue.add(new Frame(serializedFrame));  
}

void Receiver::sendACK(Byte ACKtype, Byte frameNo) {
  Ack *ack = new Ack(ACKtype, frameNo);  
  if(sendto(sockfd, ack->getSerialized(), ack->getSerializedSize(), 0,(struct sockaddr *) &srcAddr, srcLen) < 0)
     error("ERROR: Failed to send ACK/NAK.\n");
}

bool Receiver::inWindow(int frameNo) {
  return (((frameNo-startWindow)<WINSIZE && (frameNo-startWindow)>=0) || (frameNo+MAXSEQ-i)<WINSIZE && (frameNo+MAXSEQ-i)>0);
}

void Receiver::error(const char *message) {
	perror(message);
	exit(1);
}