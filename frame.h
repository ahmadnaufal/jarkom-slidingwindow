#ifndef FRAME_H
#define FRAME_H
#include "slidingwindow.h"
#include <cstring>

using namespace std;

class Frame {
public:
	Frame();
	Frame(int frameNo);
	Frame(Byte* serializedFrame);
	Frame(const Frame& f);
	Frame& operator= (const Frame& F);
	~Frame();
	Byte getNo();
	Byte* getData();
	unsigned short getChecksum();
	Byte* getSerialized();

private:
	Byte no;
	Byte* data;
	unsigned short checksum;
	Byte* serialized;
	const unsigned int soh = SOH;
	const unsigned int stx = STX;
	const unsigned int etx = ETX; 
	void serialize();
};

#endif