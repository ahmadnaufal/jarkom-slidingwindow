#include "frame.h"
#include <iostream>

using namespace std;

//ctor
Frame::Frame() {
	data = new char[1];
	serialized = new char[1];
	no = 0;
	size = 0;
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
	serialized = (char*) serializedFrame;
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

<<<<<<< HEAD
    unsigned short *sptr = (unsigned short*) bptr;
    checksum = *sptr;
    cout << "make frame: " << checksum << endl;
     sptr++;
    size = i + 6;
    cout << "make frame: " << getSize() << endl;
=======
    int offset = 4 + i;
    //unsigned short* sptr = (unsigned short*) bptr;
    //checksum = *sptr;
    memcpy(&checksum, serializedFrame + offset, 2);
    //sptr++;
    size = i + 6;
    /*cout << "deserialized: " << endl;
    for (int j=0;j<size;j++) {
    	cout << serialized[j] << endl;
    }*/
>>>>>>> b25966cb428c6f87d0049b87e91ea282dd202a4d
}

//cctor
Frame::Frame(const Frame& f) {
	no = f.no;
<<<<<<< HEAD
	data = new char[ sizeof(f.data) ];
	serialized = new char [ sizeof(f.serialized) ];
=======
	size = f.size;

	data = new char[ sizeof(f.data) ];
	serialized = new char [size];
>>>>>>> b25966cb428c6f87d0049b87e91ea282dd202a4d
	memcpy(data, f.data, sizeof(f.data));
	memcpy(serialized, f.serialized, size);
	
	checksum = f.checksum;
	size = f.size;
}

//operator =
Frame& Frame::operator=(const Frame& f) {
	no = f.no;
	delete [] data;
	delete [] serialized;

	data = new char[ sizeof(f.data) ];
	size = f.size;
	serialized = new char [size];
	memcpy(data, f.data, sizeof(f.data));
	memcpy(serialized, f.serialized, size);

	checksum = f.checksum;
<<<<<<< HEAD
	size = f.size;
=======

>>>>>>> b25966cb428c6f87d0049b87e91ea282dd202a4d
	return *this;
}

//dtor
Frame::~Frame() {
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

	checksum = Checksum::createChecksum(serializedFrame, i);
	memcpy(serializedFrame + i, &checksum, sizeof(checksum));
	i += sizeof(checksum); size = i;
	cout << getSize() << endl;
	serialized = new char[size];
	memcpy(serialized, serializedFrame, size);
}

// int main(int argc, char const *argv[])
// {
// 	char hello[] = "hello";
// 	Byte car = 3;
// 	Frame frame(car, hello);
// 	cout << frame.getNo() << " " << frame.getData() << " " << frame.getChecksum();

// 	Frame t(frame.getSerialized());
// 	cout << t.getNo() << " " << t.getData() << " " << t.getChecksum();
// 	return 0;
// }