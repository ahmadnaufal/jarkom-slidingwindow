/* 
 * File 		: ack.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Ack header
 */ 

 #ifndef _ACK_H_
 #define _ACK_H_

 #include "slidingwindow.h"

 class Ack {
 public:
 	Ack(); //Ctor
 	Ack(unsigned int a, Byte f, unsigned short c);
 	Ack(const Ack& a); //Cctor
 	Ack& operator=(const Ack& a); //Operator assignment
 	~Ack(); //Dtor

 private:
 	unsigned int ack;
	Byte frameno;
	unsigned short checksum; 
 };

 #endif