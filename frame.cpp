#include "frame.h"

using namespace std;

//ctor
Frame::Frame() {

}

//ctor with frameNo & frameData
Frame::Frame(int frameNo, char* frameData) {
	no = frameNo;
	data = frameData;
	serialize();
}

//ctor with serializedFrame
Frame::Frame(const char* serializedFrame) {
    unsigned int *ptr = (unsigned int*) data;    
    ptr++;

    char *bptr = (char*) ptr;
    no = *bptr; bptr++;

    ptr = (unsigned int*) bptr;
    ptr++;

    bptr = (char*) ptr;
    int i = 0;
    while (i < DATAMAX && *bptr != '\0') {
        data[i] = *bptr;
        bptr++;
        i++;
    }

    ptr = (unsigned int*) bptr;
    ptr++;

    unsigned short *sptr = (unsigned short*) ptr;
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
	int offset=0;
	Byte serializedFrame[15+DATAMAX];
	memcpy(serializedFrame + offset, &soh, sizeof(soh));
	offset+=sizeof(soh);
	memcpy(serializedFrame + offset, &no, sizeof(no));
	offset+=sizeof(no);
	memcpy(serializedFrame + offset, &stx, sizeof(stx));
	offset+=sizeof(stx);
	memcpy(serializedFrame + offset, data, strlen(data));
	offset+=strlen(data);
	memcpy(serializedFrame + offset, &etx, sizeof(etx));
	offset+=sizeof(etx);
	checksum = 1;//Checksum::checksum(serializedFrame,offset);
	memcpy(serializedFrame + offset, &checksum, sizeof(checksum));
	offset+=sizeof(checksum); size = offset;
	memcpy(serialized, serializedFrame, sizeof(serializedFrame));
}