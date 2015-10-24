
#include "transmitter.h"

using namespace std;

Transmitter::Transmitter() { //Ctor

}

Transmitter::Transmitter(char* IP, char* portNo, char* file) { //Ctor with param
	receiverIP = IP;
	port = atoi(portNo);
	tFile = fopen(file, "r");
	if (tFile == NULL) 
		error("ERROR: File text not Found.\n");
	initializeTransmitter();
	readFile();
}

Transmitter::~Transmitter() { //Dtor

}

void Transmitter::initializeTransmitter() {
	pthread_t thread[1];

	if ((server = gethostbyname(receiverIP)) == NULL)
		error("ERROR: Receiver Address is Unknown or Wrong.\n");
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		error("ERROR: Create socket failed.\n");
	isSocketOpen = true;

	memset(&receiverAddr, 0, sizeof(receiverAddr));
	receiverAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&receiverAddr.sin_addr.s_addr, server->h_length);
	receiverAddr.sin_port = htons(port);
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
			frameStorage[fcount] = Frame(fcount%MAXSEQ,tempdata); 
			fcount++;		
		}

		if (c==EOF) {
			tempdata[i] = Endfile;
			tempdata[i+1] = '\0';
			frameStorage[fcount] = Frame(fcount%MAXSEQ,tempdata);
		} else {
			tempdata[i] = c;
			i++;
		}
	} while (c!=EOF);
}

void Transmitter::sendFrames() {
	if (pthread_create(&thread[0], NULL, childProcessACK, 0) != 0) 
		error("ERROR: Failed to create thread for child. Please free some space.\n");
	
	int i = 0;

	while (i<=fcount) {
		if (sendto(sockfd, frameStorage[i].getSerialized(), frameStorage[i].getSize(), 0, (const struct sockaddr *) &receiverAddr, sizeof(receiverAddr)) != frameStorage[i].getSize())
			error("ERROR: sendto() sent buffer with size more than expected.\n");
				
		printf("Sending frame no. %d: %s\n", frameStorage[i].getNo(),frameStorage[i].getData());
		i++;
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

	while (isSocketOpen) {
		if (recvfrom(sockfd, serializedAck, 4, 0, (struct sockaddr *) &srcAddr, &srcLen) != sizeof(Ack))
			error("ERROR: recvfrom() receive buffer with size more than expected.\n");

		Ack *ack = new Ack(serializedAck);

		printf("Received ACK for Frame No: %d\n",ack->getFrameNo());
	}

	pthread_exit(NULL);
}