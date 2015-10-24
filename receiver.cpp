/* 
 * File 		: receiver.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Receiver implementation
 */ 

#include "receiver.h"

using namespace std;

Receiver::Receiver(char* _port) { //Ctor
	port = atoi(_port);

	printf("Creating socket to self in Port %s...\n", argv[1]);
 	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
 		error("ERROR: Create socket failed.\n");

 	bzero((char*) &adhost, sizeof(adhost));
 	adhost.sin_family = AF_INET;
 	adhost.sin_port = htons(port);
 	adhost.sin_addr.s_addr = INADDR_ANY;

 	endFileReceived = 0;
}

void Receiver::bind() {
	int res = bind(sockfd, (struct sockaddr*) &adhost, sizeof(adhost))
	if (res < 0)
 		error("ERROR: Binding failed.\n");
}