/* 
 * File 		: transmitter.c
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Main source code program for transmitter
 */ 
#include "transmitter.h"

/* NETWORKS */
int sockfd, port;		// sock file descriptor and port number
struct hostent *server;
struct sockaddr_in receiverAddr;			// receiver host information
int receiverAddrLen = sizeof(receiverAddr);

/* FILE AND BUFFERS */
FILE *tFile;			// file descriptor
char *receiverIP;		// buffer for Host IP address
char xbuf[1];				// buffer for receiving XON/XOFF characters
FRAME *frameBuffer;
ACKN ackBuffer[WINSIZE];

/* FLAGS */
int isXON = 1;		// flag for XON/XOFF sent
int isSocketOpen;	// flag to indicate if connection from socket is done

int main(int argc, char *argv[]) {
	pthread_t thread[1];

	if (argc < 4) {
		// case if arguments are less than specified
		printf("Please use the program with arguments: %s <target-ip> <port> <filename>\n", argv[0]);
		return 0;
	}

	if ((server = gethostbyname(argv[1])) == NULL)
		error("ERROR: Receiver Address is Unknown or Wrong.\n");

	// creating IPv4 data stream socket
	printf("Creating socket to %s Port %s...\n", argv[1], argv[2]);
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		error("ERROR: Create socket failed.\n");

	// flag set to 1 (connection is established)
	isSocketOpen = 1;

	// initializing the socket host information
	memset(&receiverAddr, 0, sizeof(receiverAddr));
	receiverAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&receiverAddr.sin_addr.s_addr, server->h_length);
	receiverAddr.sin_port = htons(atoi(argv[2]));

	// open the text file
	tFile = fopen(argv[argc-1], "r");
	if (tFile == NULL) 
		error("ERROR: File text not Found.\n");

	if (pthread_create(&thread[0], NULL, childProcess, 0) != 0) 
		error("ERROR: Failed to create thread for child. Please free some space.\n");
	
	// read all characters from file and put in frameBuffer
	int fcount = 0;
	int i = 0;
	char c;

	do {
		c=fgetc(tFile);
		if (i==DATAMAX-1) {
			fcount++;
			i=0;
			frameBuffer[fcount] = malloc(sizeof(FRAME));
			frameBuffer[fcount].data = malloc(DATAMAX*sizeof(Byte));
			frameBuffer[fcount].frameno = fcount%WINSIZE;
			frameBuffer[fcount].checksum = checksum();
			frameBuffer[fcount].soh = SOH;
			frameBuffer[fcount].stx = STX;
			frameBuffer[fcount].etx = ETX;		
		}
		if (c==EOF) {
			frameBuffer[fcount].data[i] = Endfile;
		} else {
			frameBuffer[fcount].data[i] = c;
			i++;
		}
	} while (c!=EOF);

	i=0;
	/*char tempMessage[15+DATAMAX];
	int datalength;*/

	while (i<=fcount) {
		if (isXON) {
			// convert FRAME struct to string
			/*tempMessage[0]=frameBuffer[i].soh;
			tempMessage[4]=frameBuffer[i].frameno;
			tempMessage[5]=frameBuffer[i].stx;
			tempMessage[9]=frameBuffer[i].data;
			datalength = strlen(frameBuffer[i].data);
			tempMessage[9+datalength]=frameBuffer[i].etx;
			tempMessage[13+datalength]=frameBuffer[i].checksum;

			if (sendto(sockfd, tempMessage, 13+datalength, 0, (const struct sockaddr *) &receiverAddr, receiverAddrLen) != 13+datalength)
				error("ERROR: sendto() sent buffer with size more than expected.\n");*/
			if (sendto(sockfd, frameBuffer[i], sizeof(FRAME), 0, (const struct sockaddr *) &receiverAddr, receiverAddrLen) != sizeof(FRAME))
				error("ERROR: sendto() sent buffer with size more than expected.\n");
			
			printf("Sending frame no. %d: %s", i,frameBuffer[i].data);
			i++;
		} else {
			while (!isXON) {
				printf("Waiting for XON...\n");
				sleep(1);
			}
		}
		sleep(1);
	}

	fclose(tFile);
	
	printf("Frame sending done! Closing sockets...\n");
	close(sockfd);
	isSocketOpen = 0;
	printf("Socket Closed!\n");

	// finishing program and closing
	printf("TRANSMITTER Finished transmitting frames!\n");
	printf("TRANSMITTER saying goodbye and thanks!\n");

	return 0;
}

void error(const char *message) {
	perror(message);
	exit(1);
}

void *childProcessXONXOFF(void *threadid) {
	// child process
	// read if there is XON/XOFF sent by receiver using recvfrom()
	struct sockaddr_in srcAddr;
	int srcLen = sizeof(srcAddr);
	while (isSocketOpen) {
		if (recvfrom(sockfd, xbuf, 1, 0, (struct sockaddr *) &srcAddr, &srcLen) != 1)
			error("ERROR: recvfrom() receive buffer with size more than expected.\n");

		if (xbuf[0] == XOFF) {
			isXON = 0;
			printf("[XOFF] Receiving XOFF. Rest a while buddy...\n");
		} else if (xbuf[0] == XON) {
			isXON = 1;
			printf("[XON] Receiving XON. Work again!\n");
		} else {
			printf("What the hell man?\n");
		}
	}

	pthread_exit(NULL);
}

void *childProcessACK(void *threadid) {
	// child process
	// receive ACK/NAK from receiver
	struct sockaddr_in srcAddr;
	int srcLen = sizeof(srcAddr);
	ACKN acktemp;

	while (isSocketOpen) {
		if (recvfrom(sockfd, acktemp, sizeof(ACKN), 0, (struct sockaddr *) &srcAddr, &srcLen) != sizeof(ACKN))
			error("ERROR: recvfrom() receive buffer with size more than expected.\n");

		ackBuffer[acktemp.frameno] = acktemp;
	}

	pthread_exit(NULL);
}