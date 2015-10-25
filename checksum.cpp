/* 
 * File         : checksum.h
 * Author       : Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description  : Checksum implementation
 */ 

#include "checksum.h"

#define GENPOLY 0x11933

using namespace std;

unsigned short Checksum::createChecksum(Byte *data, size_t bytes) {
    unsigned short remain;
    unsigned long long msg;

    int i=0, j=0;
    while (i<bytes) {
        msg = msg << j;
        msg += data[i];
        j+=8;
        ++i;
        cout << msg << endl;
    };

    unsigned int size_bit = (bytes * sizeof(Byte) * 8) + (sizeof(remain) * 8); 
    msg = msg << (sizeof(remain) * 8);
    unsigned int polypoly;

    while (size_bit>(sizeof(remain) * 8)) {
        polypoly = GENPOLY << (size_bit-sizeof(remain)*8-1);
        if ((msg>>(size_bit-1)) & 0x1 ==1) {
            msg = msg ^ polypoly;
        } else {
        }
        size_bit--;
    }
    remain = msg & 0xffff;
    return (remain);
}