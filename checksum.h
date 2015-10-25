/* 
 * File 		: checksum.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Checksum header
 */ 

#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_


#include "slidingwindow.h"
#include <cstring> 
#include <iostream>
#include <stdio.h>

using namespace std;

class Checksum {
public:
	Checksum();
	~Checksum();
	static unsigned short createChecksum(char *data, int bytes);
private:

};

#endif