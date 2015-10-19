/* 
 * File 		: transmitter.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Header for transmitter
 */ 

#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "slidingwindow.h"

void error(const char *message);

void *childProcessXONXOFF(void *threadid);
void *childProcessACK(void *threadid);
#endif