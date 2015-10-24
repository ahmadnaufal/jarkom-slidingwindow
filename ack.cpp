/* 
 * File 		: ack.cpp
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Receiver implementation
 */ 

 #include "ack.h"

 using namespace std;

 Ack::Ack() { //Ctor
 	ack = 0;
 	frameno = 0;
 	checksum = 0;
 }

 Ack::Ack(Byte a, Byte f, unsigned short c) { //Ctor with param
 	ack = a;
 	frameno = f;
 	checksum = c;
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

}

int main() {
 	
 	return 0;
}