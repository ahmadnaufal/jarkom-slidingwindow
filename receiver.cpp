/* 
 * File     : receiver.cpp
 * Author     : Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description  : Receiver implementation
 */ 

#include "receiver.h"
#include <stdio.h>

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
  startChildProcess();
}

Receiver::~Receiver() {
}

void Receiver::initializeReceiver() {
  printf("Creating socket to self in Port %d...\n", port);
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
     error("ERROR: Create socket failed.\n");

   bzero((char*) &adhost, sizeof(adhost));
   adhost.sin_family = AF_INET;
   adhost.sin_port = htons(port);
   adhost.sin_addr.s_addr = INADDR_ANY;

   endFileReceived = false;

   int res = bind(sockfd, (struct sockaddr*) &adhost, sizeof(adhost));
  if (res < 0)
     error("ERROR: Binding failed.\n");

   for (int i=0;i<MAXSEQ;i++) {
     sentAck[i] = false;
   }
}

void Receiver::startChildProcess() {
  std::thread processThread(&Receiver::receiveFrames, this);
  std::thread processThreadTwo(&Receiver::childProcessFrames, this);
  //std::thread mainThread(&Transmitter::sendFrames, this);

    // more stuff
    processThread.join();
    processThreadTwo.join();
    //mainThread.join();
}

void Receiver::receiveFrames() {;
  while (!endFileReceived) {
    while (sentAck[startWindow]) {
      //tambahin kode buat nambahin frameBuffer[startWindow].getData() ke akhir string finalMessage ya mad
      string temp(frameBuffer[startWindow].getData());
      finalMessage += temp;
      sentAck[startWindow] = false;
      startWindow=(startWindow+1)%MAXSEQ;
    }
    if (!tempQueue.isEmpty()) {
      Frame temp = tempQueue.del();   
      if (inWindow(temp.getNo())) {
       if (temp.getChecksum() == Checksum::createChecksum(temp.getSerialized(), temp.getSize()-2)) {
         printf("[ACK] Package %d secure. Sending ACK %d...\n", temp.getNo(), temp.getNo());
         sendACK(ACK, temp.getNo());
         frameBuffer[temp.getNo()] = temp;
         sentAck[temp.getNo()] = true;
       } else {
         printf("[NAK] Package %d error or corrupt. Sending NAK...\n", temp.getNo());
         sendACK(NAK, temp.getNo());
       }
      } else {
       sendACK(ACK,temp.getNo());
      }
      //cek apakah ada karakter endfil e atau nggak
    }
  }
}

void Receiver::childProcessFrames() {
    while (true) {
   char serializedFrame[DATAMAX+6];
    
      socklen_t srcLen = sizeof(srcAddr);
      int rs = recvfrom(sockfd, serializedFrame, DATAMAX+6, 0, (struct sockaddr *) &srcAddr, &srcLen);
      Frame temp(serializedFrame);
      printf("Receiving packet no-%d, checking...\n", temp.getNo());
      
      if (rs == -1)
        error("ERROR: Failed to receive character from socket\n");

      tempQueue.add(temp);
    }  
}

void Receiver::sendACK(Byte ACKtype, Byte frameNo) {
  Ack *ack = new Ack(ACKtype, frameNo);
  if(sendto(sockfd, ack->getSerialized(), sizeof(ack->getSerialized())-1, 0,(struct sockaddr *) &srcAddr, sizeof(srcAddr)) == -1)
     error("ERROR: Failed to send ACK/NAK.\n");
}

bool Receiver::inWindow(int frameNo) {
  return (((frameNo-startWindow)<WINSIZE && (frameNo-startWindow)>=0) || (frameNo+MAXSEQ-startWindow)<WINSIZE && (frameNo+MAXSEQ-startWindow)>0);
}

void Receiver::error(const char *message) {
	perror(message);
	exit(1);
}