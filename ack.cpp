/* 
 * File 		: ack.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Ack implementation
 */ 

 #include "ack.h"
 #include <iostream>

 using namespace std;

 const int Ack::acksize = 4;

 Ack::Ack() { //Ctor
 	ack = 0;
 	frameno = 0;
 	checksum = 0;
 	serialized = new char[1];
 }

 Ack::Ack(Byte a, Byte f) { //Ctor with param
 	ack = a;
 	frameno = f;
 	serialized = new char[acksize];
 	serialize();
 }

 Ack::Ack(const char* serializedAck) {	// ack constructor with a serialized ack as parameter
 	// build ack from serialized structure
 	Byte *bptr = (Byte*) serializedAck;
    ack = *bptr; bptr++;

    frameno = *bptr; bptr++; 	// skipping through etx

    unsigned short *sptr = (unsigned short*) bptr;
    checksum = *sptr; sptr++;
 }

 Ack::Ack(const Ack& a) {
 	int i;
 	ack = a.ack;
 	frameno = a.frameno;
 	checksum = a.checksum;
 	serialized = new char[sizeof(a.serialized)];
 	for (i=0; i<sizeof(serialized); ++i)
 		serialized[i] = a.serialized[i];
 	serialized[i] = '\0';
 }

 Ack& Ack::operator=(const Ack& a) {
 	int i;
 	ack = a.ack;
 	frameno = a.frameno;
 	checksum = a.checksum;
 	serialized = new char[sizeof(a.serialized)];
 	for (i=0; i<sizeof(serialized); ++i)
 		serialized[i] = a.serialized[i];
 	serialized[i] = '\0';
 	return *this;
 }

 Ack::~Ack() { //Dtor
 	delete [] serialized;
 }

Byte Ack::getAck() {
	return ack;
}

Byte Ack::getFrameNo() {
	return frameno;
}

unsigned short Ack::getChecksum() {
	return checksum;
}

char* Ack::getSerialized() {
	return serialized;
}

void Ack::serialize() {
	char serializedAck[acksize];
	
	int i = 0;
	serializedAck[i] = ack; i++;
	serializedAck[i] = frameno; i++;

	checksum = Checksum::createChecksum(serializedAck,i);
	memcpy(serializedAck + i, &checksum, sizeof(checksum));
	// copying to serialized attribute
	memcpy(serialized, serializedAck, sizeof(serializedAck));
}