#include <stdio.h>
#include "slidingwindow.h"
#include <string.h>

#define POLYNOMM 0x1021

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

unsigned short checksum(Byte *data, size_t bytes )
{
        unsigned short sum1 = 0xff, sum2 = 0xff;
 
        while (bytes) {
                size_t tlen = bytes > 20 ? 20 : bytes;
                bytes -= tlen;
                do {
                        sum2 += sum1 += *data++;
                } while (--tlen);
                sum1 = (sum1 & 0xff) + (sum1 >> 8);
                sum2 = (sum2 & 0xff) + (sum2 >> 8);
        }
        /* Second reduction step to reduce sums to 8 bits */
        sum1 = (sum1 & 0xff) + (sum1 >> 8);
        sum2 = (sum2 & 0xff) + (sum2 >> 8);
        return sum2 << 8 | sum1;
}



unsigned short anotherChecksum(Byte *data, size_t bytes )
{
    unsigned short remain;
    unsigned int calculated = (unsigned int) *data;
    cout << calculated << "  ini cal" << endl;
    unsigned int checksumSize = sizeof(remain);

    /* for (unsigned int i=0; i<bytes; i++) {
        calculated[i] = data[i];
    }
    calculated[bytes] = '\0';*/

    /* for (unsigned int i=0; i<bytes; i++) {
        calculated += data[i];
        printf("1: %d\n", calculated);
        calculated >> 8;
        printf("2: %d\n", calculated);

    } */
    printf("The byte: %d\n", calculated);

    
    return (remain);
}


int main() {
        Byte *str = "04C11DB7";

        printf("%d\n", anotherChecksum(str, sizeof(str)));

        return 0;
}