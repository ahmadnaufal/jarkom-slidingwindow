#include "frame.h"

using namespace std;

//ctor
Frame::Frame() {

}

//ctor with frameNo & frameData
Frame::Frame(Byte frameNo, char* frameData) {
	no = frameNo;
	data = new char[ sizeof(frameData) ];
	strcpy(data, frameData);
	serialize();
}

//ctor with serializedFrame
Frame::Frame(const char* serializedFrame) {
    char *bptr = (char*) serializedFrame;
    bptr++; 	// skipping through soh
    no = *bptr;

    bptr += 2;	// skipping through stx

    int i = 0;
    char temp[DATAMAX]; 	// temporary array to store the data
    while (i < DATAMAX && *bptr != ETX) {
        temp[i] = *bptr;
        bptr++;
        i++;
    }

    // allocating the data containers for the frame
    data = new char[i];
    memcpy(data, temp, i);

    bptr++; 	// skipping through etx

    unsigned short *sptr = (unsigned short*) bptr;
    checksum = *sptr; sptr++;
}

//cctor
Frame::Frame(const Frame& f) {
	no = f.no;
	memcpy(data, f.data, sizeof(f.data));
	memcpy(serialized, f.serialized, sizeof(f.serialized));
	checksum = f.checksum;
}

//operator =
Frame& Frame::operator=(const Frame& f) {
	no = f.no;
	delete [] data;
	delete [] serialized;

	data = new char[ sizeof(f.data) ];
	serialized = new char [ sizeof(f.serialized) ];
	memcpy(data, f.data, sizeof(f.data));
	memcpy(serialized, f.serialized, sizeof(f.serialized));
	checksum = f.checksum;
	return *this;
}

//dtor
Frame::~Frame() {
	delete[] data;
}

Byte Frame::getNo() {
	return no;
}

char* Frame::getData() {
	return data;
}

unsigned short Frame::getChecksum() {
	return checksum;
}

char* Frame::getSerialized() {
	return serialized;
}

int Frame::getSize() {
	return size;
}

void Frame::serialize() {
	char serializedFrame[DATAMAX+6];
	int i = 0;

	serializedFrame[i++] = SOH;
	serializedFrame[i++] = no;

	serializedFrame[i++] = STX;
	memcpy(serializedFrame + i, data, strlen(data));
	i += strlen(data);

	serializedFrame[i++] = ETX;

	checksum = 1;//Checksum::checksum(serializedFrame,offset);
	memcpy(serializedFrame + i, &checksum, sizeof(checksum));
	i += sizeof(checksum); size = i;
	serialized = new char[i];
	memcpy(serialized, serializedFrame, size);
}