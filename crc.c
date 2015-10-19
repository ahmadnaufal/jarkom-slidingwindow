/* 
 * File 		: crc.c
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Implement the Cyclic Redundancy Check (CRC) error detecting code
 */ 

#include <stdio.h>
#include "slidingwindow.h"

#define GEN_POLYNOMIAL 0x8408 /* Generator Polynomial x^16 + x^12 + x^5 + 1 (reverse) */
 /* The highest bit assumed to always be set so only need 16 bit even though the checksum
 size is 16 */


unsigned short crcChecksum (Byte *data_p, unsigned short length) {
	unsigned char i;
	unsigned int data;
	unsigned int remainder = 0xffff;

	if (length == 0)
		return (~remainder);
	
	do {
		for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8; 
                 i++, data >>= 1)
            {
                  if ((remainder & 0x0001) ^ (data & 0x0001))
                        remainder = (remainder >> 1) ^ GEN_POLYNOMIAL;
                  else  remainder >>= 1;
            }
	} while (--length);

	remainder = ~remainder;
	data = remainder;
	remainder = (remainder << 8) | (data >> 8 & 0xff);

	return (remainder);
}

int main() {
    Byte *str = "E100CAFE";

    printf("%d\n", crcChecksum(str, sizeof(str)));

    return 0;
}