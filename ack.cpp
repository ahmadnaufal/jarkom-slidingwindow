/* 
 * File 		: ack.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Ack implementation
 */ 

 #include "ack.h"

 using namespace std;

 Ack::Ack() { //Ctor
 	ack = 0;
 	frameno = 0;
 	checksum = 0;
 }

 Ack::Ack(Byte a, Byte f) { //Ctor with param
 	ack = a;
 	frameno = f;
 	serialize();
 }

 Ack::Ack(const char* serializedAck) {
 	Byte *bptr = (Byte*) serializedAck;
    bptr++; 	// skipping through soh
    ack = *bptr; bptr++;

    frameno = *bptr; bptr++; 	// skipping through etx

    unsigned short *sptr = (unsigned short*) bptr;
    checksum = *sptr; sptr++;
 }

 Ack::Ack(const Ack& a) {
 	ack = a.ack;
 	frameno = a.frameno;
 	checksum = a.checksum;
 }

 Ack& Ack::operator=(const Ack& a) {
 	ack = a.ack;
 	frameno = a.frameno;
 	checksum = a.checksum;
 	return *this;
 }

 Ack::~Ack() { //Dtor
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

void Ack::serialize() {
	int offset=0;
	Byte serializedAck[4];
	memcpy(serializedAck + offset, &ack, sizeof(ack));
	offset+=sizeof(ack);
	memcpy(serializedAck + offset, &frameno, sizeof(frameno));
	offset+=sizeof(frameno);
	checksum = 1;//Checksum::checksum(serializedAck,offset);
	memcpy(serializedAck + offset, &checksum, sizeof(checksum));
	offset+=sizeof(checksum);
	memcpy(serialized, serializedAck, sizeof(serializedAck));
}