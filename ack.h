/* 
 * File 		: ack.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Ack header
 */ 

 #ifndef _ACK_H_
 #define _ACK_H_

 #include "slidingwindow.h"
#include <cstring>

 class Ack {
 public:
 	Ack(); //Ctor
 	Ack(Byte a, Byte f);
 	Ack(const char* serializedAck);
 	Ack(const Ack& a); //Cctor
 	Ack& operator=(const Ack& a); //Operator assignment
 	~Ack(); //Dtor

 	Byte getAck();			// get ack type if ACK/NAK
 	Byte getFrameNo();			// get ack frame number
 	unsigned short getChecksum();	// get checksum of the ack

 private:
 	Byte ack;
	Byte frameno;
	unsigned short checksum;
	char* serialized;

	void serialize();
 };

 #endif