/* 
 * File 		: transmitter.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Transmitter implementation
 */ 


#include "transmitter.h"
#include <iostream>

using namespace std;

//Initialize the static member
int Transmitter::sockfd = 0;
bool Transmitter::isSocketOpen = false;
Queue Transmitter::window(WINSIZE);
int* Transmitter::timeout = new int[WINSIZE];
bool* Transmitter::isAck = new bool[WINSIZE];
sockaddr_in Transmitter::receiverAddr = {0};

Transmitter::Transmitter() { //Ctor

}

Transmitter::Transmitter(char* IP, char* portNo, char* file) { //Ctor with param
	receiverIP = IP;
	port = atoi(portNo);
	tFile = fopen(file, "r");
	frameStorage = new Frame[100];
	if (tFile == NULL) 
		error("ERROR: File text not Found.\n");
	initializeTransmitter();
	readFile();
}

Transmitter::~Transmitter() { //Dtor

}

void Transmitter::initializeTransmitter() {
	if ((server = gethostbyname(receiverIP)) == NULL)
		error("ERROR: Receiver Address is Unknown or Wrong.\n");
	if ((Transmitter::sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		error("ERROR: Create socket failed.\n");
	Transmitter::isSocketOpen = true;

	memset(&Transmitter::receiverAddr, 0, sizeof(Transmitter::receiverAddr));
	Transmitter::receiverAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&Transmitter::receiverAddr.sin_addr.s_addr, server->h_length);
	Transmitter::receiverAddr.sin_port = htons(port);
	printf("Socket has been created at %s:%d\n", receiverIP, port);
}

void Transmitter::readFile() {
	fcount = 0;
	int i = 0;

	char c;
	char* tempdata;

	do {
		c = fgetc(tFile);
		
		if (i==DATAMAX) {
			tempdata[i] = '\0';
			i=0;
			cout << "ini tempdata " << tempdata << endl;
			//frameStorage[fcount] = Frame(fcount%MAXSEQ,tempdata); 
			fcount++;
			cout << "berhasil bikin" << endl;		
		}

		if (c==EOF) {
			tempdata[i] = Endfile;
			tempdata[i+1] = '\0';
			//frameStorage[fcount] = Frame(fcount%MAXSEQ,tempdata);
		} else {
			tempdata[i] = c;
			i++;
		}
		cout << i << " " << tempdata[i-1] << endl;
	} while (c!=EOF);
	cout << "berhasil baca file" << endl;
}

void Transmitter::sendFrames() {
	if (pthread_create(&thread[0], NULL, &childProcessACK, 0) != 0) 
		error("ERROR: Failed to create thread for child. Please free some space.\n");
	
	int i = 0; //number of frame sent
	int j = 0; //number of frame put to the window
	int timeCount = 0;

	//Initialize the window
	while (!Transmitter::window.isFull()){
		if(j<=fcount) {
			Transmitter::window.add(frameStorage[j]);
			Transmitter::timeout[j] = timeCount;
			Transmitter::isAck[j] = false;
			timeCount += 1;
			j++;
		} else {
			break;
		}
	}

	while (i<=fcount) {
		for(int x=0; x<Transmitter::window.getCount(); x++) {
			int a = (x+Transmitter::window.getHead())%WINSIZE; // Ini ga efektif banget maaf ya
			if(Transmitter::timeout[a] == 0) {
				if (sendto(Transmitter::sockfd, Transmitter::window.getElement(a).getSerialized(), Transmitter::window.getElement(a).getSize(), 0, (const struct sockaddr *) &Transmitter::receiverAddr, sizeof(Transmitter::receiverAddr)) != Transmitter::window.getElement(a).getSize())
					error("ERROR: sendto() sent buffer with size more than expected.\n");
				printf("Sending frame no. %d: %s\n", Transmitter::window.getElement(a).getNo(), Transmitter::window.getElement(a).getData());
				i++;
			}
			x++;
		}
		//Reduce all timeout
		for (int x=0; x<WINSIZE; x++) {
			if (Transmitter::timeout[x] > 0)
				x--;
		}
		usleep(1000000);
		int a=0; //Cek pergeseran
		while (a<window.getCount()) {
			if (isAck[window.getHead()+a] == true) {
				window.del();
				window.add(frameStorage[j]);
				isAck[window.getHead()+a] = false;
				timeout[window.getHead()+a] == WINSIZE;
			} else {
				break;
			}
		}
	}
}

void Transmitter::error(const char *message) {
	perror(message);
	exit(1);
}

void* Transmitter::childProcessACK(void *threadid) {
	// child process
	// receive ACK/NAK from receiver
	struct sockaddr_in srcAddr;
	socklen_t srcLen;
	
	char* serializedAck;

	while (Transmitter::isSocketOpen) {
		if (recvfrom(Transmitter::sockfd, serializedAck, 4, 0, (struct sockaddr *) &srcAddr, &srcLen) != sizeof(Ack))
			error("ERROR: recvfrom() receive buffer with size more than expected.\n");

		Ack *ack = new Ack(serializedAck);
		int i=0;
		while (i<window.getCount()) {
			if (Transmitter::window.getElement((Transmitter::window.getHead()+i)%WINSIZE).getNo() == ack->getFrameNo())
				break;
			else
				i++;
		}
		if (ack->getAck()==ACK) {
			printf("Received ACK for Frame No: %d\n",ack->getFrameNo());
			isAck[(Transmitter::window.getHead()+i)%WINSIZE] = true;
		} else if (ack->getAck()==NAK) {
			if (sendto(Transmitter::sockfd, Transmitter::window.getElement(i).getSerialized(), Transmitter::window.getElement(i).getSize(), 0, (const struct sockaddr *) &Transmitter::receiverAddr, sizeof(Transmitter::receiverAddr)) != Transmitter::window.getElement(i).getSize())
					error("ERROR: sendto() sent buffer with size more than expected.\n");
				printf("Sending frame no. %d: %s\n", Transmitter::window.getElement(i).getNo(), Transmitter::window.getElement(i).getData());
		}		
	}

	pthread_exit(NULL);
}
