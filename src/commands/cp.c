#include "../include/high.h"


int main(int argc, char *argv[]) {
	if (argc < 3) {
		println("Usage: ./cp <source> <destination>");
		return 1;
	}

	int copied = fcopy(argv[1], argv[2]);

	if (copied < 0) println("Failed while trying to copy the file");

	return 0;
}
