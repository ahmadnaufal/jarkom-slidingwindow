#include <stdio.h>
#include "slidingwindow.h"

unsigned char checksum(Byte *data, size_t bytes )
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

int main() {
        Byte *str = "ahahaha";

        printf("%d\n", checksum(str, sizeof(str)));

        return 0;
}