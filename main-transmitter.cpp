#include "transmitter.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 4) {
		// case if arguments are less than specified
		printf("Please use the program with arguments: %s <target-ip> <port> <filename>\n", argv[0]);
		return 0;
	} else {
		Transmitter tm(argv[1], argv[2], argv[3]);
	}

	return 0;
}