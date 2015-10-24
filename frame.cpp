#include "frame.h"

using namespace std;

//ctor
Frame::Frame() {

}

//ctor with frameNo & frameData
Frame::Frame(int frameNo, Byte* frameData) {
	no = frameNo;
	data = frameData;
	serialize();
}

//ctor with serializedFrame
Frame::Frame(const Byte* serializedFrame) {
    unsigned int *ptr = (unsigned int*) data;    
    stx = *ptr; ptr++;

    Byte *bptr = (Byte*) ptr;
    no = *bptr; bptr++;

    ptr = (unsigned int*) bptr;
    stx = *ptr; ptr++;

    bptr = (Byte*) ptr;
    int i = 0;
    while (i < DATAMAX && *bptr != '\0') {
        data[i] = *bptr;
        bptr++;
        i++;
    }

    ptr = (unsigned int*) bptr;
    etx = *ptr; ptr++;

    unsigned short *sptr = (unsigned short*) ptr;
    checksum = *sptr; sptr++;
}

//cctor
Frame::Frame(const Frame& f) {
	no = f.no;
	strcpy(data, f.data);
	strcpy(serialized, f.serialized);
	checksum = f.checksum;

}

//operator =
Frame& Frame::operator=(const Frame& f) {
	no = f.no;
	delete [] data;
	strcpy(data, f.data);
	strcpy(serialized, f.serialized);
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

Byte* Frame::getData() {
	return data;
}

unsigned short Frame::getChecksum() {
	return checksum;
}

Byte* Frame::getSerialized() {
	return serialized;
}

void Frame::serialize() {
	int offset=0;
	Byte* serializedFrame[15+DATAMAX];
	memcpy(serializedFrame + offset, &soh, sizeof(soh));
	offset+=sizeof(soh);
	memcpy(serializedFrame + offset, &frameno, sizeof(frameno));
	offset+=sizeof(no);
	memcpy(serializedFrame + offset, &stx, sizeof(stx));
	offset+=sizeof(stx);
	memcpy(serializedFrame + offset, data, strlen(data));
	offset+=strlen(data);
	memcpy(serializedFrame + offset, &etx, sizeof(etx));
	offset+=sizeof(etx);
	checksum = Checksum::checksum(serializedFrame,offset);
	memcpy(serializedFrame + offset, &checksum, sizeof(checksum));
	offset+=sizeof(checksum);
	serialized = serializedFrame;
}