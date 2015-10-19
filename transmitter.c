 /* File 	: transmitter.c */
#include "transmitter.h"

/* NETWORKS */
int sockfd, port;		// sock file descriptor and port number
struct hostent *server;
struct sockaddr_in receiverAddr;			// receiver host information
int receiverAddrLen = sizeof(receiverAddr);

/* FILE AND BUFFERS */
FILE *tFile;			// file descriptor
char *receiverIP;		// buffer for Host IP address
char buf[BUFMAX+1];		// buffer for character to send
char xbuf[BUFMAX+1];	// buffer for receiving XON/XOFF characters

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
	
	// this is the parent process
	// use as char transmitter from the text file
	// connect to receiver, and read the file per character
	int counter = 1;
	while ((buf[0] = fgetc(tFile)) != EOF) {
		if (isXON) {
			if (sendto(sockfd, buf, BUFMAX, 0, (const struct sockaddr *) &receiverAddr, receiverAddrLen) != BUFMAX)
				error("ERROR: sendto() sent buffer with size more than expected.\n");
			
			printf("Sending byte no. %d: ", counter++);
			switch (buf[0]) {
				case CR:	printf("\'Carriage Return\'\n");
							break;
				case LF:	printf("\'Line Feed\'\n");
							break;
				case Endfile:
						printf("\'End of File\'\n");
						break;
				case 255:	break;
				default:	printf("\'%c\'\n", buf[0]);
							break;
			}
		} else {
			while (!isXON) {
				printf("Waiting for XON...\n");
				sleep(1);
			}
		}
		sleep(1);
	}

	// sending endfile to receiver, marking the end of data transfer
	buf[0] = Endfile;
	sendto(sockfd, buf, BUFMAX, 0, (const struct sockaddr *) &receiverAddr, receiverAddrLen);
	fclose(tFile);
	
	printf("Byte sending done! Closing sockets...\n");
	close(sockfd);
	isSocketOpen = 0;
	printf("Socket Closed!\n");

	// finishing program and closing
	printf("TRANSMITTER Finished transmitting bytes!\n");
	printf("TRANSMITTER saying goodbye and thanks!\n");

	return 0;
}

void error(const char *message) {
	perror(message);
	exit(1);
}

void *childProcess(void *threadid) {
	// child process
	// read if there is XON/XOFF sent by receiver using recvfrom()
	struct sockaddr_in srcAddr;
	int srcLen = sizeof(srcAddr);
	while (isSocketOpen) {
		if (recvfrom(sockfd, xbuf, BUFMAX, 0, (struct sockaddr *) &srcAddr, &srcLen) != BUFMAX)
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