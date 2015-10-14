/* File : T1_rx.cpp */

#include "receiver.h"

Byte rxbuf[RXQSIZE];
QTYPE rcvq = { 0, 0, 0, RXQSIZE, rxbuf };
QTYPE *rxq = &rcvq;
Byte sent_xonxoff = XON;
unsigned send_xon = 0,
send_xoff = 0;
int endFileReceived;

/* Socket */
int sockfd; // listen on sock_fd
struct sockaddr_in adhost;
struct sockaddr_in srcAddr;
unsigned int srcLen = sizeof(srcAddr);

int main(int argc, char *argv[])
{
 	pthread_t thread[1];

 	if (argc<2) {
 		// case if arguments are less than specified
 		printf("Please use the program with arguments: %s <port>\n", argv[0]);
 		return 0;
 	}

 	printf("Creating socket to self in Port %s...\n", argv[1]);
 	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
 		printf("ERROR: Create socket failed.\n");

 	bzero((char*) &adhost, sizeof(adhost));
 	adhost.sin_family = AF_INET;
 	adhost.sin_port = htons(atoi(argv[1]));
 	adhost.sin_addr.s_addr = INADDR_ANY;

 	if(bind(sockfd, (struct sockaddr*) &adhost, sizeof(adhost)) < 0)
 		error("ERROR: Binding failed.\n");

 	endFileReceived = 0;

 	memset(rxbuf, 0, sizeof(rxbuf));
 	
 	/* Create child process */
 	if(pthread_create(&thread[0], NULL, childRProcess, 0)) 
 		error("ERROR: Failed to create thread for child.\n");

	/* parent process: unlimited looping */
	Byte c;
	while (1) {
 		c = *(rcvchar(sockfd, rxq));

 		if (c == Endfile)
 			endFileReceived = 1;
	}

	printf("Receiving End of File... Receiver Complete!\n");
	printf("Waiting for the next transmission...\n");

	return 0;
}


void error(const char *message) {
	perror(message);
	exit(1);
}

static Byte *rcvchar(int sockfd, QTYPE *queue)
{
 	/* Insert code here. Read a character from socket and put it to the receive buffer.
 	If the number of characters in the receive buffer is above certain level, then send
 	XOFF and set a flag (why?). Return a pointer to the buffer where data is put. */
 	Byte* current;
 	char tempBuf[1];
 	char b[1];
 	static int counter = 1;

 	if (recvfrom(sockfd, tempBuf, 1, 0, (struct sockaddr *) &srcAddr, &srcLen) < 0)
 		error("ERROR: Failed to receive character from socket\n");

 	current = (Byte *) malloc(sizeof(Byte));
 	*current = tempBuf[0];

 	if (*current != Endfile) {
 		printf("Receiving byte no. %d: ", counter++);
		switch (*current) {
			case CR:	printf("\'Carriage Return\'\n");
						break;
			case LF:	printf("\'Line Feed\'\n");
						break;
			case Endfile:
						printf("\'End of File\'\n");
						break;
			case 255:	break;
			default:	printf("\'%c\'\n", *current);
						break;
		}
 	}

 	// adding char to buffer and resync the buffer queue
 	if (queue->count < 8) {
 		queue->rear = (queue->count > 0) ? (queue->rear+1) % 8 : queue->rear;
 		queue->data[queue->rear] = *current;
 		queue->count++;
 	}

 	// if the buffer reaches Minimum Upperlimit, send XOFF to Transmitter
 	if(queue->count >= (MIN_UPPERLIMIT) && sent_xonxoff == XON) {
 		printf("[XOFF] Buffer reached Minimum Upperlimit. Sending XOFF to transmitter...\n");
 		send_xoff = 1; send_xon = 0;
 		b[0] = sent_xonxoff = XOFF;

 		if(sendto(sockfd, b, 1, 0,(struct sockaddr *) &srcAddr, srcLen) < 0)
 			error("ERROR: Failed to send XOFF.\n");
 	}

 	return current;
}


void *childRProcess(void *threadid) {
	Byte *data,
	*current = NULL;

 	while (1) {
 		current = q_get(rxq, data);

 		// if end file, quit the process
 		/*if (current != NULL && endFileReceived)
 			break;*/
 		// introduce some delay here
 		sleep(2);
 	}

 	pthread_exit(NULL);
}

static Byte *q_get(QTYPE *queue, Byte *data)
/* q_get returns a pointer to the buffer where data is read or NULL if buffer is empty. */
{
 	Byte *current = NULL;
 	char b[1];
 	static int counter = 1;

 	/* Only consume if the buffer is not empty */
 	if (queue->count > 0) {
 		current = (Byte *) malloc(sizeof(Byte));
 		*current = queue->data[queue->front];
 		//if (*current == Endfile) exit(0);
 		// incrementing front (circular) and reducing number of elements
 		queue->front++;
 		if (queue->front == 8) queue->front = 0;
 		queue->count--;

 		printf("CONSUME! Consuming byte no. %d: ", counter++);
		switch (*current) {
			case CR:	printf("\'Carriage Return\'\n");
						break;
			case LF:	printf("\'Line Feed\'\n");
						break;
			case Endfile:
						printf("\'End of File\'\n");
						break;
			case 255:	break;
			default:	printf("\'%c\'\n", *current);
						break;
		}
 	}

 	/* Insert code here.  Retrieve data from buffer, save it to "current" and "data"
 	If the number of characters in the receive buffer is below certain  level, then send
 	XON. Increment front index and check for wraparound. */
 	if (queue->count <= MAX_LOWERLIMIT && sent_xonxoff == XOFF) {
 		printf("[XON] Buffer reaches Maximum Lowerlimit. Sending XON to transmitter...\n");
 		send_xon = 1; send_xoff = 0;

 		b[0] = sent_xonxoff = XON;
 		if(sendto(sockfd, b, 1, 0, (struct sockaddr *) &srcAddr, srcLen) < 0)
 			error("ERROR: Failed to send XON.\n");
 	}	

 	// return the Byte consumed
 	return current;
}
