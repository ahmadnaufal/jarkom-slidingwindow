#include "receiver.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		// case if arguments are less than specified
		printf("Please use the program with arguments: %s <port>\n", argv[0]);
		return 0;
	} else {
		Receiver rc(argv[1]);
	}

	return 0;
}