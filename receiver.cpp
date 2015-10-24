/* 
 * File 		: receiver.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Receiver implementation
 */ 

#include "receiver.h"

using namespace std;

Receiver::Receiver() {

}

Receiver::Receiver(char* _portNo) { //Ctor
	port = atoi(_port);
	frameEater = new Queue();

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

 	endFileReceived = 0;

 	int res = bind(sockfd, (struct sockaddr*) &adhost, sizeof(adhost))
	if (res < 0)
 		error("ERROR: Binding failed.\n");
}

void Receiver::receiveFrames() {
	if(pthread_create(&thread[0], NULL, childRProcess, 0)) 
 		error("ERROR: Failed to create thread for child.\n");

	/* parent process: unlimited looping */
	Frame temp;
	while (1) {
 		temp = *(rcvframe());

 		if (c == Endfile)
 			endFileReceived = 1;
	}
}

static void Receiver::rcvframe() {
	/* Insert code here. Read a character from socket and put it to the receive buffer.
 	If the number of characters in the receive buffer is above certain level, then send
 	XOFF and set a flag (why?). Return a pointer to the buffer where data is put. */
 	char* serializedFrame;
 	struct sockaddr_in srcAddr; socklen_t srcLen;

 	if (recvfrom(sockfd, serializedFrame, 20, 0, (struct sockaddr *) &srcAddr, &srcLen) < 0)
 		error("ERROR: Failed to receive character from socket\n");

	Frame *temp = new Frame(serializedFrame); 	

 	// printf("Receiving frame no. %d: %s", , frame[0].data);

 	// receiving frame from transmitter
 	if (temp->checksum == Checksum::checksum(serializedFrame,offset)) {
 		printf("[ACK] Package %d secure. Sending ACK %d...\n", temp->frameno, temp->frameno);
 		Ack *ack = new Ack(ACK, temp->frameno);

 		// serialize the ACK packet
 		// send the serialized ACK packet
 		if(sendto(sockfd, ack->getSerialized(), ack->getSerializedSize(), 0,(struct sockaddr *) &srcAddr, srcLen) < 0)
 			error("ERROR: Failed to send ACK.\n");

 		// adding char to buffer and resync the buffer queue
 		if (queue->count < 8) {
	 		queue->rear = (queue->count > 0) ? (queue->rear+1) % 8 : queue->rear;
	 		queue->data[queue->rear] = frame[0];
	 		queue->count++;
	 	}

 	} else {
 		printf("[NAK] Package %d error or corrupt. Sending NAK %d...\n", frame[0].frameno, frame[0].frameno);
 		Ack *ack = new Ack(NAK, temp->frameno);

 		// serialize the ACK packet
 		// send the serialized ACK packet
 		if(sendto(sockfd, ack->getSerialized(), ack->getSerializedSize(), 0,(struct sockaddr *) &srcAddr, srcLen) < 0)
 			error("ERROR: Failed to send ACK.\n");
 	}
}