#ifndef FRAME_H
#define FRAME_H
#include "slidingwindow.h"
#include <cstring>

using namespace std;

class Frame {
public:
	Frame();
	Frame(int frameNo, char* frameData);
	Frame(const char* serializedFrame);
	Frame(const Frame& f);
	Frame& operator= (const Frame& F);
	~Frame();
	Byte getNo();
	char* getData();
	unsigned short getChecksum();
	char* getSerialized();
	int getSize();

private:
	Byte no;
	char* data;
	unsigned short checksum;
	char* serialized;
	const unsigned int soh = SOH;
	const unsigned int stx = STX;
	const unsigned int etx = ETX;
	int size;

	void serialize();
};

#endif