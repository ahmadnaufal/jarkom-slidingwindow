/* 
 * File 		: transmitter.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Transmitter implementation
 */ 


#include "transmitter.h"
#include <iostream>

using namespace std;

Transmitter::Transmitter() { //Ctor

}

Transmitter::Transmitter(char* IP, char* portNo, char* file) { //Ctor with param
	receiverIP = IP;
	frameStorage = new Frame[50];
	port = atoi(portNo);

	tFile = fopen(file, "r");
	if (tFile == NULL) 
		error("ERROR: File text not Found.\n");

	initializeTransmitter();
	readFile();
	startChildProcess();
}

Transmitter::~Transmitter() { //Dtor
	
}

void Transmitter::initializeTransmitter() {
	if ((server = gethostbyname(receiverIP)) == NULL)
		error("ERROR: Receiver Address is Unknown or Wrong.\n");
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		error("ERROR: Create socket failed.\n");
	isSocketOpen = true;

	memset(&receiverAddr, 0, sizeof(receiverAddr));
	receiverAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&receiverAddr.sin_addr.s_addr, server->h_length);
	receiverAddr.sin_port = htons(port);
	printf("Socket has been created at %s:%d\n", receiverIP, port);
}

void Transmitter::readFile() {
	fcount = 0;
	int i = 0;

	char c;
	char* tempdata = new char[DATAMAX+1];

	do {
		c = fgetc(tFile);
		
		if (i==DATAMAX) {
			tempdata[i] = '\0';
			i=0;
			Frame temp(fcount%MAXSEQ,tempdata);
			frameStorage[fcount++] = temp; 		
		}

		if (c==EOF) {
			tempdata[i] = Endfile;
			tempdata[i+1] = '\0';
			Frame temp(fcount%MAXSEQ,tempdata);
			frameStorage[fcount] = temp; 
		} else {
			tempdata[i++] = c;
		}
	} while (c!=EOF);
}

void Transmitter::startChildProcess() {

	std::thread processThread(&Transmitter::childProcessACK, this);
	std::thread processThreadTwo(&Transmitter::startParentProcess, this);
	//std::thread mainThread(&Transmitter::sendFrames, this);

    // more stuff
    processThread.join();
    processThreadTwo.join();
    //mainThread.join();
}

void Transmitter::startParentProcess() {
	sendFrames();
}

void Transmitter::sendFrames() {
	//if (pthread_create(&thread[0], NULL, &childProcessACK, 0) != 0) 
	//	error("ERROR: Failed to create thread for child. Please free some space.\n");
	
	//std::thread child(&Transmitter::childProcessACK, this);
	int i = 0; //number of frame sent
	int j = 0; //number of frame put to the window
	int timeCount = 0;
	received = 0;

	//Initialize the window
	while (!window.getFrameBuffer().isFull()) {
		if(j<=fcount) {
			window.addFrame(frameStorage[j], timeCount);
			timeCount += 1;
			j++;
		} else {
			break;
		}
	}

	//Sending the frame
	while (i<=fcount || received<fcount) {
		for(int x=0; x<window.getFrameBuffer().getCount(); x++) {
			int a = (x+window.getFrameBuffer().getHead())%WINSIZE; // Ini ga efektif banget maaf ya
			if(window.getTimeOut(a) == 0 && !window.getIsAck(a)) {
				if (sendto(sockfd, window.getFrameBuffer().getElement(a).getSerialized(), sizeof(window.getFrameBuffer().getElement(a).getSerialized()), 0, (const struct sockaddr *) &receiverAddr, sizeof(receiverAddr)) == -1)
					error("ERROR: sendto() sent buffer with size more than expected.\n");
				printf("Sending frame no. %d: %s\n", window.getFrameBuffer().getElement(a).getNo(), window.getFrameBuffer().getElement(a).getData());
				i++;
				window.setTimeOut(a);
			}
		}
		//Reduce all timeout
		window.reduceTimeOut();

		sleep(1);
		//Slide window
		int a=0;
		while (a<WINSIZE) {
			if (window.getIsAck(window.getFrameBuffer().getHead()+a) == true) {
				window.slideWindow(frameStorage[j]);
				j++;
			} else {
				break;
			}
		}
	}

	//pthread_join(thread[0], NULL);
	close(sockfd);
	isSocketOpen = false;
}

void Transmitter::error(const char *message) {
	perror(message);
	exit(1);
}

void Transmitter::childProcessACK() {
	// child process
	// receive ACK/NAK from receiver
	struct sockaddr_in srcAddr;
	socklen_t srcLen;
	
	char serializedAck[4];

	while (isSocketOpen) {
		if (recvfrom(sockfd, serializedAck, 4, 0, (struct sockaddr *) &srcAddr, &srcLen) < 0 )
			error("ERROR: recvfrom() receive buffer with size more than expected.\n");

		Ack *ack = new Ack(serializedAck);
		int i=0;
		while (i<WINSIZE) {
			if (window.getFrameBuffer().getElement((window.getFrameBuffer().getHead()+i)%WINSIZE).getNo() == ack->getFrameNo())
				break;
			else
				i++;
		}
		if (i<WINSIZE) {
			if (ack->getAck()==ACK) {
				printf("Received ACK for Frame No: %d\n",ack->getFrameNo());
				window.setAckTrue((window.getFrameBuffer().getHead()+i)%WINSIZE);
				received++;
			} else if (ack->getAck()==NAK) {
				printf("Received NAK for Frame No: %d\n",ack->getFrameNo());
				if (sendto(sockfd, window.getFrameBuffer().getElement(i).getSerialized(), window.getFrameBuffer().getElement(i).getSize(), 0, (const struct sockaddr *) &receiverAddr, sizeof(receiverAddr)) != window.getFrameBuffer().getElement(i).getSize())
						error("ERROR: sendto() sent buffer with size more than expected.\n");
				printf("Sending frame no. %d: %s\n", window.getFrameBuffer().getElement(i).getNo(), window.getFrameBuffer().getElement(i).getData());
				window.setTimeOut(i);
			} 	
		}	
	}
}