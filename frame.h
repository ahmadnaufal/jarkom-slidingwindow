#ifndef FRAME_H
#define FRAME_H
#include "slidingwindow.h"
#include "checksum.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std;

class Frame {
public:
	Frame();
	Frame(Byte frameNo, char* frameData);
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
	/* Actual properties of the Frame */
	Byte no;					// frame number
	char* data;					// data containers
	unsigned short checksum;	// checksum for data

	char* serialized;
	int size;

	void serialize();
};

#endif