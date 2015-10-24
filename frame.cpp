#include "frame.h"

using namespace std;

//ctor
Frame::Frame() : soh(SOH), stx(STX), etx(ETX) {

}

//ctor with frameNo & frameData
Frame::Frame(Byte frameNo, char* frameData) : soh(SOH), stx(STX), etx(ETX) {
	no = frameNo;
	data = frameData;
	serialize();
}

//ctor with serializedFrame
Frame::Frame(const char* serializedFrame) : soh(SOH), stx(STX), etx(ETX) {
    char *bptr = (char*) serializedFrame;
    bptr++; 	// skipping through soh
    no = *bptr;

    bptr += 2;	// skipping through stx

    int i = 0;
    while (i < DATAMAX && *bptr != etx) {
        data[i] = *bptr;
        bptr++;
        i++;
    }

    bptr++; 	// skipping through etx

    unsigned short *sptr = (unsigned short*) bptr;
    checksum = *sptr; sptr++;
}

//cctor
Frame::Frame(const Frame& f) : soh(f.soh), stx(f.stx), etx(f.etx) {
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