/* 
 * File 		: slidingwindow.h
 * Author 		: Ahmad Naufal (049) - Tifani Warnita (055) - Asanilta Fahda (079)
 * Description	: Header for sliding window  
 */ 

#ifndef slidingwindow_h
#define slidingwindow_h

/* ASCII Const */ 
#define SOH 1 /* Start of Header Character */
#define STX 2 /* Start of Text Character */
#define ETX 3 /* End of Text Character */ 
#define ENQ 5 /* Enquiry Character */ 
#define ACK 6 /* Acknowledgement */ 
#define BEL 7 /* Message Error Warning */ 
#define CR 13 /* Carriage Return */
#define LF 10 /* Line Feed */ 
#define NAK 21 /* Negative Acknowledgement */
#define Endfile 26 /* End of file character */
#define ESC 27 /* ESC key */ 

/* XON/XOFF protocol */ 
#define XON (0x11) 
#define XOFF (0x13) 

/* Const */ 
#define BYTESIZE 256 /* The maximum value of a byte */
#define MAXLEN 1024 /* Maximum messages length */ 
#define WINSIZE 5 /* Window size */
#define DATAMAX 5	/* Maximum length of data that can be sent per frame */

/* Define receive buffer size */
#define RXQSIZE 8

typedef unsigned char Byte;

typedef struct FRAME {
 	unsigned int soh;
 	Byte frameno;
 	unsigned int stx;
 	Byte *data;
 	unsigned int etx;
 	unsigned short checksum;
} FRAME;

typedef struct ACKN {
	unsigned int ack;
	Byte frameno;
	unsigned short checksum; 
} ACKN;

#endif