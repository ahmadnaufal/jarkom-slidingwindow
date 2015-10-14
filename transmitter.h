#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFMAX 5	/* Maximum size of buffer that can be sent */

void error(const char *message);

void *childProcess(void *threadid);
#endif